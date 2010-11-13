#include "PrecompiledHeader.h"

#include "CTurretCore.h"
#include "CCamera.h"
#include "CMapLoad.h"
#include "CSinglePlayerState.h"

#include "CGame.h"

CTurretCore::CTurretCore()
{
}
CTurretCore::CTurretCore(int nImageID, float PosX, float PosY, int nGlobalType, int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{

	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(nGlobalType));
	GetAnimations()->SetCurrentAnimation(0);
	SetShotDelay(0.0f);
	SetRateOfFire(0.75f);
}
CTurretCore::~CTurretCore()
{

}

void CTurretCore::Update(float fElapsedTime)
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
		if(!GetAnimations()->SameFrame() && GetAnimations()->GetTrigger() != 0)
		{
			if(this->GetEnemyType() ==Turret_Gun )
			{
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateBulletMessage(this));
			}
			else if(this->GetEnemyType() ==Turret_Frost )
			{
				//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateIceMessage(CSinglePlayerState::GetInstance()->GetPlayerPointer(), this));
			}
			else if(this->GetEnemyType() ==Turret_Fire )
			{
				//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateFireMessage(CSinglePlayerState::GetInstance()->GetPlayerPointer(), this));
			}
			else if(this->GetEnemyType() ==Turret_Multi )
			{
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
			}
		}
		break;
	case iDead:
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CTurretCore::Render()
{
	CIdleEnemy::Render();
}

CPoint CTurretCore::GetBulletStartPos( void )
{
	CPoint ptStartPos;
	RECT rFrame = GetAnimations()->GetCollisionFrame( (int)GetPosX(), (int)GetPosY() );
	ptStartPos.m_nX = rFrame.left + GetAnimations()->GetPivotPoint().m_nX;
	ptStartPos.m_nY = rFrame.top + GetAnimations()->GetPivotPoint().m_nY;

	return ptStartPos;
}