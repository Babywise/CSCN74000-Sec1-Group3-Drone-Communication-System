#include "Client.h"
#include "MessagePacket.h"
#include "PacketManager.h"

void openChat(Client& client);
bool sendChatMessage(Client& client, std::string message);
bool recieveChatMessage(Client& client);