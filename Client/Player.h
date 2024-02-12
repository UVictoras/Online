#pragma once
#include <string>

class Player
{
public:
	std::string m_sName;
	int m_iId;

	Player();

	void ChangeName(std::string sName);
};