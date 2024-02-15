#include "GameManagerS.h"
#include "Math.h"
#include <iostream>
#include <sstream>
#define NOMINMAX
#include <vector>
#include <winsock2.h>

using json = nlohmann::json;
using namespace std;
sf::Event pressed;

GameManager* GameManager::pInstance = nullptr;

/*
-----------------------------------------------------------------------
|   Following are the methods corresponding to the GameManager Class  |
-----------------------------------------------------------------------
*/

GameManager::GameManager()// Calling RenderWindow constructor for our game window
{
    m_bWon1 = false;
    m_bWon2 = false;
    m_bDraw = false;
    m_iTurn = 0;

    Player* m_pPlayer = new Player(1, " ", 1);
    m_pPlayers.push_back(m_pPlayer);

    m_pPlayer = new Player(2, " ", 2);
    m_pPlayers.push_back(m_pPlayer);

    m_Grid = { 0,0,0,0,0,0,0,0,0 };
    m_jServ["Grid"] = m_Grid;
    m_jClient["Name"] = " ";
}

void GameManager::AssignPlayer(SOCKET sSock) {
    if (m_pPlayers[0]->m_sSock == NULL && m_pPlayers[1]->m_sSock != sSock && m_jClient["Name"] != " ") {
        m_pPlayers[0]->m_sName = m_jClient["Name"];
        m_pPlayers[0]->m_sSock = sSock;
        m_pPlayers[0]->m_sId = 0;
        std::cout << "p1: " << m_pPlayers[0]->m_sName << " sock: " << m_pPlayers[0]->m_sSock << std::endl;
    }
    else if (m_pPlayers[1]->m_sSock == NULL && m_pPlayers[0]->m_sSock != sSock && m_jClient["Name"] != " " && m_pPlayers[0]->m_sSock != NULL) {
        m_pPlayers[1]->m_sName = m_jClient["Name"];
        m_pPlayers[1]->m_sSock = sSock;
        m_pPlayers[1]->m_sId = 1;
        std::cout << "p2: " << m_pPlayers[1]->m_sName << " sock: " << m_pPlayers[1]->m_sSock << std::endl;
    }
}

void GameManager::PlaceSign(json m_jClient) {
    if (m_jClient["Cell"] != -1) {
        if (m_Grid[m_jClient["Cell"]] == 0) {
            if (m_pPlayers[m_iTurn]->m_sId == m_jClient["Id"])
            {
                cout << m_jClient["Cell"] << endl;
                std::cout << "Serv Move TRUE" << std::endl;
                m_Grid[m_jClient["Cell"]] = m_pPlayers[m_iTurn]->m_sSign;
                ChangeTurn();
                SendJSON(true, true);
                return;
            }
        }
    }
    SendJSON(true, false);
}

bool GameManager::GameReady() {
    for (Player* pPlayer : m_pPlayers) {
        if (pPlayer->m_sSock == NULL) {
            return false;
        }
    }

    SendJSON(true, false);
    return true;
}

void GameManager::SendJSON(bool GameRunnig, bool ValidMove) {
    for (Player* pPlayer : m_pPlayers) {
        m_jServ["GameRunning"] = GameRunnig;
        m_jServ["ValidMove"] = ValidMove;
        m_jServ["PlayerTurn"] = (m_iTurn == pPlayer->m_sId) ? true : false;
        m_jServ["Grid"] = m_Grid;
        m_jServ["Id"] = pPlayer->m_sId;
        std::string jtext = m_jServ.dump(-1) + "\n";
        int jLen = sizeof(jtext);
        jtext = (char)jLen + jtext;
        // send json to server
        int bytesSent = send(pPlayer->m_sSock, jtext.c_str(), static_cast<int>(strlen(jtext.c_str())), 0);
        if (bytesSent == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
                closesocket(pPlayer->m_sSock);
                WSACleanup();
            }
        }
    }
}

void GameManager::GetJSON(json jClient) { m_jClient = jClient; }

void GameManager::ChangeTurn() {
    if (m_iTurn)
        m_iTurn = 0;
    else
        m_iTurn = 1;
}

void GameManager::CheckWin() {
    // VERIFICATION COLUMN
    for (int x = 0; x < 3; x++) {
        CheckAndSetWinner(m_Grid[x], m_Grid[x + 3], m_Grid[x + 6]);
    }

    // VERIFICATION LINE
    for (int y = 0; y < 3; y++) {
        CheckAndSetWinner(m_Grid[y * 3], m_Grid[y * 3 + 1], m_Grid[y * 3 + 2]);
    }

    // VERIFICATION DIAGONAL
    CheckAndSetWinner(m_Grid[0], m_Grid[4], m_Grid[8]);
    CheckAndSetWinner(m_Grid[2], m_Grid[4], m_Grid[6]);
}

void GameManager::CheckAndSetWinner(int a, int b, int c) {
    if (a != 0 and b != 0 and c != 0) {
        if (a == b and c == a) {
            if (a == 1)
                m_pPlayers[0]->m_sWin = true;
            else
                m_pPlayers[1]->m_sWin = true;
        }
    }
}

void GameManager::CheckDraw()
{
    for (int i : m_Grid)
    {
        if (i == 0) {
            m_bDraw = false;
            return;
        }

    }
    m_bDraw = true;
}

void GameManager::GameLoop() {
    while (m_pPlayers[0]->m_sWin == false and m_pPlayers[1]->m_sWin == false) {

    }
}