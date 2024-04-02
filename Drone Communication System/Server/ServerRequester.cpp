/*
* Project: Next Level Drone Systems
* Module: server
* Language: C++
*
* File: ServerRequester.cpp
*
* Description: Runs server code on the server for tx communication with the client
* during the chat window
*
* Authors : Islam Ahmed
*/

#include "ServerRequester.h"

Client::Client(std::string droneID) : droneID(droneID), towerID("replace_me Twr ID"), currMessage(),currDate(), wsaData(), clientSocket(), serverAddress()
{
    // Initialize Winsock
    if ( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 ) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( clientSocket == INVALID_SOCKET ) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << '\n';
        WSACleanup();
        return;
    }

}

bool Client::connectToServer(const std::string& ipAddress, int port)
{
    //Connect socket to specified server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if ( inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr) <= 0 ) {
        return false;
    }

    if ( (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress))) == SOCKET_ERROR ) {
        closesocket(clientSocket);
        WSACleanup();
        return false;
    } else {
        return true;
    }
}

bool Client::closeConnection()
{
    // Close server socket once server is done
    if ( closesocket(clientSocket) != SOCKET_ERROR ) {
        WSACleanup();
        return true;
    } else {
        return false;
    }
}

int Client::sendPacket(Packet& packet)
{
    packet.setSource(1);
    int sendResult = send(clientSocket, packet.serialize(), maxPacketSize, 0);

    return sendResult;
}

SOCKET Client::getClientSocket()
{
    return clientSocket;
}

std::string Client::getDroneID()
{
    return droneID;
}

std::string Client::getTowerID()
{
    return towerID;
}

std::string Client::getCurrMessage()
{
    return currMessage;
}

void Client::setCurrMessage(std::string message)
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    std::string d = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
    this->currMessage = message;
    this->currDate = d;
}

void Client::clearCurrMessage()
{
    this->currMessage.erase();
}

bool Client::setTimeout(int duration)
{
    if ( setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&duration, sizeof(duration)) == SOCKET_ERROR ) {
        return false;
    }
    return true;
}

void Client::setCurrDate(std::string date)
{
	this->currDate = date;
}