#include <stream.h>

#include <array>
#include <ctime>
#include <string>

#define MAXDATASIZE 500 // max number of bytes we can get at once 

using boost::asio::ip::tcp;

Stream::Stream() {
    
}

std::string daytimeString()
{
    std::time_t now = time(0);
    return std::ctime(&now);
}

void Stream::startClient()
{
    try
    {
        tcp::resolver resolver(io);
        tcp::resolver::results_type endpoints = resolver.resolve("raspberrypi", "daytime");
        tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);

        for (;;)
        {
            std::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            if (error == boost::asio::error::eof)
                break;
            else if (error)
                throw boost::system::system_error(error);

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Stream::startHost()
{
    try
    {
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
        for (;;)
        {
            tcp::socket socket(io);
            acceptor.accept(socket);

            std::string msg = daytimeString();
            boost::system::error_code err;
            boost::asio::write(socket, boost::asio::buffer(msg), err);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
