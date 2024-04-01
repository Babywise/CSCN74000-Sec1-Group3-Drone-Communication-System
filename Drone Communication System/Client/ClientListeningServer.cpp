#include "ClientListeningServer.h"

//Initialize the server with the tower ID and port and create/bind a socket
Server::Server(std::string towerID, int port)
{
    this->towerID = towerID;

    // Initialize Winsock
    if ( WSAStartup(MAKEWORD(2, 2), &wsaData) != 0 ) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( serverSocket == INVALID_SOCKET ) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << '\n';
        WSACleanup();
        return;
    }

    // Bind the ip address and port to a socket
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // Bind the server socket to the specified IP and port
    if ( bind(serverSocket, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR ) {
        std::cerr << "Bind failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
}

//Listen for incoming connections on the server socket
bool Server::listenforConnection()
{
    if ( listen(serverSocket, SOMAXCONN) == SOCKET_ERROR ) {
        std::cerr << "Listen failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return false;
    } else {
        return true;
    }
}

//Accept a connection from a client and add the client socket to the queue
bool Server::acceptConnection()
{
    // Accept client connections and add to queue
    clientSockets.push_back(accept(serverSocket, nullptr, nullptr));
    if ( clientSockets.back() == INVALID_SOCKET ) {
        std::cerr << "Accept failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return false;
    } else {
        return true;
    }
}

//Close the last connection in the queue
bool Server::closeLastConnection()
{
    if ( closesocket(clientSockets.back()) != SOCKET_ERROR ) {
        return true;
    } else {
        std::cout << "Thread:" << WSAGetLastError() << '\n';
        return false;
    }
}

//Close the server socket
bool Server::shutdownServer()
{
    // Close server socket once server is done
    if ( closesocket(serverSocket) != SOCKET_ERROR ) {
        WSACleanup();
        return true;
    } else {
        std::cout << WSAGetLastError() << '\n';
        return false;
    }
}

//Get the server socket
SOCKET Server::getServerSocket()
{
    return serverSocket;
}

//Get the client sockets
vector<SOCKET>& Server::getClientSockets()
{
    return clientSockets;
}

//Get the tower ID
std::string Server::getTowerID()
{
    return towerID;
}

//Set the tower ID
void Server::setTowerID(std::string towerID)
{
    this->towerID = towerID;
}

//Get the drone ID
std::string Server::getDroneID()
{
    return droneID;
}

//Set the drone ID
void Server::setDroneID(std::string droneID)
{
    this->droneID = droneID;
}

//Send a packet to a client
int Server::sendPacket(Packet& packet, SOCKET& clientSocket)
{
    int sendResult = send(clientSocket, packet.serialize(), maxPacketSize, 0);

    return sendResult;
}

//Get the current message
std::string Server::getCurrMessage()
{
    return currMessage;
}

//Set the current message
void Server::setCurrMessage(std::string message)
{
    this->currMessage = message;
}

//Clear the current message
void Server::clearCurrMessage()
{
    this->currMessage.erase();
}

//Set a timeout for a client socket
bool Server::setTimeout(SOCKET& clientSocket, int duration)
{
    if ( setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&duration, sizeof(duration)) == SOCKET_ERROR ) {
        return false;
    }
    return true;
}
