#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/PacketManager.h"

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

public:
    Server(std::string towerID, int port);
    bool listenforConnection();
    bool acceptConnection();
    bool closeLastConnection();
    bool shutdownServer();

    SOCKET getServerSocket();
    vector<SOCKET>& getClientSockets();
    std::string getTowerID();
    void setTowerID(std::string);
    void setDroneID(std::string);
    std::string getDroneID();

    int sendPacket(Packet& packet, SOCKET& clientSocket);

    std::string getCurrMessage();
    void setCurrMessage(std::string message);
    void clearCurrMessage();
    bool setTimeout(SOCKET& clientSocket, int duration);

};