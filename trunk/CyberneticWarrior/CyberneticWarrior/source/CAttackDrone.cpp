#include "PrecompiledHeader.h"

#include "CAttackDrone.h"
#include "CCamera.h"


#include "CGame.h"
#include "CMapLoad.h"

CAttackDrone::CAttackDrone()
{
}
CAttackDrone::CAttackDrone(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Drone_Attack));
	GetAnimations()->SetCurrentAnimation(1);
}
CAttackDrone::~CAttackDrone()
{

}

void CAttackDrone::Update(float fElapsedTime)
{
	CPatrolEnemy::Update(fElapsedTime);

	if(ReturnAIState() == pDead)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
	}
}

void CAttackDrone::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

}