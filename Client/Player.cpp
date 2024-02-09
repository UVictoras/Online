#include "Player.h"

Player::Player(char sSign)
{
	m_sSign = sSign;
}

void Player::MakePlay(Case* cCase, int* iTurn, sf::Texture* tTextureX, sf::Texture* tTextureCircle)
{
	if (cCase->m_bIsFull == true)
		return;

	cCase->FillGrid(m_sSign, tTextureX, tTextureCircle);
	iTurn++;
}