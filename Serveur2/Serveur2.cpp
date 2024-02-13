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
    SocketManager::Get()->Accept();
    SocketManager::Get()->Read();

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int) msg.wParam;
}

