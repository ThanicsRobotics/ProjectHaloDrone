#ifndef PTPCONTROLLER_H
#define PTPCONTROLLER_H

#include <inttypes.h>
#include <ptpcam.h>
#include <array>

class PTPController {
public:
    // struct PTP_USB {
    //     usb_dev_handle* handle;
    //     int inep;
    //     int outep;
    //     int intep;
    // };

    int getProperty(uint16_t propertyCode);
    int setProperty(uint16_t propertyCode, char *value);
    int genericRequest(uint16_t requestCode, std::array<uint32_t, 5> params);
    int captureImage();

private:
    int busn = 0, devn = 0;
	int action=0;
	short force=0;
	int overwrite=SKIP_IF_EXISTS;
	uint16_t property=0;
	char* value=NULL;
	char* propstr=NULL;
	uint32_t handle=0;
	char *filename=NULL;
	int num=0;
	int interval=0;
	uint16_t reqCode=0;
	uint32_t reqParams[5];
	uint32_t direction=PTP_DP_GETDATA;
	char data_file[256];













    // short verbose = 0;

    // void reset_device (int busn, int devn, short force);
    // int usb_ptp_device_reset(PTP_USB* ptp_usb);
    // int usb_ptp_get_device_status(PTP_USB* ptp_usb, uint16_t* devstatus);

    // void show_info (int busn, int devn, short force);
    // void list_files (int busn, int devn, short force);
    // void get_file (int busn, int devn, short force, uint32_t handle, char* filename, int overwrite);
    // void get_all_files (int busn, int devn, short force, int overwrite);
    // void capture_image (int busn, int devn, short force);
    // void delete_object (int busn, int devn, short force, uint32_t handle);
    // void delete_all_files (int busn, int devn, short force);
    // void loop_capture (int busn, int devn, short force, int n, int interval, int overwrite);
    // void save_object(PTPParams *params, uint32_t handle, char* filename, PTPObjectInfo oi, int overwrite);
    // void get_save_object (PTPParams *params, uint32_t handle, char* filename, int overwrite);
    // void send_generic_request (int busn, int devn, uint16_t reqCode, uint32_t *params, uint32_t direction, char *data_file);

    // struct usb_bus* init_usb(void);
    // void close_usb(PTP_USB* ptp_usb, struct usb_device* dev);

    // void init_ptp_usb (PTPParams*, PTP_USB*, struct usb_device*);
    // static short ptp_read_func (unsigned char *bytes, unsigned int size, void *data);
    // static short ptp_write_func (unsigned char *bytes, unsigned int size, void *data);
    // static short ptp_check_int (unsigned char *bytes, unsigned int size, void *data);

    // void clear_stall(PTP_USB* ptp_usb);

    // int usb_get_endpoint_status(PTP_USB* ptp_usb, int ep, uint16_t* status);
    // int usb_clear_stall_feature(PTP_USB* ptp_usb, int ep);
    // int open_camera (int busn, int devn, short force, PTP_USB *ptp_usb, PTPParams *params, struct usb_device **dev);
    // void close_camera (PTP_USB *ptp_usb, PTPParams *params, struct usb_device *dev);

    // void ptpcam_debug (void *data, const char *format, va_list args);
};

#endif