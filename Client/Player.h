#pragma once
#include "Case.h";

class Player
{
public:
	char m_sSign;

	Player(char sSign);

	void MakePlay(Case* cCase, int* iTurn, sf::Texture* tTextureX, sf::Texture* tTextureCircle);
};