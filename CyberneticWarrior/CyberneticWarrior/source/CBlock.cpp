#include "PrecompiledHeader.h"

#include "CBlock.h"
//#include "CCamera.h"

CBlock::CBlock(int nBlockType, float fPosX, float fPosY, int nWidth, int nHeight)
{
	m_nBlockType = nBlockType;
	SetPosX(fPosX);
	SetPosY(fPosY);
	SetWidth(nWidth);
	SetHeight(nHeight);
}

CBlock::~CBlock(void)
{
}

void CBlock::Update(float fElapsedTime)
{
}

void CBlock::Render(void)
{
}	

RECT CBlock::GetRect(void)
{
	RECT rBlock;
	rBlock.left = (LONG)GetPosX();
	rBlock.top = (LONG)GetPosY();
	rBlock.right = rBlock.left + (LONG)GetWidth();
	rBlock.bottom = rBlock.top + (LONG)GetHeight();

	return rBlock;
}

bool CBlock::CheckCollision(CBase* pBase)
{
	RECT rIntersect;
	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}