/*
* Project: Next Level Drone Systems
* Module: server
* Language: C++
*
* File: ServerRequester.cpp
*
* Description: Runs server code on the server for tx communication with the client
* during the chat window
*
* Authors :
*          1. Islam Ahmed
*          2. Danny Smith
*          3. Nicholas Prince
*/

// Local Libraries
#include "Server.h"
#include "ChatWindow.h"
#include "ServerRequester.h"
#include "ServerMenus.h"
#include "../DCS Class Library/Packet.h"

// System Libraries
#include <Windows.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <codecvt> 

// defines
#define TOWER_ID "T001"
#define NOTIFY_CONNECTION_LINE 6
#define IMAGE_PATH "./Images/"

//enums
enum class ServerState {
    ACTIVE,
    INACTIVE,
    WAITING_IMAGE,
    CHATTING,
    LISTENING
};

// declarations
int clientService(Server& server, Server& chatServer, SOCKET& clientChatSocket);
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer);
void mainLoop(bool& connectionPending, bool& listening, std::string& command, bool& menuSelected, bool& connectionFailed);
void main_program();
void stateMachine();
void getState();
ServerState STATE = ServerState::INACTIVE;
std::string droneID = "replace_me_droneID";
std::string extension = ".jpg";



/* Runs a daemon thread to provide the current state to any clients that connect*/
void stateMachine() {

    while ( true ) {
        Server state_machine(TOWER_ID, SERVER_STATE_PORT);
        state_machine.listenforConnection();
        state_machine.acceptConnection();
        MessagePacket* msgPacket = new MessagePacket();
        // convert enum to string
        switch ( STATE ) { 
        case ServerState::LISTENING:
            msgPacket->setMessage((char*)"LISTENING");
            break;
        case ServerState::ACTIVE:
            msgPacket->setMessage((char*)"ACTIVE");
            break;
        case ServerState::INACTIVE:
            msgPacket->setMessage((char*)"INACTIVE");
            break;
        case ServerState::WAITING_IMAGE:
            msgPacket->setMessage((char*)"WAITING_IMAGE");
            break;
        case ServerState::CHATTING:
            msgPacket->setMessage((char*)"CHATTING");
            break;
        }
        sendChatMessage(state_machine, state_machine.getClientSockets().back(), msgPacket->getMessage());
        state_machine.closeLastConnection();
        state_machine.shutdownServer();
    }
}
// Gets the current state from the client
void getState() {
    Client state_server(TOWER_ID);
    state_server.connectToServer(SERVER_IP, CLIENT_STATE_PORT);
    recieveClientMessage(state_server);
    std::cout << "State: " << state_server.getCurrMessage() << std::endl;
    state_server.closeConnection();
}


/*
* Main function to run the server and client
*/
int main(void) {
    std::thread state_machine = std::thread([&]() { stateMachine(); }); // Daemon thread to send state to client
    state_machine.detach();
    //getState();
    //system("pause");
    main_program();
    return 0;
}
/*
* Runs the main program for the server
*/
void main_program() {
    std::string command;

    while ( command != "3" ) {

        bool connectionPending = false;
        bool listening = false;
        bool menuSelected = false;
        bool connectionFailed = false;
        std::thread main = std::thread([&]() { mainLoop(connectionPending, listening, command, menuSelected, connectionFailed); });

        while ( listening == false ) {}   // wait to start listening

    while (listening == false) {}   // wait to start listening


    while ( command != "3" ) {

        Client client(TOWER_ID);

        // Main Menu
        while (command != "1" && command != "2" && command != "3") { // whitelist options

            serverStartMenu(TOWER_ID);

            std::cin >> command;
            int choice = std::stoi(command);
            menuSelected == true;

            // Connect to the Client
            if (choice == 1) {
                std::cout << "Waiting...\n";
                if (!client.connectToServer(SERVER_IP, LISTEN_PORT)) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                char RxBuffer[maxPacketSize] = {};
                if (recv(client.getClientSocket(), RxBuffer, maxPacketSize, 0) <= 0) {
                    std::cout << "Response Lost.\n";
                    connectionFailed = true;
                    connectionPending = false;
                    main.join();
                    break;
                }

                //Recv Msg Packet
                PacketManager pM(RxBuffer);
                if (pM.getPacketType() == PacketType::packetMessage) {
                    MessagePacket* msgPacket = new MessagePacket(RxBuffer);
                    client.setCurrMessage(msgPacket->getMessage());
                    std::cout << client.getCurrMessage() << "\n";
                }
                Sleep(5000);
                connectionPending = false;
                main.join();

            }
            // Check Connections
            else if (choice == 2) {
                Sleep(6000);
                if (connectionPending == true) {
                    connectionPending = false;
                    main.join();
                }
            
            // Exit
            } else if ( choice == 3 ) {
                std::cout << "Thank you for using Drone Communication System!\n";
                Sleep(2000);
                break;
            }
            // Invalid Option
            else {
                std::cout << "Invalid Option.\n";
            }
        }

        // Reset menu option
        if ( command != "3" ) {
            command.erase();
        }

        // Close client Socket
        client.closeConnection();
    }
}

/*
* This function runs a listening socket to listen for connection requests and alert the user when a connection is incoming
* When the user accepts the connection this function proceeds and handles the connection
@param connectionPending: bool to check if a connection is pending, listening: bool to check if the server is listening
@return void
*/

void mainLoop(bool& connectionPending, bool& listening, std::string& command, bool& menuSelected, bool& connectionFailed) {

    // Main server loop to accept connections and handle them
    // while Exit is not selected
    while ( command != "3" ) {

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
        // inform main loop we are listening
        listening = true; 
        bool continueProgram = true;

        // Wait for user to select an option
        while ( menuSelected != true) {
            // Exit
            if ( command == "3" ) {
                continueProgram = false;
                break;
            // Valid Menu Option
            } else if ( command == "1" || command == "2" ) {
                continueProgram = true;
                break;
            }
            //else { return; }
        }
        // Wait for connectionFailed to be updated
        Sleep(5000);
        if ( continueProgram && !connectionFailed ) {

            if ( !server.acceptConnection() ) {
                std::cout << "Accepting Server Connection Failed.\n";
                break;
            }
            if ( !chatServer.acceptConnection() ) {
                std::cout << "Accepting Chat Server Connection Failed.\n";
                break;
            }
            // Connection was made so inform main loop that a connection is pending
            connectionPending = true;
            // The server is no longer listening since the sockets are used
            listening = false; 
            // Loop until user accepts connection
            while ( connectionPending != false ) { 
                printToCoordinates(NOTIFY_CONNECTION_LINE, 0, (char*)"Connection incoming... (10)");
            }

            //Potentially move this to main
            checkConnectionsFromClient(threads, server, chatServer);
        }

        // Close server sockets
        server.shutdownServer();
        chatServer.shutdownServer();

        // Reset menu option
        if ( command != "3" || connectionFailed) {
            menuSelected = false;
            command.erase();
            //main_program();
            return;

        }

    }
}

/*
* This function is used to recieve an image from the client
@param clientChatSocket: socket to handle image communication
@return bool
*/
bool receiveImage(SOCKET& clientChatSocket) {

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    //check if image directory exists
    if (CreateDirectoryW(converter.from_bytes(IMAGE_PATH).c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Folder exists or was created successfully." << std::endl;
    }
    else {
        std::cout << "Failed to create folder." << std::endl;
    }

    while (true) {

        int imageSize;
        int recvResult = recv(clientChatSocket, reinterpret_cast<char*>(&imageSize), sizeof(imageSize), 0);

        // Check if there are any images to receive
        if (recvResult <= 0 || imageSize == -1) {
            std::cout << "No more images to receive or connection closed by client.\n";
            break;
        }

        //save image to file
        std::string filename = IMAGE_PATH + string("received_image_") + std::to_string(time(nullptr)) + extension;
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to create file\n";
            break;
        }

        // Receive image data
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
    bool running = true;

    while ( running ) {

        // Print Menu
        serverDroneMenu(server.getTowerID(), server.getDroneID());

        std::string command;
        std::cin >> command;

        int choice = std::stoi(command);

        switch ( choice ) {
        // run chat window
        case 1:
            runChatWindow(chatServer, clientChatSocket);
            break;
        // Request images
        case 2:
            receiveImage(clientChatSocket);
            break;
        // Exit chat
        case 3:
            std::cout << "GoodBye!" << std::endl;
            running = false;
            break;
        // No valid option selected
        default:
            std::cout << "No Option Selected.\n";
            break;
        }
    }
        
    return 1;

}

/*
* This function is used to check on a connection request from a client and allow the user to accept or reject the connection
* Push a new thread to the std::vector of threads to handle the connection then join the thread if connection is accepted
@param threads: std::vector of threads to store the threads created, server: server object to handle server connections, chatServer: server object to handle chat server connections
@return void
*/
void checkConnectionsFromClient(std::vector<std::thread>& threads, Server& server, Server& chatServer) {

    std::string command;

    while ( command != "1" && command != "2" ) { // Accept or Reject

        // Print Menu
        serverConnectionMenu();

        std::cin >> command;
        int choice = std::stoi(command);
        // Regret Connection
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
        // Go back
        } else if ( choice == 3 ) {
            return;
        // Accept Connection and bind to a new thread
        } else if ( choice == 1 ) { // Handle connection with clientService.
            threads.push_back(std::thread([&]() { clientService(server, chatServer, chatServer.getClientSockets().back()); }));
            threads.back().join();
            std::cout << "Thread created\n";
        }
    }

}