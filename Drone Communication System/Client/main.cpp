#include "../Shared/Packet.h"
#include "../Shared/MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"
#include "ClientListeningServer.h"
#include "../Shared/menus.h"
#include <iostream>
#include <string>

#define DRONE_ID "D001"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CHAT_SERVER_PORT 10000


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
        drone_connect_menu();

        std::cin >> command;
        int choice = std::stoi(command);
        if ( choice == 1 ) {
            // Connect to the server
            std::cout << "Waiting...\n";
            if ( !client.connectToServer(SERVER_IP, SERVER_PORT) ) {
                std::cout << "Server Connection Failed.\n";
                break;
            }

            if ( !chatClient.connectToServer(SERVER_IP, CHAT_SERVER_PORT) ) {
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
        drone_main_menu(client.getDroneID(), client.getTowerID());

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