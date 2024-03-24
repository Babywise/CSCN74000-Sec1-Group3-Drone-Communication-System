#include "ChatServer.h"

void openChat(Server& server, SOCKET& clientSocket) {
    // Open Chat
    std::string messageToSend;
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Chat opened\n";
    std::cout << "Type 'exit' to close chat\n\n";
    while ( messageToSend.compare("exit") != 0 ) {
        recieveChatMessage(server, clientSocket);
        system("pause");
        std::cout << server.getTowerID() << "|( You )" << ": ";
        std::cin >> messageToSend;

        sendChatMessage(server, clientSocket, messageToSend);
    }
    
}

bool recieveChatMessage(Server& server, SOCKET& clientSocket) {

    char RxBuffer[maxPacketSize] = {};

    int bytesReceived = recv(clientSocket, RxBuffer, maxPacketSize, 0);

    if ( bytesReceived > 0) {

        PacketManager pM(RxBuffer);
        if ( pM.getPacketType() == PacketType::packetMessage ) {
            MessagePacket* msgPacket= new MessagePacket(RxBuffer);
            std::string message = msgPacket->getMessage();
            std::cout << server.getDroneID() << "|(Client): " << message << "\n";
            return true;
        }
        return false;
    } else {
        return false;
    }
    return true;
}

bool sendChatMessage(Server& server, SOCKET& clientSocket, std::string message) {

    MessagePacket msgPacket;
    msgPacket.setMessage(message);

    PacketManager pM(msgPacket.serialize());
    Packet* packet = pM.getPacket();

    if ( server.sendPacket(*packet, clientSocket) > 0 ) {
        return true;
    } else {
        return false;
    }
}