/*
* Project: Next Level Drone Systems
* Package: DCS Class Library
* Language: C++
*
* File: PacketManager.h
*
* Description: Contains the PacketManager class for the DCS Class Library
*
* Authors : Islam Ahmed
* 
*/
#pragma once

//includes
#include "Packet.h"
#include "MessagePacket.h"

// Packet Manager Class for converting packets to their 
// respective types
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
		case PacketType::packetInvalid: // invalid packet
			//throw new exception("Not Implemented");
			break;
		case PacketType::packetMessage: // message packet
			return new MessagePacket(buffer);
			break;
		default:
			return this->packet;
		}
	}

};