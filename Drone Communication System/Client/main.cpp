#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"
#include "ClientListeningServer.h"
#include "ClientMenus.h"

#include <iostream>
#include <string>
#include <fstream>
#include <queue>

#define DRONE_ID "D001"
#define CLIENT_PORT 12345
#define CHAT_PORT 10000
#define LISTEN_PORT 20000

void clientService(Client& client, Client& chatClient);
void checkConnectionsFromServer(Client& client, Client& chatClient, Server& server);

int main(void) {

    while ( true ) {

        std::system("cls");
        // Create a client object
        Client client(DRONE_ID);
        Client chatClient(DRONE_ID);
        Server server(DRONE_ID, LISTEN_PORT);

        if ( !server.listenforConnection() ) {
            std::cout << "Server Listening Failed.\n";
            break;
        }

        std::cout << "Listening for connections...\n";

        std::string command;

        while ( command != "1" && command != "2" && command != "3" ) {

            clientStartMenu(DRONE_ID);

            std::cin >> command;
            int choice = std::stoi(command);
            if ( choice == 1 ) {
                // Connect to the server
                std::cout << "Waiting...\n";
                if ( !client.connectToServer("127.0.0.1", CLIENT_PORT) ) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                if ( !chatClient.connectToServer("127.0.0.1", CHAT_PORT) ) {
                    std::cout << "Chat Server Connection Failed.\n";
                    break;
                }
                clientService(client, chatClient);

            } else if ( choice == 2 ) {
                checkConnectionsFromServer(client, chatClient, server);
            } else {
                std::cout << "Invalid Option.\n";
            }
        }

        client.closeConnection();
        chatClient.closeConnection();
        server.shutdownServer();
    }
	return 0;
}

void mainLoop() {

}


bool sendImage(Client& chatClient) {

    
    std::queue<std::string> imageQueue;
    std::string filename;
    while (true) {
        std::cout << "Enter the name of the image file to send (or 'done' to finish): ";
        std::cin >> filename;
        if (filename == "done") {
            break;
        }
        imageQueue.push(filename);
    }

    while (!imageQueue.empty()) {
        std::string filename = imageQueue.front();
        imageQueue.pop();
        std::string filepath = "./images/" + filename;
        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open image file\n";
            continue;
        }

        std::string imageData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        int imageSize = imageData.size();
        std::cout << "Sending image size: " << imageSize << " bytes\n";
        send(chatClient.getClientSocket(), reinterpret_cast<const char*>(&imageSize), sizeof(imageSize), 0);

        int totalBytesSent = 0;
        int packetSize = 1024;
        while (totalBytesSent < imageSize) {
            int bytesToSend = std::min<int>(packetSize, imageSize - totalBytesSent);
            int bytesSent = send(chatClient.getClientSocket(), imageData.data() + totalBytesSent, bytesToSend, 0);
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Error sending data: " << WSAGetLastError() << '\n';
                file.close();
                break;
            }
            totalBytesSent += bytesSent;
            std::cout << "Sent " << bytesSent << " bytes (Total: " << totalBytesSent << "/" << imageSize << ")\n";
        }

        std::cout << "Image sent successfully\n";
        file.close();

        // Wait for acknowledgment
        char ack[256];
        int bytesReceived = recv(chatClient.getClientSocket(), ack, sizeof(ack), 0);
        if (bytesReceived > 0) {
            ack[bytesReceived] = '\0';
            std::cout << "Server acknowledgment: " << ack << '\n';
        }

        Sleep(1000); // Pause before sending the next image
    }

    // Signal end of transmission
    int endSignal = -1; // Use -1 to indicate the end of image transmission
    send(chatClient.getClientSocket(), reinterpret_cast<const char*>(&endSignal), sizeof(endSignal), 0);

    system("pause");
    return true;
    
}


void clientService(Client& client, Client& chatClient) {

    while ( true ) {

        clientDroneMenu(client.getDroneID(), client.getTowerID());

        std::string command;
        std::cin >> command;
        int choice = std::stoi(command);

        bool imageStatus = false;


        switch ( choice ) {
        case 1:
            runChatWindow(chatClient);
            break;
        case 2:
            std::cout << "Piccc.\n";
            imageStatus = sendImage(chatClient);
            std::cout << "ImageStatus: " << imageStatus << std::endl;
            break;
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
    }
}

void checkConnectionsFromServer(Client& client, Client& chatClient, Server& server) {

    std::string command;

    while ( command != "1" && command != "2" ) {

        clientConnectionMenu();

        std::cin >> command;
        int choice = std::stoi(command);
        if ( choice == 2 ) {
            if ( !server.closeLastConnection() ) {
                std::cout << "Closing Server Connection Failed.\n";
                break;
            }
            continue;
        } else if ( choice == 1 ) {
            if ( !server.acceptConnection() ) {
                std::cout << "Accepting Server Connection Failed.\n";
                break;
            } else {
                std::cout << "Server Connection (SUCCESS).\n";
                std::string message = DRONE_ID + string("| Accepted Your Connection");
                MessagePacket msgPacket;
                char messageToSend[MAX_MESSAGE_SIZE] = {};
                strcpy_s(messageToSend, message.c_str());
                msgPacket.setMessage(messageToSend);

                PacketManager pM(msgPacket.serialize());
                Packet* packet = pM.getPacket();

                int response = send(server.getClientSockets().back(), packet->serialize(), maxPacketSize, 0);

                if ( !client.connectToServer("127.0.0.1", CLIENT_PORT) ) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                if ( !chatClient.connectToServer("127.0.0.1", CHAT_PORT) ) {
                    std::cout << "Chat Server Connection Failed.\n";
                    break;
                }

                clientService(client, chatClient);
            }
        }
    }

}