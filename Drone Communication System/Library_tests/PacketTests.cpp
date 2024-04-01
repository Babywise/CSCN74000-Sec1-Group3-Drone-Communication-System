#include "pch.h"
#include "CppUnitTest.h"
#include "../DCS Class Library/ChatWindowCommunication.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Librarytests
{

	TEST_CLASS(Packet_Tests)
	{
	public:

		TEST_METHOD(Init_Source_0)
		{
			Packet pckt("");
			Assert::AreEqual(0, pckt.getSource());
		}
		TEST_METHOD(Init_Destination_0)
		{
			Packet pckt("");
			Assert::AreEqual(0, pckt.getDestination());
		}
		TEST_METHOD(Init_fin_0)
		{
			Packet pckt("");
			Assert::AreEqual(0, pckt.getFin());
		}
		TEST_METHOD(Init_bytes)
		{
			Packet pckt = Packet();			
			Assert::AreEqual(0, pckt.getBytes());
		}

		TEST_METHOD(Init_pType_message)
		{
			Packet pckt = Packet();
			Assert::AreEqual((int)PacketType::packetInvalid, (int)pckt.getPacketType());
		}
		
		TEST_METHOD(Buffer)
		{
			Packet pckt = Packet();
			Packet pckt2(pckt.serialize());
			Assert::AreEqual(pckt.getBuffer(), pckt2.getBuffer());
		}
		TEST_METHOD(deserialize) {
			Packet pckt = Packet();
			pckt.setFin(1);
			Packet pckt2(pckt.serialize());
			Assert::AreEqual(1, pckt2.getFin());
		}



		
	};
}
