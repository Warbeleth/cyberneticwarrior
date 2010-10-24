#include "PrecompiledHeader.h"

#include "CBlock.h"
//#include "CCamera.h"

CBlock::CBlock(void)
{
	this->SetPosX(0.0f);
	this->SetPosY(0.0f);
	this->SetWidth(0);
	this->SetHeight(0);
	this->SetBlockType(BLOCK_SOLID);
	this->m_vStartingPosition.fX = 0.0f;
	this->m_vStartingPosition.fY = 0.0f;
	this->SetType(OBJ_BLOCK);
}

CBlock::~CBlock(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
}

void CBlock::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

}

void CBlock::Render(void)
{
	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), (int)this->GetPosX(), (int)this->GetPosY());
}	

RECT CBlock::GetRect(void) const
{
	RECT rBlock;
	rBlock.top = (LONG)this->GetPosX();
	rBlock.left = (LONG)this->GetPosY();
	rBlock.right = rBlock.top + (LONG)this->GetWidth();
	rBlock.bottom = rBlock.left + (LONG)this->GetHeight();

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

int	CBlock::GetBlockType(void) {return this->m_nBlockType;}

void CBlock::SetBlockType(int nBlockType) {this->m_nBlockType = nBlockType;}

//tVector2D	CBlock::GetStartingPos(void)	{return this->m_vStartingPosition;}
//
//void	CBlock::SetStartingPos(tVector2D vStartingPos) { this->m_vStartingPosition = vStartingPos; }