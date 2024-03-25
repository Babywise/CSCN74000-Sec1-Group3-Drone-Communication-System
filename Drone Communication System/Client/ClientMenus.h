#pragma once
#include <iostream>

void clientStartMenu(std::string droneId) {
    std::system("cls");
    std::cout << "(" << droneId << ") - Welcome to Next Level Drone Systems\n";
    std::cout << "1. Connect to Server\n";
    std::cout << "2. Check Connections\n";
    std::cout << "3. Exit\n";
}

void clientDroneMenu(std::string towerId, std::string droneId) {
    std::system("cls");
    std::cout << "Drone ID: " << droneId << "\n";
    std::cout << "Connected: " << towerId << "\n";
    std::cout << "Waiting for command...\n";
    std::cout << "1. Open Chat\n";
    std::cout << "2. Take & Send Picture\n";
    std::cout << "3. Disconnect\n";
}

void clientConnectionMenu() {
    std::system("cls");
    std::cout << "Server connected\n";
    std::cout << "1. Accept\n";
    std::cout << "2. Reject\n";
    std::cout << "3. Go Back\n";
}