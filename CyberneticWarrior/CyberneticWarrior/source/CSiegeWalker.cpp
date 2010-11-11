#include "PrecompiledHeader.h"

#include "CSiegeWalker.h"
#include "CCamera.h"

#include "CGame.h"
#include "CMapLoad.h"

CSiegeWalker::CSiegeWalker()
{
}
CSiegeWalker::CSiegeWalker(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Ground_Siege));
	GetAnimations()->SetCurrentAnimation(0);
}
CSiegeWalker::~CSiegeWalker()
{

}

void CSiegeWalker::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	switch(ReturnAIState())
	{
	case Idle:
		GetAnimations()->SetCurrentAnimation(0);
		break;
	case iActive:
		GetAnimations()->SetCurrentAnimation(1);
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
		break;
	case iDead:
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CSiegeWalker::Render()
{
	CIdleEnemy::Render();
}