#pragma once
#include "Packet.h"

class MessagePacket : public Packet {
private:
    struct Body {
        std::string message;
    } Body;
public:

    MessagePacket() : Packet() {
        Head.Source = 0,
            Head.Destination = 0,
            Head.Bytes = sizeof(Body) + emptyPacketSize,
            Head.Fin = 0,
            Head.Ack = 0,
            Head.pType = PacketType::packetMessage;
        Body.message.erase();
    };

    MessagePacket(char* src) {
        // Deserialize the message from buffer
        std::memcpy(&this->Head, src, sizeof(Head));
        std::memcpy(&this->Body, src + sizeof(Head), sizeof(Body));

        this->pSerialBuff = nullptr;
        this->buffer = nullptr;
    }

    void setMessage(string message) {
        this->Body.message = message;
    }

    std::string getMessage() {
        return this->Body.message;
    }

    char* serialize() override {
        unsigned int totalSize = emptyPacketSize + this->Head.Bytes;

        this->pSerialBuff = new char[totalSize] { NULL };

        std::memcpy(this->pSerialBuff, &this->Head, sizeof(Head));
        std::memcpy(this->pSerialBuff + sizeof(Head), &this->Body, sizeof(this->Body));

        return this->pSerialBuff;
    }

};