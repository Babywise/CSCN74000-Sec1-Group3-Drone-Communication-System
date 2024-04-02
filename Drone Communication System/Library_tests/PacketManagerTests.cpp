#include "pch.h"
#include "CppUnitTest.h"
#include "../DCS Class Library/PacketManager.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Librarytests
{

	TEST_CLASS(PacketManager_Tests)
	{
	public:

		TEST_METHOD(Packet_type_message)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home");
			PacketManager pcktMngr(msgpckt.serialize());
			pcktMngr.getPacketType();
			Assert::AreEqual((int)PacketType::packetMessage, (int)pcktMngr.getPacketType());
		}
		TEST_METHOD(Packet_deserialize)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home");
			PacketManager pcktMngr(msgpckt.serialize());
			MessagePacket msgPacketCompare = MessagePacket(pcktMngr.getPacket()->serialize());
			Assert::AreEqual((std::string)"welcome home", msgPacketCompare.getMessage());
		}
	};
}
