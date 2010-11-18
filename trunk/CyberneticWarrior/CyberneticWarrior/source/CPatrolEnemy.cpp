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
#include "CSPawner.h"

CPatrolEnemy::CPatrolEnemy(void)
{
}

CPatrolEnemy::CPatrolEnemy(int nState, float fCurrentPatrolDistance, float fMaxPatrolDistance, int nGlobalType, int nImageID, 
				 int nMaxHP, int nCurrentHP,int nSightRange, int nAttackRange, float fRateOfFire, 
			     float fSpeed, float PosX, float PosY, int Width, int Height) : CBaseEnemy(nGlobalType, nImageID, nMaxHP, nCurrentHP, 
																		nSightRange, nAttackRange, fRateOfFire, fSpeed,
																		PosX, PosY, Width, Height)

{
	m_nState = nState;
	m_fCurrentPatrolDistance = fCurrentPatrolDistance;
	m_fMaxPatrolDistance = fMaxPatrolDistance;
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

		float Distance = sqrt(Result.fX*Result.fX + Result.fY*Result.fY);

		if(Distance <= GetSightRange())
		{
			ChangeAIState(pActive);
			this->ReturnAIState();
		}
		else
			ChangeAIState(Patrol);

		switch(ReturnAIState())
		{
		case Patrol:
			{
				SetPosX((GetPosX() + GetBaseVelX() * fElapsedTime));

				if(GetPosX() <= 0)
				{
					SetPosX(0);
					SetCurrentDist(0);
					SetSpeed(-1*GetSpeed());
				}

				SetCurrentDist(GetCurrentDist() + (fabs(GetBaseVelX()) * fElapsedTime));

				if(GetCurrentDist() >= GetMaxDist())
				{
					SetCurrentDist(0);
					SetSpeed(-1*GetSpeed());
				}

				SetBaseVelX(GetBaseVelX() + GetSpeed() * fElapsedTime);

				if(GetBaseVelX() > 50)
					SetBaseVelX(50);
				else if(GetBaseVelX() < -50)
					SetBaseVelX(-50);
			}
			break;
		case pActive:
			{
				SetSpeed(-1*GetSpeed());
				SetBaseVelX(0);
			}
			break;
		case pDead:
			{
			}
			break;
		};
	}
}

void CPatrolEnemy::Render()
{
	CBaseEnemy::Render();

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

	if(ReturnAIState() == Patrol)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Patrol", (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 0, 0, 255);
	else if(ReturnAIState() == pActive)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Active", (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 0, 255, 0);
	else
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Dead", (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 255, 0, 0);
}

void CPatrolEnemy::ChangeAIState(int nNewState)
{
	m_nState = nNewState;
}

int CPatrolEnemy::ReturnAIState()
{
	return m_nState;
}