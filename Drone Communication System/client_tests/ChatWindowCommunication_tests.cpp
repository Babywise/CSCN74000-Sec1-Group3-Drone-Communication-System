#include "pch.h"
#include "CppUnitTest.h"
#include "../Shared/ChatWindowCommunication.h"
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
	};
}
