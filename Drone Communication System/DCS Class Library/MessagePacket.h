/*
* Project: Next Level Drone Systems
* Package: DCS Class Library
* Language: C++
*
* File: MessagePacket.h
*
* Description: Contains the MessagePacket class for the DCS Class Library
*
* Authors : Islam Ahmed
*/
#pragma once
// includes
#include "../DCS Class Library/Packet.h"
#include <string>
#include "../DCS Class Library/Logger.h"

/*
* Message Packet class that holds the message and date
* Inherits from Packet
*/
class MessagePacket : public Packet {
private:

    struct Body {
        char message[MAX_MESSAGE_SIZE] = {};
        char date[20] = {};
    } Body;

public:

    MessagePacket() : Packet() {
        Head.Source = 0,
            Head.Destination = 0,
            Head.Bytes = sizeof(Body) + emptyPacketSize,
            Head.Fin = 0,
            Head.Ack = 0,
            Head.pType = PacketType::packetMessage;
    };

    // Deserialize the message from buffer
    MessagePacket(char* src) {
        std::memcpy(&this->Head, src, sizeof(Head));
        std::memcpy(&this->Body, src + sizeof(Head), sizeof(Body));

        this->pSerialBuff = nullptr;
        this->buffer = nullptr;
    }

     // Setters
    std::string setCurrDate() {
		time_t now = time(0);
		tm* ltm = localtime(&now);
		std::string d = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
		strcpy(this->Body.date, d.c_str());
		return d;
	}
    // Message
    void setMessage(char* message) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        std::string d = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
        strcpy(this->Body.date, d.c_str());
        strcpy_s(this->Body.message, message);
    }

    //getters
    std::string getCurrDate() {
	    return this->Body.date;
	}
    std::string getMessage() {
        if (this->Body.date == "") {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            std::string d = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
            return d + (std::string)" - " + this->Body.message;
        }
        return this->Body.date + (std::string)" - " +this->Body.message;
    }

    // Serialize the packet
    char* serialize() override {
        unsigned int totalSize = emptyPacketSize + this->Head.Bytes;

        this->pSerialBuff = new char[totalSize] { NULL };

        std::memcpy(this->pSerialBuff, &this->Head, sizeof(Head));
        std::memcpy(this->pSerialBuff + sizeof(Head), &this->Body, sizeof(this->Body));
        Logger log;
        std::string append;
        if(this->Head.Source == 0)
			append = "Client";
		else
			append = "Server";
        log.log(this->Body.message + (std::string)" From: "+ append, 0, "MessagePacket");
        return this->pSerialBuff;
    }

};