#include "PrecompiledHeader.h"

#include "CPimpStriker.h"
#include "CCamera.h"

#include "CGame.h"

CPimpStriker::CPimpStriker()
{
}
CPimpStriker::CPimpStriker(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{

}
CPimpStriker::~CPimpStriker()
{

}

void CPimpStriker::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	if(ReturnAIState() == iDead)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
	}
}

void CPimpStriker::Render()
{
	CIdleEnemy::Render();
}