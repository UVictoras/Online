#pragma once

#include <winsock2.h>
#include <string>

class Player
{
public:
	std::string m_sName;
	char m_sSign;
	int m_sId;
	bool m_sWin;
	SOCKET m_sSock;

	Player(char sSign, std::string sName, int sId);

	void AssignInfo(std::string sName, SOCKET sSock);

	//void MakePlay(Case* cCase, int* iTurn, sf::Texture* tTextureX, sf::Texture* tTextureCircle);
};