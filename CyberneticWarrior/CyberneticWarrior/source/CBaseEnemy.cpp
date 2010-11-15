//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CBaseEnemy.cpp”
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies to derive off of
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CBaseEnemy.h"
#include "CSinglePlayerState.h"
#include "CMapLoad.h"
#include "CSPawner.h"

CBaseEnemy::CBaseEnemy()
{
	SetType(OBJ_ENEMY);
	m_nGlobalType = -1;
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
	m_pHostSpawner = NULL;
	m_bSinglePlayer = true;	//Will be a CGame::GetInstance() call to check for game state when creating enemies
}

CBaseEnemy::CBaseEnemy(int nGlobalType, int nImageID, int nMaxHP, int nCurrentHP, 
			   int nSightRange, int nAttackRange, float fRateOfFire, 
			   float fSpeed, float nPosX, float nPosY, int nWidth, int nHeight)
{
	SetType(OBJ_ENEMY);
	m_nGlobalType = nGlobalType;
	//m_nImageID = nImageID;
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
	SetAnimations(NULL);
	switch(nGlobalType)
	{
	case Turret_Gun:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.TurretGun; 
		break;
	case Turret_Frost:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.TurretFrost; 
		break;
	case Turret_Fire:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.TurretFire; 
		break;
	case Turret_Multi:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.TurretMulti; 
		break;
	case Drone_Attack:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.DroneAttack; 
		break;
	case Drone_Seeker:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.DroneSeeker; 
		break;
	case Drone_Heavy:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.DroneHeavy; 
		break;
	case Ground_Mech:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.GroundMech; 
		break;
	case Ground_Siege:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.GroundSiege; 
		break;
	case Ground_FLCL:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.GroundFLCL; 
		break;
	case Boss_Apple:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.BossApple; 
		break;
	case Boss_Pimp:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.BossPimp; 
		break;
	case Boss_Pirate:
		m_nImageID = CMapLoad::GetInstance()->m_sEnemyImage.BossPirate; 
		break;
	}
	m_bSinglePlayer = true; //Will be a CGame::GetInstance() call to check for game state when creating enemies
}

CBaseEnemy::~CBaseEnemy()
{
	delete GetAnimations();
	SetAnimations(NULL);

	if( m_pHostSpawner )
		m_pHostSpawner->DecrementTotalCurrentlySpawned( );
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

	if(GetAnimations())
		GetAnimations()->Update( fElapsedTime );
	//else
	//{
	//	//Check which of the two enemies is closer in multiplayer
	//}
}

void CBaseEnemy::Render(void)
{
	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();
	
	if(GetAnimations() != NULL)
	{

		GetAnimations()->Render( (int)GetPosX()+GetAnimations()->GetFrameWidth()/2, (int)GetPosY()+GetAnimations()->GetFrameHeight() );
	}
	else
	{
		if(m_nImageID != -1)
		{
			RECT rDrawRect;
			rDrawRect.left = 0;
			rDrawRect.top = 0;
			rDrawRect.right = GetWidth();
			rDrawRect.bottom = GetHeight();

			CSGD_TextureManager::GetInstance()->Draw(m_nImageID, (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), CCamera::GetInstance()->GetScale(), CCamera::GetInstance()->GetScale(), &rDrawRect);
		}
		else
		{
			#ifdef DRAWRECT
			int left = (int)GetPosX() - OffsetX;
			int top = (int)GetPosY() - OffsetY;
			int right = left + GetWidth();
			int bottom = top + GetHeight();

			CSGD_Direct3D::GetInstance()->DrawLine(left, top, right, top, 255, 0, 0);
			CSGD_Direct3D::GetInstance()->DrawLine(left, bottom, right, bottom, 255, 0, 0);
			CSGD_Direct3D::GetInstance()->DrawLine(left, top, left, bottom, 255, 0, 0);
			CSGD_Direct3D::GetInstance()->DrawLine(right, top, right, bottom, 255, 0, 0);
			#endif
		}
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
	if(pBase->GetType() != OBJ_ENEMY && pBase->GetType() != OBJ_SPAWNER )
	{
		if(GetAnimations())
		{
			if(GetAnimations()->CheckCollision( pBase, (int)GetPosX(), (int)GetPosY()))
				return true;
		}
		else
		{
			RECT rIntersect;
			if(IntersectRect(&rIntersect, &GetRect(), &(pBase->GetRect())))
					return true;
		}
	}
	return false;
}

void CBaseEnemy::CheckCulling()
{
	SetCulling(false);

	float CameraX = (float)CCamera::GetInstance()->GetOffsetX();
	float CameraY = (float)CCamera::GetInstance()->GetOffsetY();
	float xPos = GetPosX();
	float yPos = GetPosY();
	float wPos = xPos+GetWidth();
	float hPos = yPos+GetHeight();

	if(wPos < CameraX)
		SetCulling(true);
	if(xPos > CameraX+800)
		SetCulling(true);

	if(hPos < CameraY)
		SetCulling(true);
	if(yPos > CameraY+600)
		SetCulling(true);
}

CPoint CBaseEnemy::GetBulletStartPos( void )
{
	CPoint ptStartPos;
	RECT rFrame = GetAnimations()->GetCollisionFrame( (int)GetPosX(), (int)GetPosY() );
	ptStartPos.m_nX = rFrame.left + GetAnimations()->GetPivotPoint().m_nX;
	ptStartPos.m_nY = rFrame.top + GetAnimations()->GetPivotPoint().m_nY;

	return ptStartPos;
}