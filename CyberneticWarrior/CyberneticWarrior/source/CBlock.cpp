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
	m_fBasePosX = fPosX;
	m_fBasePosY = fPosY;

	SetWidth(nWidth);
	SetHeight(nHeight);
	m_fTimeWaited = 0;
	m_fSpeed = 75;
	m_bStable = true;
	m_fScaleX = 1.0f;
	m_fScaleY = 1.0f;
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

		if(GetPosY() > m_fBasePosY+500)
		{
			SetPosY(m_fBasePosY);
			SetStable(true);
			//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyBlockMessage(this));
		}
	}
}

void CBlock::Render(void)
{	
	if(GetImageID() != -1)
	{
		int OffsetX = CCamera::GetInstance()->GetOffsetX();
		int OffsetY = CCamera::GetInstance()->GetOffsetY();
		float CamScale = CCamera::GetInstance()->GetScale();
		int PosX = (int)((GetPosX()-OffsetX)*CamScale);
		int PosY = (int)((GetPosY()-OffsetY)*CamScale);

		CSGD_TextureManager* TM = CSGD_TextureManager::GetInstance();
		CSGD_Direct3D* D3D = CSGD_Direct3D::GetInstance();

		TM->Draw(GetImageID(), PosX, PosY, m_fScaleX*CamScale, m_fScaleY*CamScale, &m_rDrawRect, 0, 0, 0, -1);
		D3D->GetSprite()->Flush();	// DRAW ALL SPRITES NOW!!!
	}

	
#ifdef DRAWRECT
	int left = (int)GetPosX() - CCamera::GetInstance()->GetOffsetX();
	int top = (int)GetPosY() - CCamera::GetInstance()->GetOffsetY();
	int right = left + GetWidth();
	int bottom = top + GetHeight();

	CSGD_Direct3D::GetInstance()->DrawLine(left, top, right, top, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, bottom, right, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, top, left, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(right, top, right, bottom, 255, 0, 0);
#endif
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

void CBlock::CheckCulling()
{
	SetCulling(false);

	float CameraX = (float)CCamera::GetInstance()->GetOffsetX();
	float CameraY = (float)CCamera::GetInstance()->GetOffsetY();
	float xPos = GetPosX();
	float yPos = GetPosY();
	float wPos = xPos+GetWidth();
	float hPos = yPos+GetHeight();

	if(wPos < CameraX-100)
		SetCulling(true);
	if(xPos > CameraX+900)
		SetCulling(true);

	if(hPos < CameraY-100)
		SetCulling(true);
	if(yPos > CameraY+700)
		SetCulling(true);
}