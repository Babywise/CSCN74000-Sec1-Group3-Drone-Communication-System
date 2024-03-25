#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"
#include "ClientListeningServer.h"
#include "ClientMenus.h"

#include <iostream>
#include <string>

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

void clientService(Client& client, Client& chatClient) {

    while ( true ) {

        clientDroneMenu(client.getDroneID(), client.getTowerID());

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