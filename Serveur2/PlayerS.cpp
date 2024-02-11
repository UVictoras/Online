#include "PlayerS.h"

Player::Player(char sSign, std::string sName, int sId)
{
	m_sSign = sSign;
	m_sName = sName;
	m_sId = sId;
	m_sWin = false;
	m_sSock = NULL;
}

void Player::AssignInfo(std::string sName, SOCKET sSock) {
	m_sName = sName;
	m_sSock = sSock;
}