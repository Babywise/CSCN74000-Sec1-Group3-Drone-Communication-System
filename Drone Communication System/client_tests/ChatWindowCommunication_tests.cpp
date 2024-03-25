#include "pch.h"
#include "CppUnitTest.h"
#include "../DCS Class Library/ChatWindowCommunication.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Librarytests
{

	TEST_CLASS(ChatWindowCommunicationTests)
	{
	public:
		
		TEST_METHOD(set_text_hello)
		{
		ChatWindowCommunication cwc;
			cwc.setMessage("Hello");
			Assert::AreEqual(cwc.getMessage(), std::string("Hello"));
		}
		TEST_METHOD(set_text_hello_with_semicolan)
		{
			ChatWindowCommunication cwc;
			cwc.setMessage("Hello;");
			Assert::AreEqual(cwc.getMessage(), std::string("Hello;"));
		}
		TEST_METHOD(set_text_hello_with_Spaces)
		{
			ChatWindowCommunication cwc;
			cwc.setMessage("Hello World!");
			Assert::AreEqual(cwc.getMessage(), std::string("Hello World!"));
		}
		TEST_METHOD(set_text_hello_newline)
		{
			ChatWindowCommunication cwc;
			cwc.setMessage("Hello world\n test");
			Assert::AreEqual(cwc.getMessage(), std::string("Hello world\n test"));
		}
        TEST_METHOD(set_reciever_id)
        {
            ChatWindowCommunication cwc;
            cwc.setReceiverID(0010);
            Assert::AreEqual(cwc.getReceiverID(), 0010);
        }
        TEST_METHOD(set_sender_id)
        {
            ChatWindowCommunication cwc;
            cwc.setSenderID(0010);
            Assert::AreEqual(cwc.getSenderID(), 0010);
        }
        TEST_METHOD(inited_sender_id)
        {
            ChatWindowCommunication cwc;
            Assert::AreEqual(cwc.getSenderID(), 0);
        }
        TEST_METHOD(inited_reciever_id)
        {
            ChatWindowCommunication cwc;
            Assert::AreEqual(cwc.getReceiverID(), 0);
        }

        TEST_METHOD(inited_message)
        {
            ChatWindowCommunication cwc;
            cwc = ChatWindowCommunication();
            Assert::AreEqual(cwc.getMessage(), std::string(""));
        }

        TEST_METHOD(inited_date)
        {
            ChatWindowCommunication cwc;
            Assert::AreEqual(cwc.getMessage(), std::string(""));
        }
	};
}
