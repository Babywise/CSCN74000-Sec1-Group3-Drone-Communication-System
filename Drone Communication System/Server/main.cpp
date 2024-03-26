#include "Server.h"
#include "ChatWindow.h"
#include "ServerRequester.h"
#include "ServerMenus.h"

#include <thread>
#include <iostream>
#include <fstream>

int clientService(Server& server, SOCKET& clientSocket, Server& chatServer, SOCKET& clientChatSocket);
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer);
void mainLoop(bool& connectionPending, bool& listening);

#define TOWER_ID "AA001"
#define SERVER_PORT 12345
#define CHAT_PORT 10000
#define LISTEN_PORT 20000

int main(void) {

    bool connectionPending = false;
    bool listening = false;
    std::thread main = std::thread([&]() { mainLoop(connectionPending, listening); });
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
                if ( !client.connectToServer("127.0.0.1", LISTEN_PORT) ) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                char RxBuffer[maxPacketSize] = {};
                if ( recv(client.getClientSocket(), RxBuffer, maxPacketSize, 0) <= 0 ) {
                    std::cout << "Response Lost.\n";
                    break;
                } 

                PacketManager pM(RxBuffer);
                if ( pM.getPacketType() == PacketType::packetMessage ) {
                    MessagePacket* msgPacket = new MessagePacket(RxBuffer);
                    client.setCurrMessage(msgPacket->getMessage());
                    std::cout << client.getCurrMessage() << "\n";
                }
                Sleep(2000);
                connectionPending = false;
                main.join();

            } else if ( choice == 2 ) {

                if ( connectionPending == true ) {
                    connectionPending = false;
                    //use sockets below 
                    // checkConnectionsFromClient(threads, server, chatServer);
                    main.join();
                    //Potentially needed
                    //main = std::thread([&]() { mainLoop(connectionPending, listening); });
                }

            } else {
                std::cout << "Invalid Option.\n";
            }
        }

        client.closeConnection();
    }

    return 0;
}

void mainLoop(bool& connectionPending, bool& listening) {
    // Main server loop to accept connections and handle them
    while (true) {

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

        connectionPending = true;
        listening = false;
        while ( connectionPending != false ) {
            printToCoordinates(6 /*replace with non magic number (screen position)*/, 0, (char*)"Connection incoming... (10)");
        }

        //Potentially move this to main
        checkConnectionsFromClient(threads, server, chatServer);

        server.shutdownServer();
        chatServer.shutdownServer();
    }
}

bool receiveImage(Server& chatServer, SOCKET& clientChatSocket) {
    while (true) {
        int imageSize;
        int recvResult = recv(clientChatSocket, reinterpret_cast<char*>(&imageSize), sizeof(imageSize), 0);
        if (recvResult <= 0 || imageSize == -1) {
            std::cout << "No more images to receive or connection closed by client.\n";
            break;
        }

        std::string filename = "./Images/received_image_" + std::to_string(time(nullptr)) + ".jpg";
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create file\n";
            break;
        }

        char buffer[1024];
        int totalBytesReceived = 0;
        while (totalBytesReceived < imageSize) {
            int bytesRead = recv(clientChatSocket, buffer, sizeof(buffer), 0);
            if (bytesRead == SOCKET_ERROR) {
                std::cerr << "Error receiving data: " << WSAGetLastError() << '\n';
                outFile.close();
                break;
            }
            outFile.write(buffer, bytesRead);
            totalBytesReceived += bytesRead;
        }

        std::cout << "Image received: " << filename << "\n";
        outFile.close();

        // Send acknowledgment back to client
        std::string ackMessage = "Image received successfully";
        send(clientChatSocket, ackMessage.c_str(), ackMessage.length(), 0);
    }
    system("pause");
    return true;
}

/*

@param: server: server object to handle server connections, chatServer: server object to handle chat communication, clientChatSocket: socket to handle chat communication
*/
int clientService(Server& server, Server& chatServer, SOCKET& clientChatSocket) {

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
        case 2:
            receiveImage(chatServer, clientChatSocket);
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
        
    }
    return 1;
}

/*
* This function is used to check on a connection request from a client and allow the user to accept or reject the connection
* Push a new thread to the vector of threads to handle the connection then join the thread if connection is accepted
@param threads: vector of threads to store the threads created, server: server object to handle server connections, chatServer: server object to handle chat server connections
@return void
*/
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer) {

    std::string command;

    while ( command != "1" && command != "2" ) { // Accept or Reject

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
            // Send to client to let them know they were rejected
            return;
        } else if ( choice == 1 ) { // Handle connection with clientService.
            threads.push_back(std::thread([&]() { clientService(server, server.getClientSockets().back(), chatServer, chatServer.getClientSockets().back()); }));
            threads.back().join();
            std::cout << "Thread created\n";
        }
    }

}