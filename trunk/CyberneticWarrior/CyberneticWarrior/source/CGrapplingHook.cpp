#include "PrecompiledHeader.h"

#include "CGrapplingHook.h"

#include "CGame.h"
#include "CPlayer.h"
#include "CSinglePlayerState.h"
#include "CBlock.h"

CGrapplingHook::CGrapplingHook(void)
{
	this->m_fRotation = 0.0f;
	this->m_fRotationRate = 0.001f;
	this->SetType(OBJ_GRAPPLING_HOOK);
	this->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/hook.png"));
	this->SetHooked(0);
	CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetHookPointer(this);
}

CGrapplingHook::~CGrapplingHook(void) 
{
	//CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
	CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetHookPointer(NULL);
	CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetHookShot(false);
}


void CGrapplingHook::Update(float fElapsedTime)
{
	//	static bool bOnGround = 1;
	//if(this->m_bHooked && !CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetOnGround())
	//{
	//	this->SetPosX(this->GetPosX() + this->GetBaseVelX() * fElapsedTime);
	//	this->SetPosY(this->GetPosY() + this->GetBaseVelY() * fElapsedTime);
	//	//tVector2D vHook, vPlayer;
	//	//vHook.fX = this->GetPosX();
	//	//vHook.fY = this->GetPosY();

	//	//vPlayer.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX();
	//	//vPlayer.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

	//	//static bool one = 1;

	//	//if(one)
	//	//{
	//	//CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetRotation(AngleBetweenVectors(vHook, vPlayer));
	//	//float test = AngleBetweenVectors(vHook, vPlayer);
	//	//one = 0;
	//	//}
	//	//CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetRotation(-this->GetRotation());
	//	//bOnGround = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetOnGround();
	//}
	//else
	//{
		/*if(!bOnGround)
		{
			this->SetBaseVelX(0.0f);
			this->SetBaseVelY(0.0f);
		}*/
	CBase::Update(fElapsedTime);
	//}

	static tVector2D vScreenDimensions;
	vScreenDimensions.fX = (float)CGame::GetInstance()->GetScreenWidth();
	vScreenDimensions.fY = (float)CGame::GetInstance()->GetScreenHeight();


	if((this->GetPosX() + this->GetWidth()/2.0f) <= (CCamera::GetInstance()->GetOffsetX() - 20)
		|| ((this->GetPosX() - this->GetWidth()/2.0f) >= (CCamera::GetInstance()->GetOffsetX() + vScreenDimensions.fX + 20))
		|| (this->GetPosY() + (this->GetHeight()/2.0f)) <= (CCamera::GetInstance()->GetOffsetY() - 20)
		|| (this->GetPosY() - (this->GetHeight()/2.0f)) >= (CCamera::GetInstance()->GetOffsetY() + vScreenDimensions.fY+20)
		&& !CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetMouseDown() && this->m_bHooked)
	{
		// destroy
		CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetHookPointer(NULL);
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this, CSinglePlayerState::GetInstance()->GetPlayerPointer()));
		CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetHookShot(false);
	}

}

void CGrapplingHook::Render(void)
{
	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), 
		(int)(((this->GetPosX() + (this->GetWidth()/2.0f)) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()),
		(int)(((this->GetPosY() - (this->GetHeight()/2.0f)) - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(), 1.0f * CCamera::GetInstance()->GetScale(), 
		0, (this->GetWidth()/2.0f), (this->GetHeight()/2.0f),
		this->GetRotation());

	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	CSGD_Direct3D::GetInstance()->DrawLine((int)(((this->GetPosX() + this->GetWidth()) - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)((this->GetPosY() - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()),
		(int)((CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX()+CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth()/2 - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()), 
		(int)((CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY() - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1, 1, 1);
}

RECT CGrapplingHook::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)(this->GetPosY() );//- (GetHeight()>>1));
	rCollision.left = (LONG)(this->GetPosX() );//- (GetWidth()>>1));
	rCollision.bottom = rCollision.top + this->GetHeight();
	rCollision.right = rCollision.left + this->GetWidth();

	return rCollision; 
}

bool CGrapplingHook::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if(IntersectRect(&rIntersect, &this->GetRect(),&pBase->GetRect()))
	{
		if(pBase->GetType() == OBJ_BLOCK && !this->m_bHooked)
		{
			CBlock* pBlock = (CBlock*)pBase;

			this->SetHooked(1);
			this->SetBaseVelX(0.0f);
			this->SetBaseVelY(0.0f);

			float nOffSet = (pBlock->GetPosY() + pBlock->GetHeight()) - this->GetPosY() +3.0f;
			float hi = pBlock->GetPosY();
			float po = (float)pBlock->GetHeight();
			float pd = this->GetPosY();

			this->SetSwingMax(nOffSet);
			//CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetRotationPosX(this->GetPosX());
			//CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetRotationPosY(this->GetPosY());
		}
		return 1;
	}//mccaw
	//Derek Bliss
	else
	{
		return 0;
	}
	return 0;
}

void CGrapplingHook::HandleEvent(CEvent *pEvent)
{

}


float	CGrapplingHook::GetRotation(void)const	{return this->m_fRotation;}
void	CGrapplingHook::SetRotation(float fRotation)	{this->m_fRotation = fRotation;}

float	CGrapplingHook::GetRotationRate(void)const {return this->m_fRotationRate;}


bool	CGrapplingHook::GetIfHooked(void)const	{return this->m_bHooked;}
void	CGrapplingHook::SetHooked(bool bHooked)	{this->m_bHooked = bHooked;}