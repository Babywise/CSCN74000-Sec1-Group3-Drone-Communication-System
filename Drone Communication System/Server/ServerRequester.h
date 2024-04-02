/*
* Project: Next Level Drone Systems
* Module: Server
* Language: C++
*
* File: ServerRequester.h
*
* Description: 
*
* Authors : Islam Ahmed
*/
#pragma once
#pragma comment(lib, "ws2_32.lib")

// Local Libraries
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"
#include "../DCS Class Library/Logger.h"
// End Local Libraries

// Standard Libraries
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
// End Standard Libraries

/*
* Class for the client to send and recieve messages
*/
class Client {
    // Variables
    std::string droneID = {};
    std::string towerID = {};
    std::string currMessage = {};
    std::string currDate = {};
    WSADATA wsaData = {};
    SOCKET clientSocket = {};
    sockaddr_in serverAddress = {};
    // End Variables

public:
    Client(std::string droneID);
    // Connections
    bool connectToServer(const std::string& ipAddress, int port);
    bool closeConnection();

    // Communication
    int sendPacket(Packet& packet);

    // Sockets
    SOCKET getClientSocket();

    // Information
    std::string getDroneID();
    std::string getTowerID();

    // Message
    void setCurrDate(std::string date);
    std::string getCurrMessage();
    void setCurrMessage(std::string message);
    void clearCurrMessage();

    // Timeouts
    bool setTimeout(int duration);

};