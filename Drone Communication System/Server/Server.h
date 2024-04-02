/*
* Project: Next Level Drone Systems
* Module: Server
* Language: C++
*
* File: server.h
*
* Description: Contains the server class for the server module
*
* Authors : Islam Ahmed
*/
#pragma once
#pragma comment(lib, "ws2_32.lib")

// Local Libraries
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"
#include "../DCS Class Library/Logger.h"


// Standard Libraries
#include <winsock2.h>
#include <iostream>
#include <vector>

/*
* Class for the server to send and recieve messages
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
    std::string currDate = {};

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
    std::string getCurrDate();
    std::string getCurrMessage();
    void setCurrMessage(std::string message);
    void clearCurrMessage();

    // Timeouts
    bool setTimeout(SOCKET& clientSocket, int duration);

};