#include "Server.h"

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
    serverAddress.sin_port = htons(port); // port 12345

    // Bind the server socket to the specified IP and port
    if ( bind(serverSocket, reinterpret_cast<SOCKADDR*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR ) {
        std::cerr << "Bind failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
}

bool Server::listenforConnection()
{
    if ( listen(serverSocket, SOMAXCONN) == SOCKET_ERROR ) {
        std::cerr << "Listen failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
        return 0;
    }
}

bool Server::acceptConnection()
{
    // Accept client connections and add to queue
    clientSockets.push_back(accept(serverSocket, nullptr, nullptr));
    if ( clientSockets.back() == INVALID_SOCKET ) {
        std::cerr << "Accept failed: " << WSAGetLastError() << '\n';
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    } else {
		return 0;
	}
}

bool Server::closeLastConnection()
{
    closesocket(clientSockets.back());
    return true;
}

bool Server::shutdownServer()
{
    // Close server socket once server is done
    if ( closesocket(serverSocket) != SOCKET_ERROR ) {
        WSACleanup();
        return true;
    } else {
		return false;
	}
}

SOCKET Server::getServerSocket()
{
    return serverSocket;
}

vector<SOCKET>& Server::getClientSockets()
{
    return clientSockets;
}

std::string Server::getTowerID()
{
    return towerID;
}

void Server::setTowerID(std::string towerID)
{
    this->towerID = towerID;
}

std::string Server::getDroneID()
{
    return droneID;
}

bool Server::receivePacket(Packet& packet, SOCKET& clientSocket)
{
    char buffer[maxPacketSize];
    int bytesReceived = recv(clientSocket, buffer, maxPacketSize, 0);

    if ( bytesReceived > 0 ) {
        PacketManager packetManager(buffer);
        MessagePacket* msgPacket;

        switch ( packetManager.getPacketType() )
        {
        case PacketType::packetMessage:
            msgPacket = new MessagePacket(buffer);
            std::cout << msgPacket->getMessage() << '\n';
            return true;
            break;
        default:
            std::cout << "Invalid Packet Type.\n";
            break;
        }
    }
    return false;
}

int Server::sendPacket(Packet& packet, SOCKET& clientSocket)
{
    int sendResult = send(clientSocket, packet.serialize(), maxPacketSize, 0);

    return sendResult;
}


