#include "PrecompiledHeader.h"

#include "CFLCLMech.h"
#include "CCamera.h"
#include "CCodeProfiler.h"
#include "CMapLoad.h"
#include "CSinglePlayerState.h"

#include "CGame.h"

CFLCLMech::CFLCLMech()
{
}
CFLCLMech::CFLCLMech(int nImageID, float PosX, float PosY,int Width, int Height, int nState, float fCurrentPatrolDistance, 
				int nMaxHP, int nCurrentHP, int nSightRange, int nAttackRange, int nGlobalType, float fRateOfFire, 
				float fSpeed, float fReviveTime, float fTimeReviving, bool bRevive) : CPatrolEnemy(nState, fCurrentPatrolDistance, 
				 float(nSightRange + 100) /*max patrol distance*/, nGlobalType, nImageID, nMaxHP, nCurrentHP, nSightRange, 
				 nAttackRange, fRateOfFire, fSpeed, PosX, PosY, Width, Height)
{
	//m_nCID = CCodeProfiler::GetInstance()->CreateFunction("FLCL Mech");
	m_bRevive = bRevive;
	m_bReviving = false;
	m_fReviveTime = fReviveTime;
	m_fTimeReviving = fTimeReviving;
	SetAnimations(CMapLoad::GetInstance()->CreateAnimation(Ground_FLCL));
	GetAnimations()->SetCurrentAnimation(3);
}
CFLCLMech::~CFLCLMech()
{

}

void CFLCLMech::Update(float fElapsedTime)
{
	//Code Profiler -START
	//CCodeProfiler::GetInstance()->FunctionStart(this->m_nCID);

	CPatrolEnemy::Update(fElapsedTime);

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
	case Patrol:
		GetAnimations()->SetCurrentAnimation(3);
		break;
	case pActive:
		GetAnimations()->SetCurrentAnimation(0);
		break;
	case pDead:
		{
			GetAnimations()->SetCurrentAnimation(1);
			if(GetRevive())
			{
				SetRevive(false);
				m_bReviving = true;
			}
			else if(m_bReviving)
			{
				SetTimeDead(GetTimeDead() + (fElapsedTime));

				if(GetTimeDead() >= GetReviveTime())
				{
					m_bReviving = false;
					SetCurrentHP(GetMaxHP());
					GetAnimations()->SetCurrentAnimation(2);
					ChangeAIState(pActive);
				}
			}
			else
			{
				ReleaseSpawner();
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyEnemyMessage((CBaseEnemy*)this));
			}
		}
		break;
	};
	//Code Profiler -END
	//CCodeProfiler::GetInstance()->FuntionEnd(this->m_nCID);
}

void CFLCLMech::Render()
{
	CPatrolEnemy::Render();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

	if(m_bReviving == true)
		CSGD_Direct3D::GetInstance()->DrawTextA("State: LOL, I\"m REVIVIN", (int)((GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), (int)((GetPosY() - OffsetY - 20) * CCamera::GetInstance()->GetScale()), 255, 0, 0);
}

bool CFLCLMech::CheckCollision(CBase* pBase)
{
	CBase::CheckCollision( pBase );
	
	if( pBase->GetType() == OBJ_PLAYER && ReturnAIState() == pActive )
	{
		if(GetAnimations())
		{
			if(GetAnimations()->CheckHit( pBase, (int)GetPosX(), (int)GetPosY() ))
				CSinglePlayerState::GetInstance()->GetPlayerPointer()->DecrementHealth(5);
		}
	}
	else if(pBase->GetType() == OBJ_BLOCK)
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
					SetSpeed(-1*GetSpeed());
					SetBaseVelX(0);
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