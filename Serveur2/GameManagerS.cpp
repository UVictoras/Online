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

    m_Grid.insert(m_Grid.end(), { 0,0,0,0,0,0,0,0,0 });
}

void GameManager::AssignPlayer() {
    //Récupere le premier JSON envoyer par le joueur, et assigne son nom
}

void GameManager::PlaceSign(int jIndex, int jId) {
    if (m_Grid[jIndex] == 0) {
        if (m_pPlayers[m_iTurn]->m_sId == jId)
        {
            m_Grid[jIndex] = m_pPlayers[m_iTurn]->m_sSign;
            ChangeTurn();
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