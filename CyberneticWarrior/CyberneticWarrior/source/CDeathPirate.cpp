#include "PrecompiledHeader.h"

#include "CDeathPirate.h"
#include "CCamera.h"

#include "CGame.h"
#include "CMapLoad.h"

CDeathPirate::CDeathPirate()
{
}
CDeathPirate::CDeathPirate(int nImageID, float PosX, float PosY,int Width, int Height, int nState,
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed) : CIdleEnemy(nState, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Boss_Pirate));
	GetAnimations()->SetCurrentAnimation(0);
}
CDeathPirate::~CDeathPirate()
{

}

void CDeathPirate::Update(float fElapsedTime)
{
	CIdleEnemy::Update(fElapsedTime);

	if(GetGround())
	{
		SetBaseVelY(0);
	}
	else
	{
		SetBaseVelY(GetBaseVelY() + 50*fElapsedTime);

		if(GetBaseVelY() > 75)
			SetBaseVelY(75);

		SetPosY(GetPosY() + GetBaseVelY() *fElapsedTime);
	}

	switch(ReturnAIState())
	{
	case Idle:
		GetAnimations()->SetCurrentAnimation(0);
		break;
	case iActive:
		GetAnimations()->SetCurrentAnimation(1);
		if(!GetAnimations()->SameFrame() && GetAnimations()->GetTrigger() != 0)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateBulletMessage(this));
		}
		break;
	case iDead:
		ReleaseSpawner();
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
		break;
	};
}

void CDeathPirate::Render()
{
	CIdleEnemy::Render();
	#ifdef DRAWRECT
	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();
	int left = (int)GetPosX() - OffsetX;
	int top = (int)GetPosY() - OffsetY;
	int right = left + GetWidth();
	int bottom = top + GetHeight();

	CSGD_Direct3D::GetInstance()->DrawLine(left, top, right, top, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, bottom, right, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, top, left, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(right, top, right, bottom, 255, 0, 0);
#endif
}

bool CDeathPirate::CheckCollision(CBase* pBase)
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