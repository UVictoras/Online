#pragma once
#include "GameObject.h"

class Case : public GameObject
{
public:
	bool m_bIsFull;

	int m_iIndex;

	char m_sSignFill;

	Case(bool bType, float fX, float fY, int iIndex, float fSizeL, float fSizeH, sf::Color cColor);

	void FillGrid(char sSign, sf::Texture* tTextureX, sf::Texture* tTextureCircle);
};