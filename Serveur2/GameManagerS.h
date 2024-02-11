#pragma once
#include "PlayerS.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include "../json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
typedef void(*func)();
using namespace std;

class GameManager
{
private:
	static GameManager* pInstance;

	bool m_bWon1, m_bWon2, m_bDraw;
	std::vector<int> m_iGrid;
	json m_jServ;

public:

	static void Initialize()
	{
		GameManager::pInstance = new GameManager();
	}

	static GameManager* Get()
	{
		return pInstance;
	}

public:

	//Player* m_pPlayers[2];

	int m_iTurn;

	vector<Player*> m_pPlayers;

	vector<int> m_Grid;

	GameManager();

	void AssignPlayer(json jClient, SOCKET* sSock);

	void GameReady();

	void SendJSON(bool GameRunnig, bool ValidMove, bool PlayerTurn, Player* pPlayer);

	void ChangeTurn();

	void CheckWin();

	void CheckDraw();

	bool IsFullGrid();

	void GameLoop();

	//Events

	void CloseWindow();

	void PlaceSign(json jClient);

};