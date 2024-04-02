#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"
#include "../DCS Class Library/Logger.h"
#include <winsock2.h>
#include <iostream>
#include <vector>


class Server {
    std::string towerID = {};
    std::string droneID = {};
    WSADATA wsaData = {};
    SOCKET serverSocket = {};
    sockaddr_in serverAddress = {};
    vector<SOCKET> clientSockets = {};
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
    vector<SOCKET>& getClientSockets();

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