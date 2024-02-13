#pragma once
#include "GameManagerS.h"
#include "framework.h"
#include "Serveur2.h"
#include <iostream>
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#pragma comment(lib, "ws2_32.lib")

class SocketManager
{
private:

	~SocketManager();

	SOCKET listenSocket;

	static SocketManager* sInstance;

public:
	std::vector<SOCKET*> vSockets;

    static void Initialize();
	static SocketManager* Get() {
		return sInstance;
	}
	static SOCKET GetSocket()
	{
		return SocketManager::Get()->listenSocket;
	}

	SocketManager();

	static ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Accept();
	void Read();

};

