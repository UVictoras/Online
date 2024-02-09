#pragma once

class Player
{
public:
	char m_sName;
	char m_sSign;
	int m_sId;

	Player(char sSign, char sName, int sId);

	void AssignInfo(char sName);

	//void MakePlay(Case* cCase, int* iTurn, sf::Texture* tTextureX, sf::Texture* tTextureCircle);
};