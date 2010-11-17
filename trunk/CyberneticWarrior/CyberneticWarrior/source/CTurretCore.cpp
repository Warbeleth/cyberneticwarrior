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

	if(GetGround())
	{
		SetBaseVelY(0);
	}
	else
	{
		SetBaseVelY(GetBaseVelY() + 25*fElapsedTime);

		if(GetBaseVelY() > 25)
			SetBaseVelY(25);

		SetPosY(GetPosY() + GetBaseVelY() *fElapsedTime);
	}

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
			if(this->GetRateOfFire() < this->GetShotDelay())
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
				this->SetShotDelay(0.0f);
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

bool CTurretCore::CheckCollision(CBase* pBase)
{
	CBase::CheckCollision( pBase );
	
	if(pBase->GetType() == OBJ_BLOCK)
	{
		CBlock* BLOCK = (CBlock*)pBase;

		RECT rIntersect;

		//Descriptive replacement variables
		RECT rMyRect = { (LONG)GetPosX(), (LONG)GetPosY(), 0, 0 };
		rMyRect.right = rMyRect.left + GetWidth();
		rMyRect.bottom = rMyRect.top + GetHeight();

		RECT rHisRect = { (LONG)BLOCK->GetPosX(), (LONG)BLOCK->GetPosY(), 0, 0 };
		rHisRect.right = rHisRect.left + BLOCK->GetWidth();
		rHisRect.bottom = rHisRect.top + BLOCK->GetHeight();

		if( IntersectRect( &rIntersect, &rMyRect, &rHisRect ) )
		{
			if( (rIntersect.right-rIntersect.left) > (rIntersect.bottom-rIntersect.top) )
			{
				if(BLOCK->GetBlock() == BLOCK_SOLID || BLOCK->GetBlock() == BLOCK_MOVING || BLOCK->GetBlock() == BLOCK_PARTIAL || BLOCK->GetBlock() == BLOCK_UNSTABLE)
				{
					if(rMyRect.bottom > rHisRect.top && rMyRect.top < rHisRect.top)
					{
						SetPosY( (float)rHisRect.top - GetHeight() );
						SetGround(true);
						SetBaseVelY(0);
						SetCollision(true);
					}
					else if(rMyRect.top < rHisRect.bottom && rMyRect.bottom > rHisRect.top)
						SetPosY((float)rHisRect.bottom);

				}
				else if(BLOCK->GetBlock() == BLOCK_TRAP)
				{
					SetGround(true);
					SetCollision(true);
					SetBaseVelY(0);
				}

			}
			else if((rIntersect.right-rIntersect.left) < (rIntersect.bottom-rIntersect.top))
			{
				if(BLOCK->GetBlock() == BLOCK_SOLID || BLOCK->GetBlock() == BLOCK_MOVING || BLOCK->GetBlock() == BLOCK_PARTIAL)
				{
				}
				else if(BLOCK->GetBlock() == BLOCK_TRAP)
				{
					SetGround(true);
					SetCollision(true);
					SetBaseVelY(0);
				}
			}
			return true;
		}
	}

	if(GetGround() && GetCollision() == false)
	{
		SetGround(false);
	}

	return false;
}