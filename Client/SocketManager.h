#pragma once
#include "GameManager.h"
#include "framework.h"
#include "Client.h"
#include <iostream>
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <codecvt>
#include <locale>

class SocketManager
{
private:

	~SocketManager();

	SOCKET listenSocket;

	static SocketManager* sInstance;

public:

	SOCKET sSocket;

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

	void AssignSocket(SOCKET sock);
	void Connect();
	void Read();

};

