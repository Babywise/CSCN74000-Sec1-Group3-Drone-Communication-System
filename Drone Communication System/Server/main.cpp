#include "Server.h"
#include "ChatWindow.h"
#include "ServerRequester.h"
#include "ServerMenus.h"

#include <thread>
#include <iostream>

int clientService(Server& server, SOCKET& clientSocket, Server& chatServer, SOCKET& clientChatSocket);
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer);
void mainLoop(bool& connectionStatus, bool& listening);

#define TOWER_ID "AA001"
#define SERVER_PORT 12345
#define CHAT_PORT 10000
#define LISTEN_PORT 20000

int main(void) {

    bool connectionStatus = false;
    bool listening = false;
    std::thread main = std::thread([&]() { mainLoop(connectionStatus, listening); });
    while ( listening == false ) {}   // wait to start listening
    while ( true ) {
        Client client(TOWER_ID);

        std::string command;

        while ( command != "1" && command != "2" && command != "3" ) {

            serverStartMenu(TOWER_ID);

            std::cin >> command;
            int choice = std::stoi(command);
            if ( choice == 1 ) {
                // Connect to the server
                std::cout << "Waiting...\n";
                if ( !client.connectToServer("127.0.0.1", 20000) ) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }
                connectionStatus = true;
                main.join();

            } else if ( choice == 2 ) {
                connectionStatus = false;
                main.join();

            } else {
                std::cout << "Invalid Option.\n";
            }
        }

        client.closeConnection();
    }

    return 0;
}

void mainLoop(bool& connectionStatus, bool& listening) {
    // Main server loop to accept connections and handle them
    while ( true ) {

        std::system("cls");

        Server server(TOWER_ID, SERVER_PORT);
        Server chatServer(TOWER_ID, CHAT_PORT);

        if ( !server.listenforConnection() ) {
            std::cout << "Server Listening Failed.\n";
            break;
        }
        if ( !chatServer.listenforConnection() ) {
            std::cout << "Chat Server Listening Failed.\n";
            break;
        }

        std::cout << "Listening for connections...\n";

        std::vector<std::thread> threads;
        listening = true;

        if ( !server.acceptConnection() ) {
            std::cout << "Accepting Server Connection Failed.\n";
            break;
        }
        if ( !chatServer.acceptConnection() ) {
            std::cout << "Accepting Chat Server Connection Failed.\n";
            break;
        }

        connectionStatus = true;
        listening = false;
        while ( connectionStatus != false ) {
            printToCoordinates(6, 0, (char*)"Connection incoming... (10)");
        }

        checkConnectionsFromClient(threads, server, chatServer);

        server.shutdownServer();
        chatServer.shutdownServer();
    }
}

int clientService(Server& server, SOCKET& clientSocket, Server& chatServer, SOCKET& clientChatSocket) {

    //get and set drone id
    //->
    server.setDroneID("replace_me_droneID");
    while ( true ) {

        serverDroneMenu(server.getTowerID(), server.getDroneID());

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

void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer) {

    std::string command;

    while ( command != "1" && command != "2" ) {

        serverConnectionMenu();

        std::cin >> command;
        int choice = std::stoi(command);
        if ( choice == 2 ) {
            if ( !server.closeLastConnection() ) {
                std::cout << "Closing Server Connection Failed.\n";
                break;
            }
            if ( !chatServer.closeLastConnection() ) {
                std::cout << "Closing Chat Server Connection Failed.\n";
                break;
            }
            return;
        } else if ( choice == 1 ) {
            threads.push_back(std::thread([&]() { clientService(server, server.getClientSockets().back(), chatServer, chatServer.getClientSockets().back()); }));
            threads.back().join();
            std::cout << "Thread created\n";
        }
    }

}