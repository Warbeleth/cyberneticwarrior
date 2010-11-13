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
	this->SetShotDelay(0.0f);
}
CSiegeWalker::~CSiegeWalker()
{

}

void CSiegeWalker::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);
	SetShotDelay(this->GetShotDelay() + fElapsedTime);

	switch(ReturnAIState())
	{
	case Idle:
		GetAnimations()->SetCurrentAnimation(0);
		break;
	case iActive:
		GetAnimations()->SetCurrentAnimation(1);
		if(this->GetShotDelay() > this->GetRateOfFire())
		{
			this->SetShotDelay(0.0f);	
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
		}
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

CPoint CSiegeWalker::GetBulletStartPos( void )
{
	CPoint ptStartingPos;
	ptStartingPos.m_nX = (int)this->GetPosX();
	ptStartingPos.m_nY = (int)this->GetPosY();

	return ptStartingPos;
}
