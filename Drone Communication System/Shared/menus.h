#pragma once
#include <iostream>


void accept_reject_menu();
void drone_connect_menu();
void server_main_menu(std::string towerId, std::string droneId);
void drone_main_menu(std::string towerId, std::string droneId);


void drone_connect_menu() {
	std::system("cls");
	std::cout << "Listening for connections...\n";
	std::cout << "Welcome to Next Level Drone Systems\n";
	std::cout << "1. Connect\n";
	std::cout << "2. Check Connections\n";
	std::cout << "3. Exit\n";
}

void server_main_menu(std::string towerId, std::string droneId) {
	std::system("cls");
	std::cout << "Tower ID: " << towerId << "\n";
	std::cout << "Connected: " << droneId << "\n";
	std::cout << "Waiting for command...\n";
	std::cout << "1. Open Chat\n";
	std::cout << "2. Take Picture\n";
	std::cout << "3. Disconnect\n";
}

void accept_reject_menu() {
	std::system("cls");
	std::cout << "Client connected\n";
	std::cout << "1. Accept\n";
	std::cout << "2. Reject\n";
}

void drone_main_menu(std::string towerId, std::string droneId) {
	std::system("cls");
	std::cout << "Drone ID: " << droneId << "\n";
	std::cout << "Connected: " << towerId << "\n";
	std::cout << "Waiting for command...\n";
	std::cout << "1. Open Chat\n";
	std::cout << "2. Take & Send Picture\n";
	std::cout << "3. Disconnect\n";
}