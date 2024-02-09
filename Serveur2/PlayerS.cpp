#include "PlayerS.h"

Player::Player(char sSign, char sName, int sId)
{
	m_sSign = sSign;
	m_sName = sName;
	m_sId = sId;
	m_sWin = false;
}

void Player::AssignInfo(char sName) {
	m_sName = sName;
}