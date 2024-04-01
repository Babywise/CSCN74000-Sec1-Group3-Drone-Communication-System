#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"
#include "../DCS Class Library/Logger.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>


class Client {
    std::string droneID = {};
    std::string towerID = {};
    std::string currMessage = {};
    WSADATA wsaData = {};
    SOCKET clientSocket = {};
    sockaddr_in serverAddress = {};

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
    std::string getCurrMessage();
    void setCurrMessage(std::string message);
    void clearCurrMessage();

    // Timeouts
    bool setTimeout(int duration);

};