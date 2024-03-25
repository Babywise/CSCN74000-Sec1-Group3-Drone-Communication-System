#pragma once
#include "../Shared/Packet.h"
#include "../Shared/MessagePacket.h"

class PacketManager {
	Packet* packet;
	PacketType pType;
public:

	PacketManager(char* buffer) {
		Packet* p = new Packet(buffer);
		this->pType = p->getPacketType();
		this->packet = this->setupPacket(buffer);
	}

	Packet* getPacket() {
		return this->packet;
	}

	PacketType getPacketType() {
		return this->pType;
	}

private:
	Packet* setupPacket(char* buffer) {

		switch (this->pType)
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