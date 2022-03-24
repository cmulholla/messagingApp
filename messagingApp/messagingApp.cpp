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
#include <vector>

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

////////////////// UDP ////////////////////////

std::vector<sf::IpAddress> addresses;

void usend(sf::UdpSocket& socket, unsigned short port) {
    while (true) {
        // Send a message to the server
        std::string temp;
        char out[1024];
        getline(std::cin, temp);
        strcpy_s(out, temp.c_str()); //copy the string over to the c string

        for (int i = 0; i < addresses.size(); i++) {
            if (socket.send(out, sizeof(out), addresses[i], port) != sf::Socket::Done)
                return;
            std::cout << "Message sent to " << addresses[i] << ": \"" << out << "\"" << std::endl;
        }
    }
}

void urecieve(sf::UdpSocket& socket, unsigned short port) {
    while (true) {
        // Receive an answer from anyone (but most likely from the server)
        char in[1024];
        std::size_t received;
        sf::IpAddress sender;
        unsigned short senderPort;
        if (socket.receive(in, sizeof(in), received, sender, senderPort) != sf::Socket::Done)
            return;
        std::cout << "Message received from " << sender << ": \"" << in << "\"" << std::endl;
        //check to see if sender is in addresses, and if not then add it.
        for (int i = 0; i < addresses.size(); i++) {
            if (addresses[i].toString() == sender.toString()) {
                break;
            }
            else if (i + 1 == addresses.size()) { //if at the end of the list and the address is not found
                addresses.push_back(sender); //add to list of people to send to
                std::cout << "address added to send list\n" << std::endl;
            }
        }
    }
}

void runUdpClient(unsigned short port) //Send a message to the server, wait for the answer
{
    // Ask for the server address
    sf::IpAddress server;
    do
    {
        std::cout << "Type the address or name of the server to connect to: ";
        std::cin >> server;
    } while (server == sf::IpAddress::None);

    addresses.push_back(server);

    // Create a socket for communicating with the server
    sf::UdpSocket socket;

    // Listen to messages on the specified port
    if (socket.bind(port) != sf::Socket::Done) {
        std::cout << "error!" << std::endl;
        std::cin >> port;
        return;
    }

    std::thread clientRecieve(urecieve, std::ref(socket), port);
    std::thread clientSend(usend, std::ref(socket), port);
    clientRecieve.join();
    clientSend.join();
}

void runUdpServer(unsigned short port) //Launch a server, wait for a message, send an answer.
{
    // Create a socket to receive a message from anyone
    sf::UdpSocket socket;

    // Listen to messages on the specified port
    if (socket.bind(port) != sf::Socket::Done)
        return;
    std::cout << "Server is listening to port " << port << std::endl;

    std::thread clientRecieve(urecieve, std::ref(socket), port);
    std::thread clientSend(usend, std::ref(socket), port);
    clientRecieve.join();
    clientSend.join();
}

int main()
{
    //two-way or multi-way?
    char com;
    std::cout << "one-to-one communication (1) or chat room (2)? ";
    std::cin >> com;


    // Client or server ?
    char who;
    std::cout << "Do you want to be a server (s) or a client (c)? ";
    std::cin >> who;

    int port = 49699;

    if (com == '1') {
        // Run the TCP protocol
        if (who == 's')
            runTcpServer(port);
        else
            runTcpClient(port);
    }
    else if (com == '2') {
        // Run the UDP protocol
        if (who == 's')
            runUdpServer(port);
        else
            runUdpClient(port);
    }

    return 0;
}

