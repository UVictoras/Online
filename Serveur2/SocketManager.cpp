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
    vSockets.push_back(sock);
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
        case FD_ACCEPT:
        {
            sAccept = accept(wParam, NULL, NULL);
            std::cout << "New connection on socket " << sAccept << std::endl;
            //MessageBox(hWnd, L"New connection", L"Notification", MB_OK | MB_ICONINFORMATION);
            WSAAsyncSelect(sAccept, hWnd, WM_USER + 1, FD_READ | FD_CLOSE);
            break;
        }
        case FD_READ:
        {
            sInfoSocket = wParam;
            if (WSARecv(sInfoSocket, &bBuffer, 1, &wBytes, &wFlags, NULL, NULL) == SOCKET_ERROR)
            {
                int errorCode = WSAGetLastError();
                if (errorCode != WSAEWOULDBLOCK)
                {
                    std::cerr << "WSARecv failed with error code: " << errorCode << std::endl;
                    closesocket(sInfoSocket);
                    return 0;
                }
            }
            else
            {
                if (bBuffer.buf[0] != NULL) {
                    int i = 0;
                    std::string message;
                    while (bBuffer.buf[i] != '\n' && i < static_cast<int>(bBuffer.len)) {
                        if (bBuffer.buf[i] != '(')
                            message += bBuffer.buf[i];
                        i++;
                    }
                    try {
                        json j = json::parse(message);
                        // Handle the parsed JSON data
                        bBuffer.buf = nullptr;
                        std::fill(std::begin(cBufferData), std::end(cBufferData), 0);
                        bBuffer.buf = cBufferData;
                        GameManager::Get()->GetJSON(j);
                    }
                    catch (const nlohmann::json::exception& e) {
                        std::cout << "JSON parsing error: " << e.what() << std::endl;
                        // Handle the error (e.g., log it, close the socket, etc.)
                        closesocket(sInfoSocket);
                        return 0;
                    }

                    SocketManager::Get()->AssignSocket(sInfoSocket);
                }
            }
            break;
        }
        case FD_CLOSE:
        {
            break;
        }
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

void SocketManager::Accept() {
    // Cr�ation du socket pour �couter les connexions entrantes
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        WSACleanup();
        MessageBox(hWnd, L"listenSocket is invalid", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Liaison du socket � l'adresse locale et au port
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // �coute sur toutes les interfaces locales
    serverAddr.sin_port = htons(PORT); // Port d'�coute
    if (bind(listenSocket, (const sockaddr*)&serverAddr, (int)sizeof(serverAddr)) == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        closesocket(listenSocket);
        WSACleanup();
        MessageBox(hWnd, L"Couldn't link listenSocket to serverAddr. Error code: " + errorCode, L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    // Mettre le socket en mode �coute
    if (listen(listenSocket, 0) == SOCKET_ERROR) {
        closesocket(listenSocket);
        WSACleanup();
        MessageBox(hWnd, L"Couldn't listen on listenSocket", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    WSAAsyncSelect(listenSocket, hWnd, WM_USER + 1, FD_ACCEPT);
}

void SocketManager::Read() {
    MSG msg;
    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (GameManager::Get()->GameReady() == false) {
            for (SOCKET sock : vSockets)
                GameManager::Get()->AssignPlayer(sock);
        }
        if (GameManager::Get()->GameReady() == true) {
            //std::cout << "GameServeur" << GameManager::Get()->m_jClient["Cell"] << std::endl;
            GameManager::Get()->PlaceSign(GameManager::Get()->m_jClient);
        }
    }
}

SocketManager::~SocketManager() {
}

SocketManager::SocketManager()
{
    listenSocket = NULL;
}