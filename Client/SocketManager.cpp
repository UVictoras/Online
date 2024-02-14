#include "SocketManager.h"

#include "framework.h"
#pragma comment(lib, "ws2_32.lib")

#define PORT 99
#define MAX_LOADSTRING 100
#define BUFFER_SIZE 1024 // Taille du tampon de lecture

using json = nlohmann::json;

// Variables globales�:
HINSTANCE hInst;
HWND hWnd;// instance actuelle
WCHAR szTitle[MAX_LOADSTRING];                  // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING];            // nom de la classe de fen�tre principale

// D�clarations anticip�es des fonctions incluses dans ce module de code�:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

SocketManager* SocketManager::sInstance = nullptr;

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Stocke le handle d'instance dans la variable globale

    HWND hWnd = CreateWindowW(L"Window", L"", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    return hWnd;
}

void SocketManager::Initialize() {
    SocketManager::sInstance = new SocketManager();

    HINSTANCE hInstance = GetModuleHandleA(0);
    GameManager::Initialize();
    // Initialise les cha�nes globales
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application�:
    hWnd = InitInstance(hInstance, 0);

    if (hWnd == NULL) {
        return;
    }
    // Initialisation de Winsock
    WSAData wsaData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsaData) != 0) {
        MessageBox(hWnd, L"WSAStartup error", L"Error", MB_OK | MB_ICONERROR);
        return;
    }
}

ATOM SocketManager::MyRegisterClass(HINSTANCE hInstance) {
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

void SocketManager::AssignSocket(SOCKET sock) {
    sSocket = sock;
}

LRESULT CALLBACK SocketManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    SOCKET sAccept, sInfoSocket;
    WSABUF bBuffer;
    char cBufferData[1024];
    bBuffer.buf = cBufferData;
    bBuffer.len = 1024;
    DWORD wBytes, wFlags = 0;
    switch (message)
    {
    case WM_USER + 1:
    {
        switch (WSAGETSELECTEVENT(lParam))
        {
        case FD_READ:
        {
            sInfoSocket = wParam;
            if (WSARecv(sInfoSocket, &bBuffer, 1, &wBytes, &wFlags, NULL, NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    MessageBox(hWnd, L"Notification failed", L"Notification", MB_OK | MB_ICONINFORMATION);
                    closesocket(sInfoSocket);
                    return 0;
                }
            }
            else
            {
                // turn buffer message into json
                if (bBuffer.buf[0] != NULL) {
                    int i = 0;
                    std::string message;
                    while (bBuffer.buf[i] != '\n' && i < static_cast<int>(bBuffer.len)) {
                        message += bBuffer.buf[i];
                        i++;
                    }
                    json j = json::parse(message);
                    bBuffer.buf = nullptr;
                    bBuffer.buf = cBufferData;
                    GameManager::Get()->GetJSON(j);
                }
            }
            break;
        }
        case FD_CLOSE:
        {
            break;
        }
        break;
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
    }
}


void SocketManager::Connect() {
    // Création du socket pour écouter les connexions entrantes
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return ;
    }

    // Liaison du socket à l'adresse locale et au port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // serverAddr.sin_addr.s_addr = inet_addr("10.1.144.36"); // Écoute sur toutes les interfaces locales
     //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Écoute sur toutes les interfaces locales
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr.s_addr);
    serverAddr.sin_port = htons(PORT); // Port d'écoute

    //if (WSAConnect(sock, (const sockaddr*)(&serverAddr), sizeof(serverAddr), nullptr, nullptr, nullptr, nullptr) == SOCKET_ERROR) {
    if (connect(sock, (const sockaddr*)(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            MessageBox(hWnd, L"Connection error", L"Error", MB_OK | MB_ICONQUESTION);
            closesocket(sock);
            DestroyWindow(hWnd);
            WSACleanup();
            return;
        }
    }
    WSAAsyncSelect(sock, hWnd, WM_USER + 1, FD_READ | FD_CLOSE);
}

void SocketManager::Read() {
    MSG msg;
    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (GameManager::Get()->m_jServ["GameReady"] == true) {
            std::cout << "UwU sussy baka desu" << std::endl;
        }
    }
}



SocketManager::~SocketManager() {
}

SocketManager::SocketManager()
{
    listenSocket = NULL;
}