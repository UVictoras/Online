// Serveur2.cpp : Définit le point d'entrée de l'application.
//

#include "GameManager.h"
#include "SocketManager.h"
#include "framework.h"
#include <iostream>
#include "Client.h"
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#pragma comment(lib, "ws2_32.lib")


int APIENTRY main(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	SocketManager::Initialize();
	EventManager::Initialize();
	SocketManager::Get()->Connect();
	GameManager::Get()->GetName();
	SocketManager::Get()->Read();

	return 0;
}