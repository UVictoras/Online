// Serveur2.cpp : Définit le point d'entrée de l'application.
//

#include "GameManagerS.h"
#include "SocketManager.h"
#include "framework.h"
#include <iostream>
#include "Serveur2.h"
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#pragma comment(lib, "ws2_32.lib")


int main()
{
	SocketManager::Initialize();
	GameManager::Initialize();
	SocketManager::Get()->Accept();
	SocketManager::Get()->Read();


	return 0;
}