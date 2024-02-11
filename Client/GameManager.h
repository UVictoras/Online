#pragma once
#include "EventManager.h"
#include "Case.h"
#include "Player.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
#include <winsock2.h>

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

	static void Initialize(SOCKET* input)
	{
		GameManager::pInstance = new GameManager(input);
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

	GameManager(SOCKET* input);

	void CreateGrid();

	void CreateSign();

	void CheckWin();

	void CheckDraw();

	bool IsFullGrid();


	void GameLoop(SOCKET sock,HWND hWnd);

	//Events

	void CloseWindow();

	void PlaceSign();
};