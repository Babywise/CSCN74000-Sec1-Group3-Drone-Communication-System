#pragma once
#include <stdlib.h>
#include <string>
#include "Packet.h"

/*
* Holds a communication object
* Includes the message, date, sender id, and receiver id
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
		time_t now = time(0);
		tm* ltm = localtime(&now);
		string d = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);	
		this->date = d;
		this->message = message;
	}
	void setSenderID(int sender_id) {
		this->sender_id = sender_id;
	}
	void setReceiverID(int receiver_id) {
		this->receiver_id = receiver_id;
	}

	//getters
	string getDate() {
		return this->date;
	}

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