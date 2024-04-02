/*
* Project: Next Level Drone Systems
* Module: client
* Language: C++
*
* File: ClientListeningServer.h
*
* Description: Runs server code on the client to listen for client connections
*
* Authors : Islam Ahmed
*/
#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"
#include "../DCS Class Library/Logger.h"
#include <winsock2.h>
#include <iostream>
#include <vector>

/*
* Server class for listening and handling client connections
* @param towerID - the client drone id
* @param port - the port number to listen on
*/
class Server {
    // Variables
    std::string towerID = {};
    std::string droneID = {};
    WSADATA wsaData = {};
    SOCKET serverSocket = {};
    sockaddr_in serverAddress = {};
    std::vector<SOCKET> clientSockets = {};
    std::string currMessage = {};

public:
    Server(std::string towerID, int port);
    // Connections
    bool listenforConnection();
    bool acceptConnection();
    bool closeLastConnection();
    bool shutdownServer();

    // Sockets
    SOCKET getServerSocket();
    std::vector<SOCKET>& getClientSockets();

    // Information
    std::string getTowerID();
    void setTowerID(std::string);
    void setDroneID(std::string);
    std::string getDroneID();

    // Communication
    int sendPacket(Packet& packet, SOCKET& clientSocket);

    // Message
    std::string getCurrMessage();
    void setCurrMessage(std::string message);
    void clearCurrMessage();

    // Timeouts
    bool setTimeout(SOCKET& clientSocket, int duration);

};