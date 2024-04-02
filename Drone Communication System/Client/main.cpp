/*
* Project: Next Level Drone Systems
* Module: Client
* Language: C++
*
* File:[Client] - main.cpp
*
* Description: 
*
* Authors :
*         1. Islam Ahmed
*         2. Danny Smith
*         3. Nicholas Prince
*/

//Include Local Libraries
#include "../DCS Class Library/Packet.h"
#include "../DCS Class Library/MessagePacket.h"
#include "Client.h"
#include "ChatWindow.h"
#include "ClientListeningServer.h"
#include "ClientMenus.h"
// End Include Local Libraries

// Include
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include <codecvt>
// End Include

//Define
#define DRONE_ID "D001"
#define SERVER_IMAGE_PATH "./Images/"

//Enums
enum class ServerState {
    ACTIVE,
    INACTIVE,
    WAITING_IMAGE,
    CHATTING,
    LISTENING
};

//Declarations
void clientService(Client& client, Client& chatClient);
void checkConnectionsFromServer(Client& client, Client& chatClient, Server& server);
void mainLoop();
bool sendImage(Client& chatClient);
void getState();
void stateMachine();
ServerState STATE = ServerState::INACTIVE;
// End Declarations

/*
* Runs as a daemon thread to send the state of the drone to the client
* When the drone connects and then closes the socket
*/
void stateMachine() {
    while ( true ) {
        Server state_machine(DRONE_ID, CLIENT_STATE_PORT);
        state_machine.listenforConnection();
        state_machine.acceptConnection();
        MessagePacket* msgPacket = new MessagePacket();
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
        // send chat message
        sendServerMessage(state_machine, state_machine.getClientSockets().back(), msgPacket->getMessage());
        state_machine.closeLastConnection();
        state_machine.shutdownServer();
    }
}
/*
* Connects to the client state machine to get the state and returns it.
*/
void getState() {
    Client state_client(DRONE_ID);
    state_client.connectToServer(SERVER_IP, SERVER_STATE_PORT);
    recieveChatMessage(state_client);
    std::cout << "State: " << state_client.getCurrMessage() << std::endl;
    state_client.closeConnection();
}

/*
* Main function to run the client
*/
int main(void) {
    std::thread state_machine = std::thread([&]() { stateMachine(); }); // Daemon thread to send state to client
    state_machine.detach();
    mainLoop();
    return 0;
}
 // Runs the client connection program until the user exits
void mainLoop() {

    std::string command;

    while ( command != "3" ) { // while user not exited

        std::system("cls");
        Client client(DRONE_ID);
        Client chatClient(DRONE_ID);
        Server server(DRONE_ID, LISTEN_PORT);

        if (!server.listenforConnection()) {
            std::cout << "Server Listening Failed.\n";
            break;
        }

        std::cout << "Listening for connections...\n";

        // Main menu
        while (command != "1" && command != "2" && command != "3") {

            clientStartMenu(DRONE_ID);

            std::cin >> command;
            int choice = std::stoi(command);

            // Connect to the server
            if (choice == 1) {
                std::cout << "Waiting...\n";
                if (!client.connectToServer(SERVER_IP, CLIENT_PORT)) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                if (!chatClient.connectToServer(SERVER_IP, CHAT_PORT)) {
                    std::cout << "Chat Server Connection Failed.\n";
                    break;
                }

                // main loop
                clientService(client, chatClient); // Runs the connected interface

            // Check Connections
            } else if (choice == 2) {
                checkConnectionsFromServer(client, chatClient, server); // check for active connections and run client service if one is accepted
            // Exit
            } else if ( choice == 3 ) {
                std::cout << "Thank you for using Drone Communication System!\n";
                Sleep(2000);
                break;
            // Invalid option
            } else {
                std::cout << "Invalid Option.\n";
            }
            
        }

        // Reset menu option
        if ( command != "3" ) {
            command.erase();
        }

        // Close connections
        client.closeConnection();
        chatClient.closeConnection();
        server.shutdownServer();
    }
}

/*
* This function sends an image to the server. It allows the user to input a file name and sends the image data to the server.
@param chatClient The client object used to send the image data to the server.
@return bool
*/
bool sendImage(Client& chatClient) {

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    
    //check if image directory exists
    if ( CreateDirectoryW(converter.from_bytes(SERVER_IMAGE_PATH).c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS ) {
        std::cout << "Folder exists or was created successfully." << std::endl;
    } else {
        std::cout << "Failed to create folder." << std::endl;
    }

    std::queue<std::string> imageQueue;
    std::string filename;

    // Get list of files to transmit
    while (true) { 
        std::cout << "Enter the name of the image file to send (or 'done' to finish): ";
        std::cin >> filename;
        if (filename == "done") {
            break;
        }
        imageQueue.push(filename);
    }

    // transmit all images
    while (!imageQueue.empty()) { 
        std::string filename = imageQueue.front();
        imageQueue.pop();
        std::string filepath = SERVER_IMAGE_PATH + filename;
        std::ifstream file(filepath, std::ios::binary);


        if (!file) {
            std::cerr << "Failed to open image file\n";
            continue;
        }

        // Get image information
        std::string imageData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        int imageSize = imageData.size();
        std::cout << "Sending image size: " << imageSize << " bytes\n";
        send(chatClient.getClientSocket(), reinterpret_cast<const char*>(&imageSize), sizeof(imageSize), 0); // send image size
        Logger l;
        std::string log("Sending image size: " + std::to_string(imageSize) + " bytes\n");
        l.log(log, 0, "MessagePacket");
        int totalBytesSent = 0;
        int packetSize = MAX_MESSAGE_SIZE;
        // Send image data
        while (totalBytesSent < imageSize) {
            int bytesToSend = std::min<int>(packetSize, imageSize - totalBytesSent);
            int bytesSent = send(chatClient.getClientSocket(), imageData.data() + totalBytesSent, bytesToSend, 0);
            l.log("Sent " + std::to_string(bytesSent) + " bytes (Total: " + std::to_string(totalBytesSent) + "/" + std::to_string(imageSize) + ")\n", 0, "MessagePacket");
            if (bytesSent == SOCKET_ERROR) {
                std::cerr << "Error sending data: " << WSAGetLastError() << '\n';
                file.close();
                break;
            }
            totalBytesSent += bytesSent;
            std::cout << "Sent " << bytesSent << " bytes (Total: " << totalBytesSent << "/" << imageSize << ")\n"; // update send status to screen
        }

        std::cout << "Image sent successfully\n";
        file.close();

        // Wait for acknowledgment
        char ack[MAX_MESSAGE_SIZE];
        int bytesReceived = recv(chatClient.getClientSocket(), ack, sizeof(ack), 0);
        if (bytesReceived > 0) {
            ack[bytesReceived-1] = '\0';
            std::cout << "Server acknowledgment: " << ack << '\n';
        }

        Sleep(1000); // Pause before sending the next image
    }

    // Signal end of transmission
    int endSignal = -1; // Use -1 to indicate the end of image transmission
    send(chatClient.getClientSocket(), reinterpret_cast<const char*>(&endSignal), sizeof(endSignal), 0);
    Logger l;
    l.log("End of image transmission\n", 0, "MessagePacket");
    system("pause");
    return true;
    
}

/*
* This function is used to handle the main menu once the client has connected to the server
@param client: client object to handle server connections, chatClient: client object to handle chat communication
@return void
*/
void clientService(Client& client, Client& chatClient) {

    bool running = true;

    while ( running ) {

        // Print Menu
        clientDroneMenu(client.getDroneID(), client.getTowerID());

        std::string command;
        std::cin >> command;
        int choice = std::stoi(command);
        switch ( choice ) {
        // run chat window
        case 1:
            runChatWindow(chatClient);
            break;
        // Send images
        case 2:
            std::cout << "ImageStatus: " << sendImage(chatClient) << std::endl;
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
}

/*
* This function is used to check the connections from the server
@param client: client object to handle server connections, chatClient: client object to handle chat communication, server: server object to handle server connections
@return void
*/
void checkConnectionsFromServer(Client& client, Client& chatClient, Server& server) {

    std::string command;

    while ( command != "1" && command != "2" ) {

        // Print Menu
        clientConnectionMenu();

        std::cin >> command;
        int choice = std::stoi(command);
        // Regret Connection
        if ( choice == 2 ) {
            /*if ( !server.closeLastConnection() ) {
                std::cout << "Closing Server Connection Failed.\n";
                break;
            }*/
            return;
        // Go back
        } else if ( choice == 3 ) {
            return;
        } else if ( choice == 1 ) {
            if ( !server.acceptConnection() ) {
                std::cout << "Accepting Server Connection Failed.\n";
                break;
            } else {
                std::cout << "Server Connection (SUCCESS).\n";

                // Creat and Send message to server
                std::string message = DRONE_ID + std::string("| Accepted Your Connection");
                MessagePacket msgPacket;
                char messageToSend[MAX_MESSAGE_SIZE] = {};
                strcpy_s(messageToSend, message.c_str());
                msgPacket.setMessage(messageToSend);

                PacketManager pM(msgPacket.serialize());
                Packet* packet = pM.getPacket();
                // let the server know that the client has accepted the connection
                int response = send(server.getClientSockets().back(), packet->serialize(), maxPacketSize, 0);

                if ( !client.connectToServer(SERVER_IP, CLIENT_PORT) ) {
                    std::cout << "Server Connection Failed.\n";
                    break;
                }

                if ( !chatClient.connectToServer(SERVER_IP, CHAT_PORT) ) {
                    std::cout << "Chat Server Connection Failed.\n";
                    break;
                }
                // Run main loop
                clientService(client, chatClient);
            }
        }
    }

}