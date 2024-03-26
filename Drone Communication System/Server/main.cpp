#include "Server.h"
#include "ChatWindow.h"
#include "ServerRequester.h"
#include "ServerMenus.h"

#include <thread>
#include <iostream>
#include <fstream>

int clientService(Server& server, Server& chatServer, SOCKET& clientChatSocket);
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer);
void mainLoop(bool& connectionPending, bool& listening);

#define TOWER_ID "AA001"
#define SERVER_PORT 12345
#define CHAT_PORT 10000
#define LISTEN_PORT 20000
#define NOTIFY_CONNECTION_LINE 6
string droneID = "replace_me_droneID";
#define IMAGE_PATH "./Images/received_image_"
string extension = ".jpg";
#define SERVER_ADDRESS "127.0.0.1"
void main_program();
/*
* Main function to run the server and client
*/
int main(void) {

    main_program();

    return 0;
}

void main_program() {
    bool connectionPending = false;
    bool listening = false;
    std::thread main = std::thread([&]() { mainLoop(connectionPending, listening); });


    while (listening == false) {}   // wait to start listening


    while (true) {
        Client client(TOWER_ID);

        std::string command;

        while (command != "1" && command != "2" && command != "3") { // user can connect / check, disconnect

            serverStartMenu(TOWER_ID);

            std::cin >> command;
            int choice = std::stoi(command);
            if (choice == 1) {
                // Connect to the server
                std::cout << "Waiting...\n";
                if (!client.connectToServer(SERVER_ADDRESS, LISTEN_PORT)) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                char RxBuffer[maxPacketSize] = {};
                if (recv(client.getClientSocket(), RxBuffer, maxPacketSize, 0) <= 0) {
                    std::cout << "Response Lost.\n";
                    break;
                }

                PacketManager pM(RxBuffer);
                if (pM.getPacketType() == PacketType::packetMessage) {
                    MessagePacket* msgPacket = new MessagePacket(RxBuffer);
                    client.setCurrMessage(msgPacket->getMessage());
                    std::cout << client.getCurrMessage() << "\n";
                }
                Sleep(2000);
                connectionPending = false;
                main.join();

            }
            else if (choice == 2) {

                if (connectionPending == true) {
                    connectionPending = false;
                    main.join();
                }

            }
            else {
                std::cout << "Invalid Option.\n";
            }
        }

        client.closeConnection();
    }
}

/*
* This function runs a listening socket to listen for connection requests and alert the user when a connection is incoming
* When the user accepts the connection this function proceeds and handles the connection
@param connectionPending: bool to check if a connection is pending, listening: bool to check if the server is listening
@return void
*/
void mainLoop(bool& connectionPending, bool& listening) {
    // Main server loop to accept connections and handle them
    while (true) {

        std::system("cls");

        Server server(TOWER_ID, SERVER_PORT); // open tx port
        Server chatServer(TOWER_ID, CHAT_PORT); // open rx port

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
        listening = true; // inform main loop we are listening

        if ( !server.acceptConnection() ) {
            std::cout << "Accepting Server Connection Failed.\n";
            break;
        }
        if ( !chatServer.acceptConnection() ) {
            std::cout << "Accepting Chat Server Connection Failed.\n";
            break;
        }

        connectionPending = true; // Connection was made so inform main loop that a connection is pending
        listening = false; // The server is no longer listening since the sockets are used
        while ( connectionPending != false ) { // until user accepts connection
            printToCoordinates(NOTIFY_CONNECTION_LINE, 0, (char*)"Connection incoming... (10)");
        }

        //Potentially move this to main
        checkConnectionsFromClient(threads, server, chatServer);

        server.shutdownServer();
        chatServer.shutdownServer();
    }
}

/*
* This function is used to recieve an image from the client
@param clientChatSocket: socket to handle image communication
@return bool
*/
bool receiveImage( SOCKET& clientChatSocket) {

    while (true) {

        int imageSize;
        int recvResult = recv(clientChatSocket, reinterpret_cast<char*>(&imageSize), sizeof(imageSize), 0);

        if (recvResult <= 0 || imageSize == -1) {
            std::cout << "No more images to receive or connection closed by client.\n";
            break;
        }

        std::string filename = IMAGE_PATH + std::to_string(time(nullptr)) + extension;
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create file\n";
            break;
        }

        char buffer[MAX_MESSAGE_SIZE];
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
* This function is used to handle the main menu once the client has connected to the server
@param: server: server object to handle server connections, chatServer: server object to handle chat communication, clientChatSocket: socket to handle chat communication
@return int
*/
int clientService(Server& server, Server& chatServer, SOCKET& clientChatSocket) {

    //get and set drone id
    //->
    server.setDroneID(droneID);
    while ( true ) {

        serverDroneMenu(server.getTowerID(), server.getDroneID()); // Print Menu

        std::string command;
        std::cin >> command;

        int choice = std::stoi(command);

        switch ( choice ) {
        case 1:
            runChatWindow(chatServer, clientChatSocket); // run chat window
            break;
        case 2:
            receiveImage(clientChatSocket); // Request image
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
            threads.push_back(std::thread([&]() { clientService(server, chatServer, chatServer.getClientSockets().back()); }));
            threads.back().join();
            std::cout << "Thread created\n";
        }
    }

}