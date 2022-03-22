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

void serverRecieve(sf::TcpSocket socket) {
    while (true) {
        // Receive a message back from the client
        char in[1024];
        std::size_t received;
        if (socket.receive(in, sizeof(in), received) != sf::Socket::Done) {
            std::cout << "Error!" << std::endl;
            return;
        }
        std::cout << in << std::endl;
    }
}

void serverSend(sf::TcpSocket socket) {
    while (true) {
        // Receive a message back from the client
        char out[1024];
        std::cin >> out;
        std::size_t received;
        if (socket.send(out, sizeof(out)) != sf::Socket::Done) {
            std::cout << "Error!" << std::endl;
            return;
        }
        std::cout << "Message sent to the client: \"" << out << "\"" << std::endl;
    }
}

void runTcpServer(unsigned short port)
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

    std::thread threadObj(serverSend, socket);
    std::thread threadObj2(serverRecieve, socket);
    threadObj2.join();
    threadObj.join();
}

void clientRecieve(sf::TcpSocket socket) {
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

void clientSend(sf::TcpSocket socket) {
    while (true) {
        // Send an answer to the server
        char out[1024];
        std::cin >> out;
        if (socket.send(out, sizeof(out)) != sf::Socket::Done) {
            std::cout << "Error!" << std::endl;
            return;
        }
        std::cout << "Message sent to the server: \"" << out << "\"" << std::endl;
    }
}

void runTcpClient(unsigned short port)
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

    std::thread threadObj(clientRecieve, socket);
    std::thread threadObj2(clientSend, socket);
    threadObj2.join();
    threadObj.join();
}

int main()
{

    // Client or server ?
    char who;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin >> who;

    int port = 49699;

    /*std::thread threadObj(test1);
    std::thread threadObj2(test2);
    threadObj2.join();
    threadObj.join();
    */

    // Test the TCP protocol
    if (who == 's')
        runTcpServer(port);
    else
        runTcpClient(port);

    std::cin >> port;

    return 0;
}