#pragma once
#include "../Shared/Packet.h"
#define MAX_MESSAGE_SIZE 256

class MessagePacket : public Packet {
private:

    struct Body {
        char message[MAX_MESSAGE_SIZE] = {};
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

    MessagePacket(char* src) {
        // Deserialize the message from buffer
        std::memcpy(&this->Head, src, sizeof(Head));
        std::memcpy(&this->Body, src + sizeof(Head), sizeof(Body));

        this->pSerialBuff = nullptr;
        this->buffer = nullptr;
    }

    void setMessage(char* message) {

        strcpy_s(this->Body.message, message);
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