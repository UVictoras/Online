#include "Player.h"

Player::Player()
{
	m_sName = " ";
	m_iId = -1;
}

void Player::ChangeName(std::string sName) {
	m_sName = sName;
}