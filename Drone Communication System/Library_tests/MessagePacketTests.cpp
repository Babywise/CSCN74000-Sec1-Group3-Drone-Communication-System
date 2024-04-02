#include "pch.h"
#include "CppUnitTest.h"
#include "../DCS Class Library/messagePacket.h"
#include <string>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Librarytests
{

	TEST_CLASS(MessagePacket_tests)
	{
	public:
		TEST_METHOD(Init_Source_0)
		{
			MessagePacket msgpckt = MessagePacket();
			Assert::AreEqual(0, msgpckt.getSource());
		}
		TEST_METHOD(Init_Destination_0)
		{
			MessagePacket msgpckt = MessagePacket();
			Assert::AreEqual(0, msgpckt.getDestination());
		}
		TEST_METHOD(Init_fin_0)
		{
			MessagePacket msgpckt = MessagePacket();
			Assert::AreEqual(0, msgpckt.getFin());
		}


		TEST_METHOD(Init_pType_message)
		{
			MessagePacket msgpckt = MessagePacket();
			Assert::AreEqual((int)PacketType::packetMessage, (int)msgpckt.getPacketType());
		}		

		TEST_METHOD(Write_Message_Space)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home");
			Assert::AreEqual((std::string)"welcome home", msgpckt.getMessage());
		}
		TEST_METHOD(Write_Message_semicolan)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home;");
			Assert::AreEqual((std::string)"welcome home;", msgpckt.getMessage());
		}
		TEST_METHOD(init_ack_0)
		{
			MessagePacket msgpckt = MessagePacket();
			Assert::AreEqual(0, msgpckt.getAck());
		}
		TEST_METHOD(Write_Message_serialize_check_buffer)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home");
			msgpckt.serialize();
			msgpckt.getBuffer();
			MessagePacket msgPacketCompare(msgpckt.serialize());
			Assert::AreEqual(msgPacketCompare.getBuffer(), msgpckt.getBuffer());
		}
		TEST_METHOD(Write_message_deserialize)
		{
			MessagePacket msgpckt = MessagePacket();
			msgpckt.setMessage("welcome home");
			msgpckt.serialize();
			msgpckt.getBuffer();
			MessagePacket msgPacketCompare(msgpckt.serialize());
			Assert::AreEqual(msgPacketCompare.getMessage(), msgpckt.getMessage());
		}
	};
}
