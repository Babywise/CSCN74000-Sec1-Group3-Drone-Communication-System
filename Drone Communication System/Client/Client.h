#pragma once
#pragma comment(lib, "ws2_32.lib")
#include "Packet.h"
#include "PacketManager.h"

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>


class Client {
    std::string droneID = {};
    std::string towerID = {};
    WSADATA wsaData = {};
    SOCKET clientSocket = {};
    sockaddr_in serverAddress = {};

public:
    Client(std::string droneID);
    bool connectToServer(const std::string& ipAddress, int port);
    int sendPacket(Packet& packet);
    bool closeConnection();


    SOCKET getClientSocket();
    std::string getDroneID();
    std::string getTowerID();

};