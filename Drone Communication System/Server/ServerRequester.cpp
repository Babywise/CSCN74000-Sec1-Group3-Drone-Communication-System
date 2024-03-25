#include "ServerRequester.h"

Client::Client(std::string droneID) : droneID(droneID), towerID("replace_me Twr ID"), currMessage(), wsaData(), clientSocket(), serverAddress()
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

int Client::sendPacket(Packet& packet)
{
    int sendResult = send(clientSocket, packet.serialize(), maxPacketSize, 0);

    return sendResult;
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
    this->currMessage = message;
}

void Client::clearCurrMessage()
{
    this->currMessage.erase();
}
