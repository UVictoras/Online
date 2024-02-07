// Serveur2.cpp : Définit le point d'entrée de l'application.
//

#include "framework.h"
#include "Serveur2.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;     
HWND hWnd;// instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    hWnd = InitInstance(hInstance, nCmdShow);

    if (hWnd == NULL) {
        return 1;
    }
    // Initialisation de Winsock
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsaData) != 0) {
        return 1;
    }

    // Création du socket pour écouter les connexions entrantes
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    WSAAsyncSelect(listenSocket, hWnd, WM_USER + 1, FD_ACCEPT);

    // Liaison du socket à l'adresse locale et au port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Écoute sur toutes les interfaces locales
    serverAddr.sin_port = htons(82); // Port d'écoute
    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Mettre le socket en mode écoute
    if (listen(listenSocket, 0) == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor        = NULL;
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = L"Window";
    wcex.hIconSm        = NULL; 

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

    HWND hWnd = CreateWindowW(L"Window", L"", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    return hWnd;
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
    SOCKET sAccept, sInfoSocket;
    WSABUF bBuffer;
    char cBufferData[1024];
    bBuffer.buf  = cBufferData;
    bBuffer.len = 1024;
    DWORD wBytes, wFlags = 0;

    switch (message)
    {
        case WM_USER + 1:
        {
            switch (WSAGETSELECTEVENT(lParam))
            {
                case FD_ACCEPT:
                {
                    sAccept = accept(wParam, NULL, NULL);
                    MessageBox(hWnd, L"New connection", L"Notification", MB_OK | MB_ICONINFORMATION);
                    WSAAsyncSelect(sAccept, hWnd, WM_USER + 1, FD_READ | FD_WRITE | FD_CLOSE);
                    break;
                }
                case FD_READ:
                {
                    sInfoSocket = wParam;

                    if (WSARecv(sInfoSocket, &bBuffer, 1, &wBytes, &wFlags, NULL, NULL) == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() != WSAEWOULDBLOCK)
                        {
                            closesocket(sInfoSocket);
                            return 0;
                        }
                    }
                    else 
                    {
                        MessageBox(hWnd, L"New notification", L"Notification", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
                case FD_WRITE:
                {
                    sInfoSocket = wParam;

                    if (WSASend(sInfoSocket, &bBuffer, 1, &wBytes, wFlags, NULL, NULL) == SOCKET_ERROR)
                    {
                        if (WSAGetLastError() != WSAEWOULDBLOCK)
                        {
                            closesocket(sInfoSocket);
                            return 0;
                        }
                    }
                    else
                    {
                        MessageBox(hWnd, L"Message Sent", L"Notification", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
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