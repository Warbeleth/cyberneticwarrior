#include "PrecompiledHeader.h"

#include "CTurretCore.h"
#include "CCamera.h"

#include "CGame.h"

CTurretCore::CTurretCore()
{
}
CTurretCore::CTurretCore(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{

}
CTurretCore::~CTurretCore()
{

}

void CTurretCore::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	if(ReturnAIState() == iDead)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
	}
}

void CTurretCore::Render()
{
	CIdleEnemy::Render();
}