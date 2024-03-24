#include "Chat.h"
#include <string>

void openChat(Client& client) {
    // Open Chat
    char messageToSend[MAX_MESSAGE_SIZE];
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Chat opened\n";
    std::cout << "Type 'exit' to close chat\n\n";
    std::string exit = "exit";
    std::cin.getline(messageToSend, MAX_MESSAGE_SIZE);
    while ( strncmp(messageToSend, exit.c_str(), exit.length()) ) {
        
        std::cout << client.getDroneID() << "|( You )" << ": ";
        std::cin.getline(messageToSend, MAX_MESSAGE_SIZE);

        sendChatMessage(client, messageToSend);
        recieveChatMessage(client);
    }
}

bool sendChatMessage(Client& client, std::string message) {

	MessagePacket msgPacket;
    char messageToSend[MAX_MESSAGE_SIZE] = {};
    strcpy_s(messageToSend, message.c_str());
	msgPacket.setMessage(messageToSend);

    PacketManager pM(msgPacket.serialize());
    Packet* packet = pM.getPacket();

    if ( client.sendPacket(*packet) > 0 ) {
        return true;
    } else {
        return false;
    }
}

bool recieveChatMessage(Client& client) {

    char RxBuffer[maxPacketSize] = {};

    int bytesReceived = recv(client.getClientSocket(), RxBuffer, maxPacketSize, 0);

    if ( bytesReceived > 0 ) {

        PacketManager pM(RxBuffer);
        if ( pM.getPacketType() == PacketType::packetMessage ) {
            MessagePacket* msgPacket = new MessagePacket(RxBuffer);
            std::string message = msgPacket->getMessage();
            std::cout << client.getDroneID() << "|(Client): " << message << "\n";
            return true;
        }
        return false;
    } else {
        return false;
    }
    return true;

}