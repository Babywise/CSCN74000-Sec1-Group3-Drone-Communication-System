#pragma once
#include <stdlib.h>
#include <string>
#include "Packet.h"

/*
* Holds a communication object
*/
class ChatWindowCommunication {

private:
	//variables
	std::string message;
	std::string date;
	int sender_id;
	int receiver_id;

public:
	ChatWindowCommunication() : message(), date(), sender_id(0), receiver_id(0) {
	}
	//setters
	void setMessage(std::string message) {
		this->message = message;
	}
	void setSenderID(int sender_id) {
		this->sender_id = sender_id;
	}
	void setReceiverID(int receiver_id) {
		this->receiver_id = receiver_id;
	}

	//getters
	std::string getMessage() {
		return message;
	}
	int getSenderID() {
		return sender_id;
	}
	int getReceiverID() {
		return receiver_id;
	}

};