#include "PrecompiledHeader.h"

#include "CHeavyAttackDrone.h"
#include "CCamera.h"


#include "CGame.h"
#include "CMapLoad.h"

CHeavyAttackDrone::CHeavyAttackDrone()
{
}
CHeavyAttackDrone::CHeavyAttackDrone(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Drone_Heavy));
	GetAnimations()->SetCurrentAnimation(1);
	this->SetShotDelay(0.0f);
}
CHeavyAttackDrone::~CHeavyAttackDrone()
{

}

void CHeavyAttackDrone::Update(float fElapsedTime)
{
	CPatrolEnemy::Update(fElapsedTime);

	switch(ReturnAIState())
	{
	case Patrol:
		GetAnimations()->SetCurrentAnimation(1);
		break;
	case pActive:
		GetAnimations()->SetCurrentAnimation(0);
		if(!GetAnimations()->SameFrame() && GetAnimations()->GetTrigger() != 0)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreatePlasmaMessage(this));
		}
		break;
	case pDead:
		ReleaseSpawner();
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CHeavyAttackDrone::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

}