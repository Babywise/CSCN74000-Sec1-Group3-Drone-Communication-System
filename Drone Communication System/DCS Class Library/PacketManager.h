#pragma once
#include "Packet.h"
#include "MessagePacket.h"

class PacketManager {
	Packet* packet;
	PacketType pType;
public:

	// Handle Packet using univeral manager
	PacketManager(char* buffer) {
		Packet* p = new Packet(buffer);
		this->pType = p->getPacketType();
		this->packet = this->setupPacket(buffer);
	}

	// Get Packet
	Packet* getPacket() {
		return this->packet;
	}

	// Get Packet Type
	PacketType getPacketType() {
		return this->pType;
	}

private:
	// Setup Packet
	Packet* setupPacket(char* buffer) {

		switch ( this->pType )
		{
		case PacketType::packetInvalid:
			//throw new exception("Not Implemented");
			break;
		case PacketType::packetMessage:
			return new MessagePacket(buffer);
			break;
		default:
			return this->packet;
		}
	}

};