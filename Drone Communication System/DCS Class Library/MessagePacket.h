#pragma once
#include "../DCS Class Library/Packet.h"
#include <string>
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

    // Message
    std::string setCurrDate() {
		time_t now = time(0);
		tm* ltm = localtime(&now);
		string d = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
		strcpy(this->Body.date, d.c_str());
		return d;
	}
    std::string getCurrDate() {
	    return this->Body.date;
	}
    void setMessage(char* message) {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        string d = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
        strcpy(this->Body.date, d.c_str());
        strcpy_s(this->Body.message, message);
    }

    std::string getMessage() {
        if (this->Body.date == "") {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            string d = to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + " " + to_string(ltm->tm_hour) + ":" + to_string(ltm->tm_min) + ":" + to_string(ltm->tm_sec);
            return d + (string)" - " + this->Body.message;
        }
        return this->Body.date + (string)" - " +this->Body.message;
    }

    // Serialize the packet
    char* serialize() override {
        unsigned int totalSize = emptyPacketSize + this->Head.Bytes;

        this->pSerialBuff = new char[totalSize] { NULL };

        std::memcpy(this->pSerialBuff, &this->Head, sizeof(Head));
        std::memcpy(this->pSerialBuff + sizeof(Head), &this->Body, sizeof(this->Body));

        return this->pSerialBuff;
    }

};