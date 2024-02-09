#include "GameManagerS.h"
#include "Math.h"
#include "PlayerS.h"
#include <iostream>
#include <sstream>

#include <winsock2.h>
#include <vector>


using json = nlohmann::json;
using namespace std;
sf::Event pressed;

GameManager* GameManager::pInstance = nullptr;

/*
-----------------------------------------------------------------------
|      Following are the functions used in the EventManager maps      |
-----------------------------------------------------------------------
*/


void EventPlaceSign()
{
    GameManager::Get()->PlaceSign();
}

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

    Player* m_pPlayer = new Player('x',' ',1);
    m_pPlayers.push_back(m_pPlayer);

    m_pPlayer = new Player('o',' ',2);
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
        if (m_gCasesBack[y * 3] != nullptr and m_gCasesBack[y * 3 + 1] != nullptr and m_gCasesBack[y * 3 + 2] != nullptr) {
            if (m_gCasesBack[y * 3]->m_bType == m_gCasesBack[y * 3 + 1]->m_bType and m_gCasesBack[y * 3 + 2]->m_bType == m_gCasesBack[y * 3]->m_bType) {
                if (m_gCasesBack[y * 3]->m_bType == true)
                    m_bWon1 = true;
                else
                    m_bWon2 = true;
            }
        }
   }

    // VERIFICATION DIAGONAL
    if (m_gCasesBack[0] != nullptr and m_gCasesBack[4] != nullptr and m_gCasesBack[8] != nullptr) {
        if (m_gCasesBack[0]->m_bType == m_gCasesBack[4]->m_bType and m_gCasesBack[8]->m_bType == m_gCasesBack[0]->m_bType) {
            if (m_gCasesBack[0]->m_bType == true)
                m_bWon1 = true;
            else
                m_bWon2 = true;
        }
    }
    if (m_gCasesBack[2] != nullptr and m_gCasesBack[4] != nullptr and m_gCasesBack[6] != nullptr) {
        if (m_gCasesBack[2]->m_bType == m_gCasesBack[4]->m_bType and m_gCasesBack[6]->m_bType == m_gCasesBack[2]->m_bType) {
            if (m_gCasesBack[2]->m_bType == true)
                m_bWon1 = true;
            else
                m_bWon2 = true;
        }
    }

}

void GameManager::CheckDraw()
{
    for (GameObject* cCase : m_gCasesBack)
    {
        if (cCase == nullptr) {
           m_bDraw = false;
           return;
        }

    }
    m_bDraw = true;
}