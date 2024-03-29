#pragma once
#include "EventManager.h"
#include "Case.h"
#include "Player.h"
#include "../json-develop/single_include/nlohmann/json.hpp"
#define NOMINMAX
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

	bool m_bWon1, m_bWon2, m_bDraw, GameRunning;

	json m_jClient;

public:

	json m_jServ;

	SOCKET sock;

	static void Initialize(SOCKET sSock)
	{
		GameManager::pInstance = new GameManager(sSock);
	}

	static GameManager* Get()
	{
		return pInstance;
	}

public:

	bool IsEventInit;

	Player* m_pPlayers;

	Case* m_cCasesList[9];

	GameObject* m_gCasesBack[9];

	GameObject* m_rBackground;

	sf::Texture* m_tTextureBlank;
	sf::Texture* m_tTextureX;
	sf::Texture* m_tTextureCircle;

	int m_iTurn;

	GameManager(SOCKET sSock);

	void GetJSON(json jServ);

	void SendJSON(int cell);

	void CreateGrid();

	void CreateSign();

	void CheckWin();

	bool CheckLine(int start, int step);

	void CheckDraw();

	bool IsFullGrid();

	// FAUT CREER LA FONCTION POUR RECUP LA REPONSE DU SERV, ET ASSIGNER L'ID DU JOUEUR
	// CREER LA FONCTION POUR RECUPERER LA REPONSE DU SERV APRES AVOIR CLIQUER ET SI LE COUP EST VALIDE ON AFFICHE
	void GetName();

	void GameLoop();

	void InitGameEvent();

	//Events

	void CloseWindow();

	void PlaceSign();

	void UpdateGrid(json servJSON);

};