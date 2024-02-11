#pragma once
#include "Case.h";

class Player
{
public:
	char m_sName;
	int m_iId;

	Player();

	void MakePlay(Case* cCase, int* iTurn, sf::Texture* tTextureX, sf::Texture* tTextureCircle);
};