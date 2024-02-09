#pragma once
#include "EventManager.h"
#include "Case.h"
#include "Player.h"
#include "../json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
typedef void(*func)();
using namespace std;

class GameManager
{
private:
	static GameManager* pInstance;

	sf::Vector2i vLocalPosition;

	sf::RenderWindow oWindow;

	bool m_bWon1, m_bWon2, m_bDraw;

	json grid ; 

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

	Player* m_pPlayers[2];

	Case* m_cCasesList[9];

	GameObject* m_gCasesBack[9];

	GameObject* m_rBackground;

	sf::Texture* m_tTextureBlank;
	sf::Texture* m_tTextureX;
	sf::Texture* m_tTextureCircle;

	int m_iTurn;

	GameManager();

	void CreateGrid();

	void CreateSign();

	void CheckWin();

	void CheckDraw();

	bool IsFullGrid();


	void GameLoop();

	//Events

	void CloseWindow();

	void PlaceSign();

	std::string GetJson();
};