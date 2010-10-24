//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CPatrolEnemy.cpp”
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies that have idle and active -
//			states to derive off of
/////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CPatrolEnemy.h"
#include "CCamera.h"

CPatrolEnemy::CPatrolEnemy(void)
{
}

CPatrolEnemy::CPatrolEnemy(int nState, int nCurrentPatrolDistance, int nMaxPatrolDistance, int nType, int nImageID, 
				 int nMaxHP, int nCurrentHP,int nSightRange, int nAttackRange, float fRateOfFire, 
			     float fSpeed, int PosX, int PosY, int Width, int Height) : CBaseEnemy(nType, nImageID, nMaxHP, nCurrentHP, 
																		nSightRange, nAttackRange, fRateOfFire, fSpeed,
																		PosX, PosY, Width, Height)

{
	m_nState = nState;
	m_nCurrentPatrolDistance = nCurrentPatrolDistance;
	m_nMaxPatrolDistance = nMaxPatrolDistance;
}

CPatrolEnemy::~CPatrolEnemy(void)
{
}

void CPatrolEnemy::Update(float fElapsedTime)
{
	CBaseEnemy::Update(fElapsedTime);

	if(GetCurrentHP() <= 0)
		ChangeAIState(pDead);
	else
	{
		tVector2D Result;
		Result.fX = GetTargetPosition().fX - GetPosX();
		Result.fY = GetTargetPosition().fY - GetPosY();

		int Distance = int(sqrt(Result.fX*Result.fX + Result.fY*Result.fY));

		if(Distance <= GetSightRange())
		{
			ChangeAIState(pActive);
		}
		else
			ChangeAIState(Patrol);

		if(ReturnAIState() == Patrol)
		{
			SetPosX(int(GetPosX() + GetSpeed() * fElapsedTime));

			if(GetPosX() <= 0)
			{
				SetPosX(0);
				SetCurrentDist(0);
				SetSpeed(-1*GetSpeed());
			}

			SetCurrentDist(GetCurrentDist() + (int(fabs(GetSpeed()) * fElapsedTime)));

			if(GetCurrentDist() >= GetMaxDist())
			{
				SetCurrentDist(0);
				SetSpeed(-1*GetSpeed());
			}

		}
	}
}

void CPatrolEnemy::Render()
{
	CBaseEnemy::Render();

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	int OffsetX = CCamera::GetInstance()->GetCameraRect().left;
	int OffsetY = CCamera::GetInstance()->GetCameraRect().top;

	if(ReturnAIState() == Patrol)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Patrol", GetPosX() - OffsetX, GetPosY() - OffsetY, 0, 0, 255);
	else if(ReturnAIState() == pActive)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Active", GetPosX() - OffsetX, GetPosY() - OffsetY, 0, 255, 0);
	else
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Dead", GetPosX() - OffsetX, GetPosY() - OffsetY, 255, 0, 0);
}

void CPatrolEnemy::ChangeAIState(int nNewState)
{
	m_nState = nNewState;
}

int CPatrolEnemy::ReturnAIState()
{
	return m_nState;
}