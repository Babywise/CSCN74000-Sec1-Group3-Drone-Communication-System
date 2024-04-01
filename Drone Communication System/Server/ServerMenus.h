#pragma once
#include <iostream>

// Start Menu for Server
void serverStartMenu(std::string towerId) {
    std::system("cls");
    std::cout << "(" << towerId << ") - Welcome to Next Level Drone Systems\n";
    std::cout << "1. Connect to Drone\n";
    std::cout << "2. Check Connections\n";
    std::cout << "3. Exit\n";
}

// Drone Menu for Server
void serverDroneMenu(std::string towerId, std::string droneId) {
    std::system("cls");
    std::cout << "Tower ID: " << towerId << "\n";
    std::cout << "Connected: " << droneId << "\n";
    std::cout << "Waiting for command...\n";
    std::cout << "1. Open Chat\n";
    std::cout << "2. Take Picture\n";
    std::cout << "3. Disconnect\n";
}

// Connection Menu for Server
void serverConnectionMenu() {
    std::system("cls");
    std::cout << "Client connected\n";
    std::cout << "1. Accept\n";
    std::cout << "2. Reject\n";
    std::cout << "3. Go Back\n";
}