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
	SocketManager();
	~SocketManager();

	SOCKET listenSocket;

	static SocketManager* pInstance;

public:
	static void Initialize();
	static SocketManager* Get() {
		return pInstance;
	}


	SOCKET Accept();
	void Read();




};
