//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CIdleEnemy.cpp”
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies that have idle and active -
//			states to derive off of
/////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CIdleEnemy.h"
#include "CCamera.h"

CIdleEnemy::CIdleEnemy(void)
{
}

CIdleEnemy::CIdleEnemy(int nState, int nType, int nImageID, int nMaxHP, int nCurrentHP, 
		   int nSightRange, int nAttackRange, float fRateOfFire, 
		   float fSpeed, int PosX, int PosY, int Width, int Height) : CBaseEnemy(nType, nImageID, nMaxHP, nCurrentHP, 
																		nSightRange, nAttackRange, fRateOfFire, fSpeed,
																		PosX, PosY, Width, Height)

{
	m_nState = nState;
}

CIdleEnemy::~CIdleEnemy(void)
{
}

void CIdleEnemy::Update(float fElapsedTime)
{
	CBaseEnemy::Update(fElapsedTime);

	if(GetCurrentHP() <= 0)
		ChangeAIState(iDead);
	else
	{
		tVector2D Result;
		Result.fX = GetTargetPosition().fX - GetPosX();
		Result.fY = GetTargetPosition().fY - GetPosY();

		int Distance = int(sqrt(Result.fX*Result.fX + Result.fY*Result.fY));

		if(Distance <= GetSightRange())
		{
			ChangeAIState(iActive);
		}
		else
			ChangeAIState(Idle);
	}
}

void CIdleEnemy::Render()
{
	CBaseEnemy::Render();

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	int OffsetX = CCamera::GetInstance()->GetCameraRect().left;
	int OffsetY = CCamera::GetInstance()->GetCameraRect().top;

	if(ReturnAIState() == Idle)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Idle", GetPosX() - OffsetX, GetPosY() - OffsetY, 0, 0, 255);
	else if(ReturnAIState() == iActive)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Active", GetPosX() - OffsetX, GetPosY() - OffsetY, 0, 255, 0);
	else
		CSGD_Direct3D::GetInstance()->DrawTextA("State: Dead", GetPosX() - OffsetX, GetPosY() - OffsetY, 255, 0, 0);
}

void CIdleEnemy::ChangeAIState(int nNewState)
{
	m_nState = nNewState;
}

int CIdleEnemy::ReturnAIState()
{
	return m_nState;
}