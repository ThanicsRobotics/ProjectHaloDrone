#include <wlan.h>

using boost::asio::ip::tcp;

WLAN::WLAN()
    : socket(io), acceptor(io, tcp::endpoint(tcp::v4(), 13)), resolver(io)
{
    
}

void WLAN::startClient(std::string ipAddress, int port)
{
    try
    {
        tcp::resolver::results_type endpoints = resolver.resolve(ipAddress, std::to_string(port));
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected" << std::endl;

        std::array<char, MAX_BUFFER_SIZE> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
        {
            std::cout << "Server closed connection" << std::endl;
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

void WLAN::write(std::string& msg)
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

void WLAN::read()
{
    try
    {
        socket.async_read_some(boost::asio::buffer(currentMessage), 
        [this](boost::system::error_code ec, std::size_t size)
        {
            if (!ec)
            {
                receiveCallback(size);
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

void WLAN::setCallback(std::function<void(std::size_t)> callback)
{
    receiveCallback = callback;
    callbackSet = true;
}

void WLAN::checkBuffer()
{
    io.poll();
}