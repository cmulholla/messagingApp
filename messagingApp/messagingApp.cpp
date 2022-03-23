// messagingApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <thread>
#include <chrono>

void recieve(sf::TcpSocket& socket) { // basic function to recieve c-strings from a socket. Used both by the server and client.
    while (true) {
        // Receive a message from the server
        char in[1024];
        std::size_t received;
        if (socket.receive(in, sizeof(in), received) != sf::Socket::Done) {
            std::cout << "Error!" << std::endl;
            return;
        }
        std::cout << in << std::endl;
    }
}

void send(sf::TcpSocket& socket) { // basic function to send strings to a socket. Used both by the server and client.
    while (true) {
        // Send an answer to the server
        std::string temp;
        char out[1024];
        getline(std::cin, temp);
        strcpy_s(out, temp.c_str()); //copy the string over to the c string
        if (socket.send(out, sizeof(out)) != sf::Socket::Done) {
            std::cout << "Error!" << std::endl;
            return;
        }
        std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
    }
}

void runTcpServer(unsigned short port) //listens for a connection from a client and starts threads.
{
    // Create a server socket to accept new connections
    sf::TcpListener listener;

    // Listen to the given port for incoming connections
    if (listener.listen(port) != sf::Socket::Done)
        return;
    std::cout << "Server is listening to port " << port << ", waiting for connections... " << std::endl;

    // Wait for a connection
    sf::TcpSocket socket;
    if (listener.accept(socket) != sf::Socket::Done)
        return;
    std::cout << "Client connected: " << socket.getRemoteAddress() << std::endl;

    std::thread clientRecieve(recieve, std::ref(socket));
    std::thread clientSend(send, std::ref(socket));
    clientRecieve.join();
    clientSend.join();
}

void runTcpClient(unsigned short port) //asks for an IP address and attempts to connect to it, then starts the threads.
{
    // Ask for the server address
    sf::IpAddress server;
    do
    {
        std::cout << "Type the address or name of the server to connect to: ";
        std::cin >> server;
    } while (server == sf::IpAddress::None);

    // Create a socket for communicating with the server
    sf::TcpSocket socket;

    // Connect to the server
    if (socket.connect(server, port) != sf::Socket::Done)
        return;
    std::cout << "Connected to server " << server << std::endl;

    std::thread clientRecieve(recieve, std::ref(socket));
    std::thread clientSend(send, std::ref(socket));
    clientRecieve.join();
    clientSend.join();
}

int main()
{

    // Client or server ?
    char who;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin >> who;

    int port = 49699;

    // Run the TCP protocol
    if (who == 's')
        runTcpServer(port);
    else
        runTcpClient(port);

    return 0;
}