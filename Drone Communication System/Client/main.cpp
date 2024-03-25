#include "Packet.h"
#include "MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"
#include "ClientListeningServer.h"

#include <iostream>
#include <string>

#define DRONE_ID "D001"

void clientService(Client& client, Client& chatClient);

int main(void) {
	// Create a client object
	Client client(DRONE_ID);
	Client chatClient(DRONE_ID);
    /*Server server(DRONE_ID, 12345);

    if ( !server.listenforConnection() ) {
        std::cout << "Server Listening Failed.\n";
        break;
    }*/


    std::string command;

    while ( true ) {
        std::system("cls");
        std::cout << "Listening for connections...\n";
        std::cout << "Welcome to Next Level Drone Systems\n";
        std::cout << "1. Connect\n";
        std::cout << "2. Check Connections\n";
        std::cout << "3. Exit\n";

        std::cin >> command;
        int choice = std::stoi(command);
        if ( choice == 1 ) {
            // Connect to the server
            std::cout << "Waiting...\n";
            if ( !client.connectToServer("127.0.0.1", 12345) ) {
                std::cout << "Server Connection Failed.\n";
                break;
            }

            if ( !chatClient.connectToServer("127.0.0.1", 10000) ) {
                std::cout << "Chat Server Connection Failed.\n";
                break;
            }

            clientService(client, chatClient);

        } else if ( choice == 2 ) {
			break;
        } else {
			std::cout << "Invalid Option.\n";
		}
    }

	client.closeConnection();

	return 0;
}

void clientService(Client& client, Client& chatClient) {

    while ( true ) {
        // clear screen
        std::system("cls");
        std::cout << "Drone ID: " << client.getDroneID() << "\n";
        std::cout << "Connected: " << client.getTowerID() << "\n";
        std::cout << "Waiting for command...\n";
        std::cout << "1. Open Chat\n";
        std::cout << "2. Take & Send Picture\n";
        std::cout << "3. Disconnect\n";

        std::string command;
        std::cin >> command;
        int choice = std::stoi(command);


        switch ( choice ) {
        case 1:
            runChatWindow(chatClient);
            break;
        case 2:
            std::cout << "Piccc.\n";
            break;
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
    }
}