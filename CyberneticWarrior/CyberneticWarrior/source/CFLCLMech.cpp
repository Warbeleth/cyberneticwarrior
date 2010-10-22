#include "CFLCLMech.h"

CFLCLMech::CFLCLMech()
{
}
CFLCLMech::CFLCLMech(bool bRevive, float fReviveTime, float fTimeReviving, int nState, int nCurrentPatrolDistance, 
				 int nMaxPatrolDistance, int nType, int nImageID, int nMaxHP, int nCurrentHP, int nSightRange, 
				 int nAttackRange, float fRateOfFire, float fSpeed, int PosX, int PosY, int Width, int Height) : CPatrolEnemy(nState, nCurrentPatrolDistance, 
				 nMaxPatrolDistance, nType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	m_bRevive = bRevive;
	m_fReviveTime = fReviveTime;
	m_fTimeReviving = fTimeReviving;
}
CFLCLMech::~CFLCLMech()
{

}

void CFLCLMech::Update(float fElapsedTime)
{
	CPatrolEnemy::Update(fElapsedTime);

	SetCurrentHP(GetCurrentHP() - int((55 * fElapsedTime)));

	if(ReturnAIState() == pDead)
	{
		if(GetRevive())
		{
			SetRevive(false);
			m_bReviving = true;
		}
		else if(m_bReviving)
		{
			SetTimeDead(GetTimeDead() + (fElapsedTime));

			if(GetTimeDead() >= GetReviveTime())
			{
				m_bReviving = false;
				SetCurrentHP(GetMaxHP());
				ChangeAIState(pActive);
			}
		}
	}
}

void CFLCLMech::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetCameraRect().left;
	int OffsetY = CCamera::GetInstance()->GetCameraRect().top;

	if(m_bReviving == true)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: LOL, I\"m REVIVIN", GetPosX() - OffsetX, GetPosY() - OffsetY-20, 255, 0, 0);
}