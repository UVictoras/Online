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

    Player* m_pPlayer = new Player('x', " ", 1);
    m_pPlayers.push_back(m_pPlayer);

    m_pPlayer = new Player('o', " " , 2);
    m_pPlayers.push_back(m_pPlayer);

    m_Grid = { 0,0,0,0,0,0,0,0,0 };
    m_jServ["Grid"] = m_Grid;
}

void GameManager::AssignPlayer(json jClient, SOCKET* sSock) {
    if (m_pPlayers[0]->m_sSock == NULL) {
        m_pPlayers[0]->m_sName = jClient["Name"];
        m_pPlayers[0]->m_sSock = *sSock;
    }
    else if (m_pPlayers[1]->m_sSock == NULL) {
        m_pPlayers[1]->m_sName = jClient["Name"];
        m_pPlayers[1]->m_sSock = *sSock;
    }
}

void GameManager::PlaceSign(json jClient) {
    if (m_Grid[jClient["Cell"]] == 0) {
        if (m_pPlayers[m_iTurn]->m_sId == jClient["Id"])
        {
            m_Grid[jClient["Cell"]] = m_pPlayers[m_iTurn]->m_sSign;
            ChangeTurn();
            SendJSON(true, true);
            return;
        }
    }
    SendJSON(true, false);
}

bool GameManager::GameReady() {
    for (Player* pPlayer : m_pPlayers) {
        if (pPlayer->m_sSock == NULL) {
            SendJSON(false, false);
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
        std::string jtext = m_jServ.dump() + "\n";
        // send json to server
        int bytesSent = send(pPlayer->m_sSock, jtext.c_str(), strlen(jtext.c_str()), 0);
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

void GameManager::ChangeTurn() {
    if (m_iTurn)
        m_iTurn = 0;
    else
        m_iTurn = 1;
}

void GameManager::CheckWin()
{
    // VERIFICATION COLUMN
    for (int x = 0; x < 3; x++) {
        if (m_Grid[x] != 0 and m_Grid[x + 3] != 0 and m_Grid[x + 6] != 0) {
            if (m_Grid[x] == m_Grid[x + 3] and m_Grid[x + 6] == m_Grid[x]) {
                if (m_Grid[x] == m_pPlayers[0]->m_sSign)
                    m_pPlayers[0]->m_sWin = true;
                else
                    m_pPlayers[1]->m_sWin = true;
            }
        }
    }

    // VERIFICATION LINE
    for (int y = 0; y < 3; y++) {
        if (m_Grid[y * 3] != 0 and m_Grid[y * 3 + 1] != 0 and m_Grid[y * 3 + 2] != 0) {
            if (m_Grid[y * 3] == m_Grid[y * 3 + 1] and m_Grid[y * 3 + 2] == m_Grid[y * 3]) {
                if (m_Grid[y] == m_pPlayers[0]->m_sSign)
                    m_pPlayers[0]->m_sWin = true;
                else
                    m_pPlayers[1]->m_sWin = true;
            }
        }

        // VERIFICATION DIAGONAL
        if (m_Grid[0] != 0 and m_Grid[4] != 0 and m_Grid[8] != 0) {
            if (m_Grid[0] == m_Grid[4] and m_Grid[8] == m_Grid[0]) {
                if (m_Grid[0] == m_pPlayers[0]->m_sSign)
                    m_pPlayers[0]->m_sWin = true;
                else
                    m_pPlayers[1]->m_sWin = true;
            }
        }
        if (m_Grid[2] != 0 and m_Grid[4] != 0 and m_Grid[6] != 0) {
            if (m_Grid[2] == m_Grid[4] and m_Grid[6] == m_Grid[2]) {
                if (m_Grid[3] == m_pPlayers[0]->m_sSign)
                    m_pPlayers[0]->m_sWin = true;
                else
                    m_pPlayers[1]->m_sWin = true;
            }
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