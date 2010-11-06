#include "PrecompiledHeader.h"

#include "CSeekerDrone.h"
#include "CCamera.h"


#include "CGame.h"

CSeekerDrone::CSeekerDrone()
{
}
CSeekerDrone::CSeekerDrone(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{

}
CSeekerDrone::~CSeekerDrone()
{

}

void CSeekerDrone::Update(float fElapsedTime)
{
	CPatrolEnemy::Update(fElapsedTime);

	if(ReturnAIState() == pDead)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
	}
}

void CSeekerDrone::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

}