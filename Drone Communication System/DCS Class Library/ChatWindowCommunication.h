/*
* Project: Next Level Drone Systems
* Package: DCS Class Library
* Language: C++
*
* File: ChatWindowCommunication.h
*
* Description:
*
* Authors :
*/
#pragma once

//includes
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
		std::string d = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
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
	std::string getDate() {
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