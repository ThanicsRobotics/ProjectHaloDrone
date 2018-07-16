#include <stream.h>

#include <array>
#include <ctime>

#define MAXDATASIZE 500 // max number of bytes we can get at once 

using boost::asio::ip::tcp;

Stream::Stream()
    : socket(io), acceptor(io, tcp::endpoint(tcp::v4(), 13)), resolver(io)
{
    
}

void Stream::startClient()
{
    try
    {
        tcp::resolver::results_type endpoints = resolver.resolve("192.168.1.24", "5000");
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected" << std::endl;

        std::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
        {
            std::cout << "Server closed connection" << std::endl;
            // break;
        }
        else if (error)
            throw boost::system::system_error(error);

        std::cout.write(buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Stream::write(std::string& msg)
{
    try
    {
        boost::system::error_code error;
        std::cout << "Writing: " << msg << std::endl;
        boost::asio::write(socket, boost::asio::buffer(msg), error);
        if (error == boost::asio::error::eof)
        {
            std::cout << "Server closed connection" << std::endl;
        }
        else if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Stream::read(std::function<void(std::size_t)> callback)
{
    try
    {
        socket.async_read_some(boost::asio::buffer(currentMessage), 
        [callback](boost::system::error_code ec, std::size_t size)
        {
            if (!ec)
            {
                callback(size);
            }
            else
                throw boost::system::system_error(ec);
        });
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}