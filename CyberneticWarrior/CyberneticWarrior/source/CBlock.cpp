#include "PrecompiledHeader.h"

#include "CGame.h"
#include "CBlock.h"
#include "CPlayer.h"
#include "CObjectManager.h"

CBlock::CBlock(int nBlockType, float fPosX, float fPosY, int nWidth, int nHeight)
{
	m_nBlockType = nBlockType;
	SetPosX(fPosX);
	SetPosY(fPosY);
	SetWidth(nWidth);
	SetHeight(nHeight);
	m_fTimeWaited = 0;
	m_fSpeed = 75;
	m_bStable = true;
}

CBlock::~CBlock(void)
{
}

void CBlock::Update(float fElapsedTime)
{
	m_fTimeWaited += fElapsedTime;

	if(m_fTimeWaited >= 7 && GetBlock()==BLOCK_MOVING)
	{
		m_fTimeWaited = 0;
		m_fSpeed = -m_fSpeed;
	}

	if(GetBlock()==BLOCK_MOVING)
	{
		SetPosX(GetPosX() + m_fSpeed*fElapsedTime);
	}
	else if(GetBlock()==BLOCK_UNSTABLE && m_bStable == false)
	{
		SetPosY(GetPosY() + m_fSpeed*fElapsedTime);

		if(GetPosY() > CCamera::GetInstance()->GetOffsetY()+2000)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyBlockMessage(this));
			//CObjectManager::GetInstance()->RemoveObject(this);
		}
	}
}

void CBlock::Render(void)
{
	int left = (int)GetPosX() - CCamera::GetInstance()->GetOffsetX();
	int top = (int)GetPosY() - CCamera::GetInstance()->GetOffsetY();
	int right = left + GetWidth();
	int bottom = top + GetHeight();

	CSGD_Direct3D::GetInstance()->DrawLine(left, top, right, top, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, bottom, right, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, top, left, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(right, top, right, bottom, 255, 0, 0);
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

	if(pBase == this)
		return false;

	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{
		return true;
	}
	else
	{
		return false;
	}
}