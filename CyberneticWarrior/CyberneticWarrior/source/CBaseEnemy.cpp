//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �CBaseEnemy.cpp�
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies to derive off of
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CBaseEnemy.h"
#include "CSinglePlayerState.h"

CBaseEnemy::CBaseEnemy()
{
	m_nGlobalType = OBJ_ENEMY;
	m_nType = -1;
	m_nImageID = -1;
	m_nMaxHP = 100;
	m_nCurrentHP = m_nMaxHP;
	m_nSightRange = 1;
	m_nAttackRange = 1;
	m_fPosX = 0;
	m_fPosY = 0;
	m_nWidth = 1;
	m_nHeight = 1;
	m_fRateOfFire = 0.0f;
	m_fSpeed = 0.0f;
	m_bSinglePlayer = true;	//Will be a CGame::GetInstance() call to check for game state when creating enemies
}

CBaseEnemy::CBaseEnemy(int nType, int nImageID, int nMaxHP, int nCurrentHP, 
			   int nSightRange, int nAttackRange, float fRateOfFire, 
			   float fSpeed, float nPosX, float nPosY, int nWidth, int nHeight)
{
	m_nGlobalType = OBJ_ENEMY;
	m_nType = nType;
	m_nImageID = nImageID;
	m_nMaxHP = nMaxHP;
	m_nCurrentHP = nCurrentHP;
	m_nSightRange = nSightRange;
	m_nAttackRange = nAttackRange;
	m_fPosX = nPosX;
	m_fPosY = nPosY;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_fRateOfFire = fRateOfFire;
	m_fSpeed = fSpeed;
	m_vTargetPosition.fX = 0;
	m_vTargetPosition.fY = 0;
	m_bSinglePlayer = true; //Will be a CGame::GetInstance() call to check for game state when creating enemies
}

CBaseEnemy::~CBaseEnemy()
{

}

RECT CBaseEnemy::GetRect()
{
	RECT rReturn;
	rReturn.left = (int)GetPosX();
	rReturn.top = (int)GetPosY();
	rReturn.right = rReturn.left + GetWidth();
	rReturn.bottom = rReturn.top + GetHeight();

	return rReturn;
}

void CBaseEnemy::Update(float fElapsedTime)
{
	//Get target position
	if(m_bSinglePlayer)
	{
		CPlayer* tmp = CSinglePlayerState::GetInstance()->GetPlayerPointer();
		m_vTargetPosition.fX = tmp->GetPosX();
		m_vTargetPosition.fY = tmp->GetPosY();
	}
	//else
	//{
	//	//Check which of the two enemies is closer in multiplayer
	//}
}

void CBaseEnemy::Render(void)
{
	int offsetX = CCamera::GetInstance()->GetOffsetX();
	int offsetY = CCamera::GetInstance()->GetOffsetY();

	if(m_nImageID != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nImageID, (int)GetPosX() - offsetX, (int)GetPosY() - offsetY);
	}
}

//void CBaseEnemy::AddRef(void)
//{
//	m_uiRefCount++;
//}
//
//void CBaseEnemy::Release(void)
//{
//	this->m_uiRefCount--;
//
//	if(this->m_uiRefCount == 0)
//		delete this;
//}

bool CBaseEnemy::CheckCollision(CBase* pBase)
{
	RECT rIntersect;

	if(IntersectRect(&rIntersect, &GetRect(), &(pBase->GetRect())))
	{ 
		if(pBase->GetType() != OBJ_ENEMY)
			return true;
	}

	return false;
}