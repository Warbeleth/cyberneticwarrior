#include "PrecompiledHeader.h"

#include "CFLCLMech.h"
#include "CCamera.h"
#include "CCodeProfiler.h"

CFLCLMech::CFLCLMech()
{
}
CFLCLMech::CFLCLMech(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed, float fReviveTime, float fTimeReviving, bool bRevive) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	//m_nCID = CCodeProfiler::GetInstance()->CreateFunction("FLCL Mech");
	m_bRevive = bRevive;
	m_bReviving = false;
	m_fReviveTime = fReviveTime;
	m_fTimeReviving = fTimeReviving;
}
CFLCLMech::~CFLCLMech()
{

}

void CFLCLMech::Update(float fElapsedTime)
{
	//Code Profiler -START
	//CCodeProfiler::GetInstance()->FunctionStart(this->m_nCID);

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

	//Code Profiler -END
	//CCodeProfiler::GetInstance()->FuntionEnd(this->m_nCID);
}

void CFLCLMech::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

	if(m_bReviving == true)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: LOL, I\"m REVIVIN", (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY - 20) * CCamera::GetInstance()->GetScale()), 255, 0, 0);
}