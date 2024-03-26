#pragma once

#include <iostream>
#define HEADER_BYTE_SIZE 16
#define MAX_MESSAGE_SIZE 78000
#define SERVER_IP "127.0.0.1"
#define CLIENT_PORT 12345
#define CHAT_PORT 10000
#define LISTEN_PORT 20000
#define SERVER_PORT 12345
#define MAX_PACKET_SIZE 79124

using namespace std;

enum class PacketType {
	packetInvalid,
	packetMessage
};

const unsigned int maxPacketSize = MAX_PACKET_SIZE;
const unsigned int emptyPacketSize = HEADER_BYTE_SIZE;

class Packet {
protected:
	struct Header {
		unsigned Source : HEADER_BYTE_SIZE;			//2 Bytes -> max = 65,535
		unsigned Destination : HEADER_BYTE_SIZE;		//2 Bytes -> max = 65,535
		PacketType pType;				//4 Bytes -> max = 2,147,483,647
		unsigned int Bytes;				//4 Bytes -> max = 4,294,967,295
		unsigned Fin : HEADER_BYTE_SIZE;				//2 byte  -> max = 255
		unsigned Ack : HEADER_BYTE_SIZE;				//2 byte  -> max = 255
	} Head;

	char* pSerialBuff;					//4 Bytes
	char* buffer;						//4 Bytes

public:
	// creating
	Packet() : pSerialBuff(nullptr), buffer(nullptr) {
		Head.Source = 0, Head.Destination = 0, Head.pType = PacketType::packetInvalid, Head.Bytes = 0, Head.Fin = 0, Head.Ack = 0;
	};

	// receiving

	Packet(char* src) {
		std::memcpy(&this->Head, src, sizeof(Head));

		this->pSerialBuff = nullptr;
		this->buffer = nullptr;
	}

	void setSource(unsigned source) {
		this->Head.Source = source;
	}

	int getSource() {
		return this->Head.Source;
	}

	void setDestination(unsigned dest) {
		this->Head.Destination = dest;
	}

	int getDestination() {
		return this->Head.Destination;
	}

	void setPacketType(PacketType pType) {
		this->Head.pType = pType;
	}

	PacketType getPacketType() {
		return this->Head.pType;
	}

	void setBytes(int Bytes) {
		this->Head.Bytes = Bytes;
	}

	int getBytes() {
		return this->Head.Bytes;
	}

	void setAck(unsigned ack) {
		this->Head.Ack = ack;
	}

	int getAck() {
		return this->Head.Ack;
	}

	void setFin(unsigned fin) {
		this->Head.Fin = fin;
	}

	int getFin() {
		return this->Head.Fin;
	}

	char* getpSerialBuff() {
		return this->pSerialBuff;
	}

	char* getBuffer() {
		return this->buffer;
	}

	virtual char* serialize() {

		unsigned int TotalSize = emptyPacketSize + this->Head.Bytes;

		this->pSerialBuff = new char[TotalSize] { NULL };

		std::memcpy(this->pSerialBuff, &this->Head, sizeof(Head));
		std::memcpy(this->pSerialBuff, this->buffer, this->Head.Bytes);

		return this->pSerialBuff;

	}

};