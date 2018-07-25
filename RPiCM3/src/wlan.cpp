#include <wlan.h>

using boost::asio::ip::tcp;

WLAN::WLAN()
    : socket(io), acceptor(io, tcp::endpoint(tcp::v4(), 13)), resolver(io)
{
    
}

WLAN::WLAN(DeviceType type, std::string ipAddress, int port)
    : socket(io), acceptor(io, tcp::endpoint(tcp::v4(), port)), resolver(io),
    hostname(ipAddress), port(port), deviceType(type)
{
    start(type, ipAddress, port);
}

void WLAN::start(DeviceType type, std::string ipAddress, int port)
{
    switch (type)
    {
        case DeviceType::CLIENT:
            startClient(ipAddress, port);
            break;
        case DeviceType::HOST:
            startHost();
            break;
        default:
            break;
    }
}

void WLAN::startHost()
{
    try
    {
        std::cout << "Waiting for client..." << std::endl;
        acceptor.accept(socket);
        connected = true;
        // Use async_accept here in the future

        std::cout << "Client connected: " << socket.remote_endpoint().address().to_string() << std::endl;
        std::string msg = "Hello from Controller\n";
        boost::system::error_code err;
        boost::asio::write(socket, boost::asio::buffer(msg), err);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void WLAN::startClient(std::string ipAddress, int port)
{
    std::cout << "Starting in client mode" << std::endl;
    try
    {
        tcp::resolver::results_type endpoints = resolver.resolve(ipAddress, std::to_string(port));
        if (endpoints.empty())
        {
            std::cout << "No hosts available" << std::endl;
            connected = false;
            return;
        }
        else
        {
            std::cout << "Available hosts:" << std::endl;
            for (auto endpoint : endpoints)
            {
                std::cout << endpoint.host_name() << ":" << endpoint.service_name() << std::endl;
            }
        }
        
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected" << std::endl;
        connected = true;

        std::array<uint8_t, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);
        if (error == boost::asio::error::eof)
        {
            std::cout << "Server closed connection" << std::endl;
        }
        else if (error)
            throw boost::system::system_error(error);

        std::cout.write((char*)buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void WLAN::write(std::array<uint8_t, PACKET_SIZE>& msg)
{
    if (!connected)
    {
        start(deviceType, hostname, port);
        return;
    }
    try
    {
        boost::system::error_code error;
        std::cout << "Writing: ";
        for (const auto& x : msg)
        {
            std::cout << x;
        }
        std::cout << std::endl;
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
    if (!connected)
    {
        start(deviceType, hostname, port);
        return;
    }
    try
    {
        socket.async_read_some(boost::asio::buffer(cachedMessage), 
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