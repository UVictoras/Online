#include "SocketManager.h"

#include "framework.h"
#pragma comment(lib, "ws2_32.lib")


#define PORT 82
#define MAX_LOADSTRING 100
#define BUFFER_SIZE 1024 // Taille du tampon de lecture

using json = nlohmann::json;

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


void SocketManager::Initialize() {
	SocketManager::pInstance = new SocketManager();

    HINSTANCE hInstance = GetModuleHandleA(0);
    GameManager::Initialize();
    // Initialise les chaînes globales
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    hWnd = InitInstance(hInstance, 0);

    if (hWnd == NULL) {
        return ;
    }
    // Initialisation de Winsock
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsaData) != 0) {
        MessageBox(hWnd, L"WSAStartup error", L"Error", MB_OK | MB_ICONERROR);
        return ;
    }
}
