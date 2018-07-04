#include <videostream.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <wiringPi.h>
#include <ctime>

VideoStream::VideoStream()
{

}

VideoStream::VideoStream(VideoSettings& videoSettings)
   : settings(videoSettings)
{
    
}

VideoStream::~VideoStream()
{
    if (pipelineStarted) stopPipeline();
}

void VideoStream::startPipeline()
{
    if(settings.camera > 1)
    {
        std::cout << "Invalid camera number [0-1]\n";
        exit(1);
    }
    if (settings.addr.ip.empty() || settings.addr.port.empty())
    {
        std::cout << "Invalid address\n";
        exit(1);
    }
    
    if (pipelineStarted)
    {
        std::cout << "Pipeline already started\n";
    }
    else
    {
        pipelineThread = std::thread([this] { startPipelineInThread(); });
        pipelineStarted = true;
    }
}

void VideoStream::stopPipeline() {
    if (pipelineStarted)
    {
        gst_element_set_state (data.pipeline, GST_STATE_READY);
        g_main_loop_quit (data.loop);
        std::cout << "Waiting for pipeline thread to finish\n";
        pipelineThread.join();
    }
    else std::cout << "Pipeline not started\n";
}

static void callbackMessage(GstBus *bus, GstMessage *msg, VideoStream::PipelineData *pipedata) {

  switch (GST_MESSAGE_TYPE (msg)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (msg, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      gst_element_set_state (pipedata->pipeline, GST_STATE_READY);
      g_main_loop_quit (pipedata->loop);
      break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      gst_element_set_state (pipedata->pipeline, GST_STATE_READY);
      g_main_loop_quit (pipedata->loop);
      break;
    case GST_MESSAGE_BUFFERING: {
      gint percent = 0;

      /* If the stream is live, we do not care about buffering. */
      if (pipedata->is_live) break;

      gst_message_parse_buffering (msg, &percent);
      g_print ("Buffering (%3d%%)\r", percent);
      /* Wait until buffering is complete before start/resume playing */
      if (percent < 100)
        gst_element_set_state (pipedata->pipeline, GST_STATE_PAUSED);
      else
        gst_element_set_state (pipedata->pipeline, GST_STATE_PLAYING);
      break;
    }
    case GST_MESSAGE_CLOCK_LOST:
      /* Get a new clock */
      gst_element_set_state (pipedata->pipeline, GST_STATE_PAUSED);
      gst_element_set_state (pipedata->pipeline, GST_STATE_PLAYING);
      break;
    default:
      /* Unhandled message */
      break;
    }
}

void VideoStream::startPipelineInThread()
{
    GstElement *pipeline;
    GstBus *bus;
    GstStateChangeReturn ret;
    GMainLoop *main_loop;
    GError *error;
    
    gst_init(NULL, NULL);

    std::string pipelineCommand;
    if (settings.record)
    {
        // Test to see if storage directory exists
        if (mkdir(videoStorageDirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
        {
            if( errno == EEXIST ) {
                std::cout << "Directory already exists\n";
            }
        }

        // Figure out file name
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        std::string fileName = videoStorageDirectory + "halo-video-cam"
            + std::to_string(settings.camera)
            + std::to_string(now->tm_year + 1900) + '-' 
            + std::to_string(now->tm_mon + 1) + '-'
            + std::to_string(now->tm_mday) + '-'
            + std::to_string(now->tm_hour) + ':'
            + std::to_string(now->tm_min) + ':'
            + std::to_string(now->tm_sec);

        if (settings.stream)
        {
            pipelineCommand = "rpicamsrc bitrate=" + std::to_string(settings.bitrate) +
                " preview=0 hflip=" + std::to_string(settings.hFlip) +
                " vflip=" + std::to_string(settings.vFlip) +
                " camera-number=" + std::to_string(settings.camera) +
                " ! video/x-h264, width=" + std::to_string(settings.width) +
                ", height=" + std::to_string(settings.height) + ", framerate=" + std::to_string(settings.fps) +
                "/1 ! tee name=t ! queue ! h264parse ! avimux ! filesink location=" + fileName + 
                ".avi t. ! queue ! h264parse ! rtph264pay config-interval=1 pt=96 ! " +
                "udpsink host=" + settings.addr.ip + " port=" + settings.addr.port + " sync=false";
        }
        else
        {
            pipelineCommand = "rpicamsrc bitrate=" + std::to_string(settings.bitrate) +
                " preview=0 hflip=" + std::to_string(settings.hFlip) +
                " vflip=" + std::to_string(settings.vFlip) +
                " camera-number=" + std::to_string(settings.camera) +
                " ! video/x-h264, width=" + std::to_string(settings.width) +
                ", height=" + std::to_string(settings.height) + ", framerate=" + std::to_string(settings.fps) +
                "/1 ! queue ! h264parse ! avimux ! filesink location=" + fileName + ".avi";
        }
    }
    else
    {
        pipelineCommand = "rpicamsrc bitrate=" + std::to_string(settings.bitrate) +
            " preview=0 hflip=" + std::to_string(settings.hFlip) +
            " vflip=" + std::to_string(settings.vFlip) +
            " camera-number=" + std::to_string(settings.camera) +
            " ! video/x-h264, width=" + std::to_string(settings.width) +
            ", height=" + std::to_string(settings.height) + ", framerate=" + std::to_string(settings.fps) +
            "/1 ! h264parse ! queue ! rtph264pay config-interval=1 pt=96 ! " +
            "udpsink host=" + settings.addr.ip + " port=" + settings.addr.port + " sync=false";
    }
    
    std::cout << "Starting pipeline with command:\n" << pipelineCommand << std::endl;

    pipeline = gst_parse_launch(pipelineCommand.c_str(), &error);
    if (error) {
        gchar *message = g_strdup_printf("Unable to build pipeline: %s", error->message);
        g_clear_error (&error);
        g_free (message);
        return;
    }
    
    bus = gst_element_get_bus (pipeline);

    /* Start playing */
    ret = gst_element_set_state (pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        g_printerr ("Unable to set the pipeline to the playing state.\n");
        gst_object_unref (pipeline);
        return;
    } else if (ret == GST_STATE_CHANGE_NO_PREROLL) {
        data.is_live = true;
    }

    main_loop = g_main_loop_new (NULL, false);
    data.loop = main_loop;
    data.pipeline = pipeline;

    gst_bus_add_signal_watch (bus);
    g_signal_connect (bus, "message", G_CALLBACK (callbackMessage), &data);

    g_main_loop_run (main_loop);

    /* Free resources */
    g_main_loop_unref (main_loop);
    gst_object_unref (bus);
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (pipeline);
}