#include "Server.h"
#include "Packet.h"
#include "MessagePacket.h"
#include "PacketManager.h"

void openChat(Server& server, SOCKET& clientSocket);
bool recieveChatMessage(Server& server, SOCKET& clientSocket);
bool sendChatMessage(Server& server, SOCKET& clientSocket, std::string message);