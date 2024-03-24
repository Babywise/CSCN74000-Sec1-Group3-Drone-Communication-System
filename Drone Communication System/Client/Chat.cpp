#include "Chat.h"

void openChat(Client& client) {
    // Open Chat
    std::string messageToSend;
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Chat opened\n";
    std::cout << "Type 'exit' to close chat\n\n";
    while ( messageToSend.compare("exit") != 0 ) {
        std::cout << client.getDroneID() << "|( You )" << ": ";
        std::cin >> messageToSend;

        sendChatMessage(client, messageToSend);
        system("pause");
        recieveChatMessage(client);
    }
}

bool sendChatMessage(Client& client, std::string message) {

	MessagePacket msgPacket;
	msgPacket.setMessage(message);

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
    PacketManager pM(RxBuffer);

    if ( bytesReceived > 0 && pM.getPacketType() == PacketType::packetMessage ) {
        MessagePacket msgPacket(RxBuffer);
        std::string message = msgPacket.getMessage();
        std::cout << client.getTowerID() << "|(Server): " << message << "\n";
        return true;
    } else {
        return false;
    }

}