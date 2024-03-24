#include "Packet.h"
#include "MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"

#include <iostream>
#include <string>

#define DRONE_ID "D001"

void clientService(Client& client, Client& chatClient);

int main(void) {
	// Create a client object
	Client client(DRONE_ID);
	Client chatClient(DRONE_ID);
    std::string command;

    while ( true ) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Welcome to Next Level Drone Systems\n";
        std::cout << "1. Connect\n";
        std::cout << "2. Exit\n";

        std::cin >> command;
        int choice = std::stoi(command);
        if ( choice == 1 ) {
            // Connect to the server
            std::cout << "Waiting...\n";
            if ( client.connectToServer("127.0.0.1", 12345) ) {
                //Get accepted message here
                if ( chatClient.connectToServer("127.0.0.1", 10000) ) {
                    //Get accepted message here

                    clientService(client, chatClient);
                }
                //clientService(client);
            }
        } else if ( choice == 2 ) {
			break;
		}
    }

	client.closeConnection();

	return 0;
}

void clientService(Client& client, Client& chatClient) {

    while ( true ) {
        // clear screen
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Drone ID: " << client.getDroneID() << "\n";
        std::cout << "Connected: " << client.getTowerID() << "\n";
        std::cout << "Waiting for command...\n";
        std::cout << "1. Open Chat\n";
        std::cout << "2. Take Picture\n";
        std::cout << "3. Disconnect\n";

        std::string command;
        std::cin >> command;
        int choice = std::stoi(command);


        switch ( choice ) {
        case 1:
            runChatWindow(client);
            //openChat(client, chatClient);
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