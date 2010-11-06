#include "PrecompiledHeader.h"

#include "CMech.h"
#include "CCamera.h"
#include "CCodeProfiler.h"
#include "CMapLoad.h"
#include "CSinglePlayerState.h"

#include "CGame.h"

CMech::CMech()
{
}
CMech::CMech(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Ground_Mech));
	GetAnimations()->SetCurrentAnimation(1);
}
CMech::~CMech()
{

}

void CMech::Update(float fElapsedTime)
{
	CPatrolEnemy::Update(fElapsedTime);

	if(ReturnAIState() == pDead)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
	}
}

void CMech::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

}

bool CMech::CheckCollision(CBase* pBase)
{
	CBase::CheckCollision( pBase );

	bool m_bAttacking = true;
	
	if( pBase->GetType() == OBJ_PLAYER && m_bAttacking )
	{
		if(GetAnimations())
		{
			if(GetAnimations()->CheckHit( pBase, GetPosX(), GetPosY() ))
				CSinglePlayerState::GetInstance()->GetPlayerPointer()->DecrementHealth(5);
		}
	}

	return false;
}