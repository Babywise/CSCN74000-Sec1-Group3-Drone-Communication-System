#include "Server.h"

#include <thread>
#include <iostream>
#include "ChatServer.h"

int clientService(Server& server, SOCKET& clientSocket);

#define TOWER_ID "AA001"

int main(void) {

    // Main server loop to accept connections and handle them
    while ( true ) {

        Server server(TOWER_ID, 12345);
		server.listenforConnection();
		std::cout << "Listening for connections...\n";

		std::vector<std::thread> threads;

		server.acceptConnection();

        std::string command;

        while ( command != "1" && command != "2" ) {
            std::cout << "\033[2J\033[1;1H";
            std::cout << "Client connected\n";
            std::cout << "1. Accept\n";
            std::cout << "2. Reject\n";

            std::cin >> command;
            int choice = std::stoi(command);
            if ( choice == 2 ) {
                server.closeLastConnection();
                continue;
            } else if ( choice == 1 ) {
                //threads.push_back(std::thread(clientService, server, server.getClientSockets().back()));
                threads.push_back(std::thread([&]() { clientService(server, server.getClientSockets().back()); }));
                threads.back().join();
                std::cout << "Thread created\n";
            }
        }

		server.shutdownServer();
    }

    return 0;
}

int clientService(Server& server, SOCKET& clientSocket ) {
    // create a terminal window

    //get and set drone id
    //->
    server.setTowerID(TOWER_ID);
    while ( true ) {
        // clear screen
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Tower ID: " << server.getTowerID() << "\n";
        std::cout << "Connected: " << server.getDroneID() << "\n";
        std::cout << "Waiting for command...\n";
        std::cout << "1. Open Chat\n";
        std::cout << "2. Take Picture\n";
        std::cout << "3. Disconnect\n";

        std::string command;
        std::cin >> command;

        int choice = std::stoi(command);

        switch ( choice ) {
        case 1:
            openChat(server, clientSocket);
            break;
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
    }
    return 1;
}