//#include "ChatServer.h"
//
//void openChat(Server& server, Server& chatServer, SOCKET& clientSocket) {
//    // Open Chat
//    char messageToSend[MAX_MESSAGE_SIZE];
//    std::cout << "\033[2J\033[1;1H";
//    std::cout << "Chat opened\n";
//    std::cout << "Type 'exit' to close chat\n\n";
//    std::string exit = "exit";
//    std::cin.getline(messageToSend, MAX_MESSAGE_SIZE);
//    while ( strncmp(messageToSend, exit.c_str(), exit.length()) ) {
//
//        recieveChatMessage(server, clientSocket);
//        std::cout << server.getTowerID() << "|( You )" << ": ";
//        std::cin.getline(messageToSend, MAX_MESSAGE_SIZE);
//
//        sendChatMessage(server, clientSocket, messageToSend);
//    }
//    
//}
//
//bool recieveChatMessage(Server& server, SOCKET& clientSocket) {
//
//    char RxBuffer[maxPacketSize] = {};
//
//    int bytesReceived = recv(clientSocket, RxBuffer, maxPacketSize, 0);
//
//    if ( bytesReceived > 0) {
//
//        PacketManager pM(RxBuffer);
//        if ( pM.getPacketType() == PacketType::packetMessage ) {
//            MessagePacket* msgPacket= new MessagePacket(RxBuffer);
//            std::string message = msgPacket->getMessage();
//            std::cout << server.getDroneID() << "|(Client): " << message << "\n";
//            return true;
//        }
//        return false;
//    } else {
//        return false;
//    }
//    return true;
//}
//
//bool sendChatMessage(Server& server, SOCKET& clientSocket, std::string message) {
//
//    MessagePacket msgPacket;
//    char messageToSend[MAX_MESSAGE_SIZE] = {};
//    strcpy_s(messageToSend, message.c_str());
//    msgPacket.setMessage(messageToSend);
//
//    PacketManager pM(msgPacket.serialize());
//    Packet* packet = pM.getPacket();
//
//    if ( server.sendPacket(*packet, clientSocket) > 0 ) {
//        return true;
//    } else {
//        return false;
//    }
//}