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


    static ATOM MyRegisterClass(HINSTANCE hInstance);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void Accept();
    void Read();

};
// Serveur2.cpp : D�finit le point d'entr�e de l'application.
//

#include "GameManagerS.h"
#include "SocketManager.h"
#include "framework.h"
#include <iostream>
#include "Serveur2.h"
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"
#pragma comment(lib, "ws2_32.lib")