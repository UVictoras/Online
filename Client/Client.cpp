// Client.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Client.h"
#include <winsock2.h>
#include <iostream>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

#define MAX_LOADSTRING 100
#define PORT 82

// Variables globales :
HINSTANCE hInst;                                // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// MessageBox for variables
#define MSGBOX(x) \
{ \
   std::ostringstream oss; \
   oss << x; \
   MessageBox(oss.str().c_str(), "Msg Title", MB_OK | MB_ICONQUESTION); \
} // ex: MSGBOX("text"<<variable);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    HWND hWnd = InitInstance(hInstance, nCmdShow);

    // Initialisation de Winsock
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsaData) != 0) {
        return 1;
    }

    // Création du socket pour écouter les connexions entrantes
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Liaison du socket à l'adresse locale et au port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.83.128"); // Écoute sur toutes les interfaces locales
    serverAddr.sin_port = htons(PORT); // Port d'écoute

    if (WSAConnect(sock, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr), nullptr, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            closesocket(sock);
            DestroyWindow(hWnd);
            WSACleanup();
            MessageBox(hWnd,L"La connection au serveur a echoue.", L"Erreur de connection", MB_OK | MB_ICONERROR);
            return 1;
        }
    }
    else {
        EventManager::Initialize();
        GameManager::Initialize(); //Initializing GameManager's singleton instance
      

        GameManager::Get()->GameLoop();
        std::string textj = GameManager::Get()->GetJson();
        MessageBox(hWnd, L"Got JSON", L"HI", MB_OK | MB_ICONERROR);


        //MessageBoxW(hWnd, ("JSON to send: "+textj).c_str(), MB_OK | MB_ICONERROR);

        WSAAsyncSelect(sock, hWnd, WM_USER + 1, FD_READ | FD_CLOSE);

        const char* message = "Hello, server!";
        int bytesSent = send(sock, message, strlen(message), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                closesocket(sock);
                WSACleanup();
                DestroyWindow(hWnd);
                MessageBox(hWnd, L"Send error.", L"Error", MB_OK | MB_ICONERROR);
                return 1;
            }
            return 0;
        }

        // send json to server
       /* bytesSent = send(sock, textj.c_str(), strlen(textj.c_str()), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                closesocket(sock);
                WSACleanup();
                DestroyWindow(hWnd);
                MessageBox(hWnd, L"Send error.", L"Error", MB_OK | MB_ICONERROR);
                return 1;
            }
            return 0;
        }*/

        MSG msg;

        while (true)
        {
            // Boucle de messages principale :
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {   
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            //Logic
            //Render
        }
        // Cleanup
        closesocket(sock);
        WSACleanup();
        DestroyWindow(hWnd);
        return (int)msg.wParam;
    }
}


//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Window";
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans une variable globale, puis
//        nous créons et affichons la fenêtre principale du programme.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Stocke le handle d'instance dans la variable globale

    return CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_USER + 1:
        {
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_READ:
                {
                    break;
                }
                //case FD_WRITE:
                //{
                //    sInfoSocket = wParam;
                //    // WSASend(sInfoSocket, &bBuffer, 1, &wBytes, wFlags, NULL, NULL)
                //    // send(sInfoSocket, message, strlen(message), 0)
                //    const char* message = "Hello, server!";
                //    MessageBox(hWnd, L"Message Sent", L"Notification", MB_OK | MB_ICONINFORMATION);

                //    if (WSASend(sInfoSocket, &bBuffer, 1, &wBytes, wFlags, NULL, NULL) == SOCKET_ERROR)
                //    {
                //        if (WSAGetLastError() != WSAEWOULDBLOCK)
                //        {
                //            closesocket(sInfoSocket);
                //            return 0;
                //        }
                //    }
                //    else
                //    {
                //        MessageBox(hWnd, L"Message Sent", L"Notification", MB_OK | MB_ICONINFORMATION);
                //    }
                //    break;
                //}
                case FD_CLOSE:
                {
                    break;
                }
            }
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
