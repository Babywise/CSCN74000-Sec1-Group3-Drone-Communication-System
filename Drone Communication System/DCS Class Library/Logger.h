/*
* Project: Next Level Drone Systems
* Package: DCS Class Library
* Language: C++
*
* File: Logger.h
*
* Description: Contains the Logger class for the DCS Class Library
*
* Authors : Danny Smith
*/
#pragma once

// includes
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <experimental/filesystem>

//define statements
#define LOGSPATH "../Logs/"
#define DEFAULT_LOG "log"
#define LOGEXTENSION ".log"
#define ARCHIVE "../Archive/"
#define ARCHIVE_EXT ".archive"

/*
* Logger class
* Used to log messages to a file
* @param message - log
* @param severity - severity of the log
* @param logname - name of the log
*/
class Logger {
	std::string fileTimeName;
	void generateFileTime() {
		std::time_t fileTimeCreation = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char* ctimeBuf = std::ctime(&fileTimeCreation);
		if (ctimeBuf[strlen(ctimeBuf) - 1] == '\n') ctimeBuf[strlen(ctimeBuf) - 1] = '\0';
		for (int i = 0; i < strlen(ctimeBuf) - 1; i++) {
			if (ctimeBuf[i] == ':') {
				ctimeBuf[i] = '-';
			}
		}
		
		//this->fileTimeName = ctimeBuf;
		this->fileTimeName += "";
	}
public:
	Logger() {
// check if the logs directory exists
if (!std::experimental::filesystem::exists(LOGSPATH)) {
			std::experimental::filesystem::create_directory(LOGSPATH);
		}
		Logger::generateFileTime();
	}
	std::string getFileTimeName() {
		return this->fileTimeName;
	}
	void log(std::string message)
	{
		Logger::log(message, -1, DEFAULT_LOG);

	}
	void log(std::string message, int severity) {
		Logger::log(message, severity, DEFAULT_LOG);

	}
	void log(std::string message, int severity, std::string logname) {
		auto now = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(now);

		std::ofstream outfile;
		outfile.open((std::string)LOGSPATH + (std::string)this->fileTimeName + (std::string)logname + (std::string)LOGEXTENSION, std::ios::app);
		char* ctimeBuf = std::ctime(&end_time);
		if (ctimeBuf[strlen(ctimeBuf) - 1] == '\n') ctimeBuf[strlen(ctimeBuf) - 1] = '\0';
		// replace endl with \0
		outfile << ctimeBuf << "\t Severity :" << severity << "\t" << message << std::endl;
		outfile.close();
	}
	void log(std::string message, std::string logName) {

		auto now = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(now);

		std::ofstream outfile;
		outfile.open((std::string)LOGSPATH + (std::string)this->fileTimeName + (std::string)logName + (std::string)LOGEXTENSION, std::ios::app);
		char* ctimeBuf = std::ctime(&end_time);
		// replace endl with \0
		if (ctimeBuf[strlen(ctimeBuf) - 1] == '\n') ctimeBuf[strlen(ctimeBuf) - 1] = '\0';
		outfile << ctimeBuf << "\t" << message << std::endl;
		outfile.close();
	}
	
};