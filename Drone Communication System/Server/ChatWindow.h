/*
* Project: Next Level Drone Systems
* Module: Server
* Language: C++
*
* File: Chat Window.h
*
* Description: Contains the Chat Window class for the server module
*
* Authors : Danny Smith
*/
#pragma once

// Local Libraries
#include "../DCS Class Library/ChatWindowCommunication.h"
#include "conio.h"
#include "Server.h"
#include "ServerRequester.h"
// End Local Libraries

// Standard Libraries
#include <stdlib.h>
#include <vector>
#include <mutex>
// End Standard Libraries

// Definitions
#define LINE_COUNT 15
#define EXIT_COMMAND "exit\r"
#define BACKSPACE '\b'
#define ENTER '\r'
#define DEFAULT_DATE "2022-12-3"
#define CHECK_INTERVAL 100
#define ENTER_MESSAGE "Enter message: "

// Function to send a message from the server
bool sendChatMessage(Server& server, SOCKET& clientSocket, std::string message) {

	MessagePacket msgPacket;
	char messageToSend[MAX_MESSAGE_SIZE] = {};
	strcpy_s(messageToSend, message.c_str());
	msgPacket.setMessage(messageToSend);
	msgPacket.setCurrDate();
	PacketManager pM(msgPacket.serialize());
	Packet* packet = pM.getPacket();

	if ( server.sendPacket(*packet, clientSocket) > 0 ) {
		return true;
	} else {
		return false;
	}
}

// Function to recieve a message from the client
bool recieveChatMessage(Server& server, SOCKET& clientSocket) {

	char RxBuffer[maxPacketSize] = {};

	int bytesReceived = recv(clientSocket, RxBuffer, maxPacketSize, 0);

	if ( bytesReceived > 0 ) {

		PacketManager pM(RxBuffer);
		if ( pM.getPacketType() == PacketType::packetMessage ) {
			MessagePacket* msgPacket = new MessagePacket(RxBuffer);
			server.setCurrMessage(msgPacket->getMessage());
			return true;
		}
		return false;
	} else {
		return false;
	}
	return true;
}

// Function to send a message from the client
bool sendClientMessage(Client& client, std::string message) {
	/*
	* Send message to server
	*/
	MessagePacket msgPacket;
	char messageToSend[MAX_MESSAGE_SIZE] = {};
	strcpy_s(messageToSend, message.c_str());
	msgPacket.setMessage(messageToSend);

	PacketManager pM(msgPacket.serialize());
	Packet* packet = pM.getPacket();

	if ( client.sendPacket(*packet) > 0 ) {
		return true;
	} else {
		return false;
	}
}

// Function to recieve a message from the server
bool recieveClientMessage(Client& client) {

	char RxBuffer[maxPacketSize] = {};

	int bytesReceived = recv(client.getClientSocket(), RxBuffer, maxPacketSize, 0);

	if ( bytesReceived > 0 ) {

		PacketManager pM(RxBuffer);
		if ( pM.getPacketType() == PacketType::packetMessage ) {
			MessagePacket* msgPacket = new MessagePacket(RxBuffer);
			client.setCurrMessage(msgPacket->getMessage());
			return true;
		}
		return false;
	} else {
		return false;
	}
	return true;

}

// Function to print to a specific coordinate on the screen
void printToCoordinates(int y, int x, char* text)
{
	printf("\033[%d;%dH%s", y, x, text);
}

/*
* This class is used to create a chat window for the server
* It will display the chat messages and allow the user to send messages
* to the client
*/
class ChatWindow {
private:
	// Variables
	std::vector<ChatWindowCommunication> chats;
	std::mutex lock;
	bool hasUpdate = false;
	bool termination_pending = false;
	bool connected = false;
	// End Variables
public:

	std::string message = "";
	ChatWindow() {

	}
	/* Add Chat to chat queue */
	void addChat(char* date, std::string message) {
		lock.lock();
		ChatWindowCommunication newChat;
		newChat.setMessage(date + message);
		chats.push_back(newChat);
		hasUpdate = true;
		lock.unlock();
	}
	 /* Update the chat window */
	void updateWindow() {
		// CLEAR Screen
		std::system("cls");
		lock.lock();
		// load last LINE_COUNT messages into a std::vector
		std::vector<ChatWindowCommunication> lastChats;
		int counter = 0;
		for ( int i = this->chats.size() - 1; i >= 0; i-- ) {
			lastChats.push_back(chats[i]);
			counter++;
			if ( counter == LINE_COUNT ) {
				break;
			}
		}
		hasUpdate = false;
		lock.unlock();
		int index = 1;

		for ( int i = lastChats.size() - 1; i >= 0; i-- ) {
			printToCoordinates(index, 0, (char*)lastChats[i].getMessage().c_str());
			index++;
		}
		std::string output_message = ENTER_MESSAGE + this->message;
		printToCoordinates(LINE_COUNT + 1, 0, (char*)output_message.c_str());
	}
	/* Check if there is an update */
	bool HasUpdate() {
		return hasUpdate;
	}
	/* Set update to true */
	void updated() {
		hasUpdate = true;
	}
	/* Get all chats */
	std::vector<ChatWindowCommunication> getChats() {
		return chats;
	}
	/* Terminate the chat window */
	void terminate() {
		termination_pending = true;
	}
	/* Check if the chat window is terminating */
	bool isTerminating() {
		return termination_pending;
	}
	/* Check if the chat window is connected */
	void connect() {
		connected = true;
	}
	/* Check if the chat window is disconnected */
	void disconnect() {
		connected = false;
	}
	/* Check if the chat window is connected */
	bool isConnected() {
		return connected;
	}
};

// Function to update the chat window 
// runs as a daemon thread
void UpdateWindow(ChatWindow& window) {
	while ( !window.isTerminating() || window.HasUpdate() ) {
		std::this_thread::sleep_for(std::chrono::milliseconds(CHECK_INTERVAL));
		if ( window.HasUpdate() ) {
			window.updateWindow();
		}
	}
}
/* Runs a listener as a daemon thread and listens for messages to add to the chat queue*/
void listener(ChatWindow& window, Server& chatClient, SOCKET& clientSocket, std::string& message) {

	while ( (!window.isTerminating() || window.HasUpdate()) && message != EXIT_COMMAND ) {
		// if message received
		chatClient.setTimeout(clientSocket, 1);
		// wait for message
		if ( recieveChatMessage(chatClient, clientSocket) ) {
			window.addChat((char*)"", chatClient.getCurrMessage());
		}
		chatClient.clearCurrMessage();
		// if message is exit command and drone is disconnected
		if ( message == EXIT_COMMAND && window.isConnected() ) {
			sendChatMessage(chatClient, clientSocket, "[" + chatClient.getTowerID() + "] " + "Server has disconnected");
            // Set state to _Menu_

		}
		chatClient.setTimeout(clientSocket, 5000);
	}
	Sleep(1000);
}

int runChatWindow(Server& chatClient, SOCKET& clientSocket) {
	system("cls");
	ChatWindow CHAT;
	CHAT.connect();
	std::thread t1 = std::thread([&]() { UpdateWindow(CHAT); });
	std::thread t2 = std::thread([&]() { listener(CHAT, chatClient, clientSocket, CHAT.message); });
	std::string message = "";
	while ( true )
	{
		// get message from user
		std::string output_message = ENTER_MESSAGE + message;
		printToCoordinates(LINE_COUNT + 1, 0, (char*)output_message.c_str());
		char user_character = _getch();
		CHAT.updated();
		message += user_character;
		if ( user_character == ENTER ) {
			if ( message == EXIT_COMMAND) {
				CHAT.message = message;
				CHAT.disconnect();
				CHAT.terminate();
				break;
			} else {
				//send message to client
				std::string add_to_chat = "[" + chatClient.getTowerID() + "] " + CHAT.message;
				sendChatMessage(chatClient, clientSocket, add_to_chat);
				CHAT.addChat((char*)chatClient.getCurrDate().c_str(), " - " + add_to_chat);
			}
			message = "";
		} else if ( user_character == BACKSPACE ) {
			if ( message.size() > 1 ) {
				message.pop_back();
				message.pop_back();
			}
		}
		CHAT.message = message;
	}
	CHAT.addChat((char*)chatClient.getCurrDate().c_str(),(char*)"Goodbye!");
	t1.join();
	t2.join();
	return 0;
}


