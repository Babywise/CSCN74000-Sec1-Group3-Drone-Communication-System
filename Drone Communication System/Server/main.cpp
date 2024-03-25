#include "Server.h"

#include <thread>
#include <iostream>
//#include "ChatServer.h"
#include "ChatWindow.h"
#include "../Shared/menus.h"
int clientService(Server& server, SOCKET& clientSocket, Server& chatServer, SOCKET& clientChatSocket);
void main_loop(bool &connectionStatus, bool &listening);   
#define TOWER_ID "AA001"
#define RX_PORT 10000
#define TX_PORT 12345





int main(void) {
    bool connectionStatus = false;
    bool listening = false;
    std::thread main= std::thread([&]() { main_loop(connectionStatus,listening); });
    while(listening==false){}   // wait to start listening
    while (true) {
      // get input from user
        std::string command;
        std::system("cls");
        std::cout << "1. Connect" << std::endl;
        std::cout << "2. Check connections" << std::endl;
        std::cout << "3. Exit" << std::endl;
        printToCoordinates(LINE_COUNT + 3, 0, (char*)"Enter : ");
        std::cin >> command;
        if (command == "1") {
			// Connect to the client
		}
		else
        if (command == "2") {
            // check connections
			connectionStatus = false;
            main.join();
            main = std::thread([&]() { main_loop(connectionStatus, listening); });
		}
        else if (command == "3") {
			// exit
			break;
        }
	}
    

    return 0;
}

void main_loop(bool &connectionStatus,bool& listening) {
    // Main server loop to accept connections and handle them
    while (true) {

        std::system("cls");

        Server server(TOWER_ID, TX_PORT);
        Server chatServer(TOWER_ID, RX_PORT);

        if (!server.listenforConnection()) {
            std::cout << "Server Listening Failed.\n";
            break;
        }
        if (!chatServer.listenforConnection()) {
            std::cout << "Chat Server Listening Failed.\n";
            break;
        }

        std::cout << "Listening for connections...\n";
     
        std::vector<std::thread> threads;
        listening = true;
        if (!server.acceptConnection()) {
            std::cout << "Accepting Server Connection Failed.\n";
            break;
        }
        if (!chatServer.acceptConnection()) {
            std::cout << "Accepting Chat Server Connection Failed.\n";
            break;
        }

        connectionStatus = true;
        listening = false;
        while (connectionStatus != false) {
            printToCoordinates(LINE_COUNT + 3, 0, (char*)"Connection incoming");
        }
        std::string command;

        while (command != "1" && command != "2") {
            accept_reject_menu();
            std::cin >> command;
            int choice = std::stoi(command);
            if (choice == 2) {
                if (!server.closeLastConnection()) {
                    std::cout << "Closing Server Connection Failed.\n";
                    break;
                }
                if (!chatServer.closeLastConnection()) {
                    std::cout << "Closing Chat Server Connection Failed.\n";
                    break;
                }
                server.shutdownServer();
                chatServer.shutdownServer();
                return;
            }
            else if (choice == 1) {
                threads.push_back(std::thread([&]() { clientService(server, server.getClientSockets().back(), chatServer, chatServer.getClientSockets().back()); }));
                threads.back().join();
                std::cout << "Thread created\n";
            }
        }

        server.shutdownServer();
        chatServer.shutdownServer();
    }
}
int clientService(Server& server, SOCKET& clientSocket, Server& chatServer, SOCKET& clientChatSocket) {
    // create a terminal window

    //get and set drone id
    //->
    server.setDroneID("replace_me_droneID");
    while ( true ) {
        // clear screen
        server_main_menu(server.getTowerID(), server.getDroneID());
        std::string command;
        std::cin >> command;

        int choice = std::stoi(command);

        switch ( choice ) {
        case 1:
            runChatWindow(chatServer, clientChatSocket);
            break;
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
    }
    return 1;
}