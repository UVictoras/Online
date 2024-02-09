#include "Case.h"

Case::Case(bool bType, float fX, float fY, int iIndex, float fSizeL, float fSizeH, sf::Color cColor, sf::Texture* tTexture) : GameObject(bType, fX, fY, fSizeL, fSizeH, cColor)
{
	m_bIsFull = false;
	m_iIndex = iIndex;
	m_sSignFill = ' ';

	m_sGraphism->setTexture(tTexture);
}

void Case::FillGrid(char sSign, sf::Texture* tTextureX, sf::Texture* tTextureCircle)
{
	m_bIsFull = true;
	m_sSignFill = sSign;

	if (m_sSignFill == 'x')
	{
		m_sGraphism->setTexture(tTextureX);
	}
	else
	{
		m_sGraphism->setTexture(tTextureCircle);
	}
}