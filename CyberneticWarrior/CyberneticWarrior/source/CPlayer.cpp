#include "PrecompiledHeader.h"

#include "CPlayer.h"
#include "CGame.h"
#include "CCamera.h"
#include "CSinglePlayerState.h"
#include "CGrapplingHook.h"
#include "CHud.h"

CPlayer::CPlayer(void)
{
	this->SetType(OBJ_PLAYER);
	this->m_bOnGround = 1;
	this->m_bOnPlatform = 0;
	this->m_bForward = 1;
	this->SetMouseDown(0);
	this->SetHookPointer(NULL);
	this->m_fRotation = 0.0f;
	this->m_vRotationCenter.fX = this->GetPosX()+this->GetWidth();
	this->m_vRotationCenter.fY = 0.0f;
	this->m_vVectorVelocity.fX = 0.0f;
	this->m_vVectorVelocity.fY = 0.0f;
	this->m_vJoyVecPos.fX	   = 0.0f;
	this->m_vJoyVecPos.fY	   = 0.0f;
	
	this->m_fJoyRot = 0.0f;
	this->m_fWaitTime = 0.0f;
	
	// Health
	m_nRemainingHealth = 100;
	m_nTotalHealth = 100;

	// Energy
	m_nRemainingEnergy = 0;
	m_nTotalEnergy = 100;

	// Score
	m_nScore = 0;

	// Currently Selected
	m_nSelectedWeapon = 6;
	m_nSelectedHeadSlot = 3;
	m_nSelectedBootSlot = 2;
	
	m_fHandRotation = 0.0f;
	m_bHomingOn = false;
	m_nHandID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/Arm.png" );

	this->m_pHud = new CHud();
	this->m_pHud->SetPlayerPointer(this);
}

CPlayer::~CPlayer(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
	delete m_pHud;
}

tVector2D CPlayer::GetSpeed(void)	{ return m_vSpeed; }

//float	CPlayer::GetVectorMagnitude(void)	{ return m_f
float	CPlayer::GetRotationRate(void)	{ return m_fRotationRate; }

void CPlayer::Update(float fElapsedTime)
{
	if(this->m_pHook)
	{
		if(!this->m_pHook->GetIfHooked() || this->GetOnGround())
		{
			CBase::Update(fElapsedTime);
		}
	}
	else
	{
		CBase::Update(fElapsedTime);
	}

	this->Input(fElapsedTime);


	if(CSGD_DirectInput::GetInstance()->MouseGetPosX() > this->GetPosX())
	{
		this->m_bForward = 1;
	}
	else
	{
		this->m_bForward = 0;
	}
	//this->m_fWaitTime +=  fElapsedTime;
	
	//this->m_vJoyVecPos.fX = this->GetPosX();
	//this->m_vJoyVecPos.fY = this->GetPosY();
	//this->m_vJoyVecPos = Vector2DRotate(this->m_vJoyVecPos, this->m_fJoyRot);
	//this->m_vJoyVecPos.fX = this->GetPosX() + this->m_vJoyVecPos.fX;
	//this->m_vJoyVecPos.fY = this->GetPosY() - this->m_vJoyVecPos.fY;
	


	if(this->m_pHook)
	{
		/*if(this->GetBaseVelX() > 0.0f && this->GetOnGround())
		{
		if(this->m_pHook->GetIfHooked() 
		&& this->m_pHook->GetPosX() < this->GetPosX() 
		&& this->m_pHook->GetRotation() > -1.80f)
		{
		this->m_pHook->SetRotation(this->m_pHook->GetRotation() - this->m_pHook->GetRotationRate() * fElapsedTime);
		if(!this->m_bOnGround)
		{
		this->SetRotation(this->m_pHook->GetRotation());
		}

		}
		}
		else if(this->GetBaseVelX() < 0.0f && this->GetOnGround())
		{
		if(this->m_pHook->GetIfHooked() 
		&& this->m_pHook->GetPosX() > this->GetPosX() 
		&& this->m_pHook->GetRotation() < 0.45f)
		{
		this->m_pHook->SetRotation(this->m_pHook->GetRotation() + this->m_pHook->GetRotationRate() * fElapsedTime);
		if(!this->m_bOnGround)
		{
		this->SetRotation(this->m_pHook->GetRotation());
		}
		}
		}*/


		tVector2D vHook;
		if(this->m_pHook->GetIfHooked() && !this->GetOnGround())// && this->GetRotation() <  3.14f/2&& this->GetRotation() > -3.14f*2)
		{
			this->m_vVectorVelocity.fX = this->GetPosX();
			this->m_vVectorVelocity.fY = this->GetPosY();
			vHook.fX = this->m_pHook->GetPosX() + this->m_pHook->GetWidth()/2;
			vHook.fY = this->m_pHook->GetPosY();

			//this->m_vVectorVelocity = Vector2DNormalize(this->m_vVectorVelocity);
			
			this->m_vVectorVelocity = this->m_vVectorVelocity - vHook;


			//D3DXToDegree(
			//this->m_vVectorVelocity = this->m_vVectorVelocity * 500.0f;// this->m_vSpeed.fX;
			this->m_vVectorVelocity = Vector2DRotate(this->m_vVectorVelocity, this->GetRotation());
			//this->SetBaseVelX(0.0f);
			//this->SetBaseVelY(0.0f);

			this->m_vSpeed.fY = -this->m_vVectorVelocity.fY;	

			this->SetPosX(this->m_vVectorVelocity.fX + vHook.fX);
			this->SetPosY(this->m_vVectorVelocity.fY + vHook.fY);
		}
		else
		{
			//this->m_vVectorVelocity.fX = 0.0f;

			//this->m_vVectorVelocity.fY = 0.0f;
			this->SetBaseVelX(this->m_vSpeed.fX);
		}


	}
	else
	{
		this->SetRotation(0.0f);
		//////////////////////////////////////////
		// Update Movement Velocity on the X-Axis.
		//////////////////////////////////////////
		this->SetBaseVelX(this->m_vSpeed.fX);
		////////////////////////////////////////
	}




	////////////////////////////////////////
	// Check to see if we should Update
	// Movement Velocity on the Y-Axis
	// on whether or not the player is
	// on top of a ground object.
	////////////////////////////////////////
	if(this->m_bOnGround)
	{
		this->m_vSpeed.fY = 0.0f;
		this->SetBaseVelY(this->m_vSpeed.fY);
	}
	else
	{
		if(this->m_pHook)
		{
			if(!this->m_pHook->GetIfHooked())
			{
				this->m_vSpeed.fY = this->m_vSpeed.fY +( 900.0f * fElapsedTime);
				this->SetBaseVelY(this->m_vSpeed.fY);
			}

		}
		else
		{
			this->m_vSpeed.fY = this->m_vSpeed.fY +( 900.0f * fElapsedTime);
			this->SetBaseVelY(this->m_vSpeed.fY);
		}
	}
	////////////////////////////////////////

	////////////////////////////////////////
	// Platform Checking
	////////////////////////////////////////
	/*if((this->GetPosY() + this->GetHeight()) > CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformPoint.fY  
	&& (this->GetPosY() + this->GetHeight()) < (CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformPoint.fY + CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformSize.fY)
	&& (this->GetPosX() + this->GetWidth() - 5) > CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformPoint.fX 
	&& (this->GetPosX()+20) < (CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformPoint.fX + CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformSize.fX)
	&& !this->m_bOnGround
	&& this->m_vSpeed.fY > 0.0f)
	{
	this->m_bOnGround = 1;
	this->SetPosY((float)CSinglePlayerState::GetInstance()->GetPlatform()->m_tTempPlatformPoint.fY - this->GetHeight());
	}
	else
	{
	this->m_bOnGround = 0;
	}*/

	if(this->GetPosY() > 600 - this->GetHeight())
	{
		this->m_bOnGround = 1;
		this->SetPosY(600 - (float)this->GetHeight());
	}
	////////////////////////////////////////

	tVector2D vecHandRotation;
	vecHandRotation.fX = 0.0f;
	vecHandRotation.fY = -1.0f;

	//vecHandRotation = Vector2DRotate( vecHandRotation,)

	tVector2D vecMouseVector;
	vecMouseVector.fX = CSGD_DirectInput::GetInstance()->MouseGetPosX() - GetPosX() + CCamera::GetInstance()->GetOffsetX();
	vecMouseVector.fY = CSGD_DirectInput::GetInstance()->MouseGetPosY() - GetPosY() + CCamera::GetInstance()->GetOffsetY();

	m_fHandRotation = AngleBetweenVectors( vecHandRotation, vecMouseVector );
	
	if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX()  - CCamera::GetInstance()->GetOffsetX() )
		m_fHandRotation = SGD_PI + (SGD_PI - m_fHandRotation);

	////////////////////////////////////////
	// Camera boundary checking
	////////////////////////////////////////
	CCamera::GetInstance()->SetCameraOffsetX(int(this->GetPosX()-400));
	
	if(CCamera::GetInstance()->GetOffsetX() < 0)
		CCamera::GetInstance()->SetCameraOffsetX(0);
		
	CCamera::GetInstance()->SetCameraOffsetY(int(this->GetPosY()-430));

	if(CCamera::GetInstance()->GetOffsetY() < 0)
		CCamera::GetInstance()->SetCameraOffsetY(0);
	
	this->m_pHud->Update( fElapsedTime );
	////////////////////////////////////////
}

void CPlayer::Input(float fElapsedTime)
{
	///////////////////////////////
	// Input Checking
	///////////////////////////////
	static float fJumpSpeed = -500.0f;
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_SPACE)
		|| CSGD_DirectInput::GetInstance()->JoystickButtonPressed(1)) && this->m_bOnGround)
	{
		this->m_bOnGround = false;
		this->m_vSpeed.fY = fJumpSpeed;
		this->SetBaseVelY(this->m_vSpeed.fY);
	}

	static int nMoveSpeed = 100;
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_A) 
		|| CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_LEFT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT)))
	{
		if(this->m_vSpeed.fX > -200.0f)
		{
			this->m_vSpeed.fX -= nMoveSpeed * fElapsedTime;
		}

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				/*if(this->GetRotation() <= 3.14f/2)
				{*/
				this->m_pHook->SetRotation(this->m_pHook->GetRotation() + this->m_pHook->GetRotationRate() * fElapsedTime);
				if(!this->m_bOnGround)
				{
					this->SetRotation(this->m_pHook->GetRotation());
				}
				/*}
				else
				{
				this->m_pHook->SetRotation(.9f);
				}*/

			}
		}

	}

	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_D) 
		|| CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_RIGHT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT)))
	{
		if(this->m_vSpeed.fX < 200.0f)
		{
			this->m_vSpeed.fX += nMoveSpeed * fElapsedTime;
		}

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				/*if(this->GetRotation() >= -3.14f*2)
				{*/
				this->m_pHook->SetRotation(this->m_pHook->GetRotation() - this->m_pHook->GetRotationRate() * fElapsedTime);
				if(!this->m_bOnGround)
				{
					this->SetRotation(this->m_pHook->GetRotation());
				}	
				/*}
				else
				{
				this->m_pHook->SetRotation(.45f);
				}*/

			}
		}
	}

	if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(MOUSE_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(4)))
	{
		this->SetMouseDown(1);
		if(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateHookMessage(this));
		}
	}

	if((CSGD_DirectInput::GetInstance()->MouseButtonReleased(MOUSE_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(4)))
	{
		this->SetMouseDown(0);
		if(this->m_pHook)
		{
			this->m_pHook->SetHooked(0);
		}
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this));
	}
		
	if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(MOUSE_LEFT) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(7)))
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LSHIFT))
	{
		m_bHomingOn = !m_bHomingOn;
	}

	//if(CSGD_DirectInput::GetInstance()->JoystickGetRStickXAmount() < 0.0f && this->m_fWaitTime > 0.1f)
	//{
	//	this->m_fWaitTime = 0.0f;
	//	this->m_fJoyRot += SGD_PI;// * fElapsedTime;
	//}
	//else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickXAmount() > 0.0f && this->m_fWaitTime > 0.1f)
	//{
	//	this->m_fWaitTime = 0.0f;
	//	this->m_fJoyRot -= SGD_PI;// * fElapsedTime;
	//}


	if(CSinglePlayerState::GetInstance()->GetInputType())
	{
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = 1.0f;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = 1.0f;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = -1.0f;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = -1.0f;
		}







		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = 0.0f;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = 0.0f;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP))
		{
			this->m_vJoyVecPos.fX = 0.0f;
			this->m_vJoyVecPos.fY = -1.0f;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN))
		{
			this->m_vJoyVecPos.fX = 0.0f;
			this->m_vJoyVecPos.fY = 1.0f;
		}
	}


	///////////////////////////////
}


void CPlayer::Render(void)
{

	RECT rDrawRect;
	rDrawRect.top = 0;
	rDrawRect.left = 0;
	rDrawRect.right = this->GetWidth();
	rDrawRect.bottom = this->GetHeight();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

	if(!this->m_bForward)
	{
		CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), 
			(int)((this->GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()) + this->GetWidth(),
			(int)((this->GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 
			-1.0f * CCamera::GetInstance()->GetScale(), 
			1.0f * CCamera::GetInstance()->GetScale(), &rDrawRect);//, (float)GetWidth()/2, (float)GetHeight());
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(),
		(int)((this->GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), 
		(int)((this->GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(),
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rDrawRect, this->m_vRotationCenter.fX, this->m_vRotationCenter.fX, this->m_fRotation);
	}

	if(m_bHomingOn)
		CSGD_Direct3D::GetInstance()->DrawLine( (int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale()),
		(int)(((GetPosY() + (GetHeight()/4)) - OffsetY) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosX()+8) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosY()+8) * CCamera::GetInstance()->GetScale()), 
		255, 0, 0 );

	/*CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(),
		(int)((this->GetPosX() - OffsetX) * CCamera::GetInstance()->GetScale()), 
		(int)((this->GetPosY() - OffsetY) * CCamera::GetInstance()->GetScale()), 
		1.0f * CCamera::GetInstance()->GetScale(),
		1.0f * CCamera::GetInstance()->GetScale(), 
		&rDrawRect, this->m_vRotationCenter.fX, this->m_vRotationCenter.fX, this->m_fRotation);*/

	CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
		(int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale()), 
		(int)(((GetPosY() - (GetHeight()/4)) - OffsetY) * CCamera::GetInstance()->GetScale()), 
		0.6f * CCamera::GetInstance()->GetScale(), 0.6f * CCamera::GetInstance()->GetScale(), 
		0, 16, 64, m_fHandRotation, -1 );
		
	this->m_pHud->Render();
}

RECT CPlayer::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)(this->GetPosY() );//- (GetHeight()>>1));
	rCollision.left = (LONG)(this->GetPosX() );//- (GetWidth()>>1));
	rCollision.bottom = rCollision.top + this->GetHeight();
	rCollision.right = rCollision.left + this->GetWidth();

	return rCollision;
}

bool CPlayer::CheckCollision(CBase* pBase)
{
	if((this->GetPosY() + this->GetHeight()) > pBase->GetPosY()  
		&& (this->GetPosY() + this->GetHeight()) < (pBase->GetPosY() + pBase->GetHeight())
		&& (this->GetPosX() + this->GetWidth() - 5) > pBase->GetPosX()
		&& (this->GetPosX()+20) < (pBase->GetPosX() + pBase->GetWidth())
		&& !this->m_bOnGround
		&& this->m_vSpeed.fY > 0.0f
		&& pBase->GetType() == OBJ_BLOCK)
	{
		this->m_bOnGround = 1;
		this->m_bOnPlatform = 1;
		this->SetPosY(pBase->GetPosY() - this->GetHeight());
		return 1;
	}
	else
	{
		if(this->m_bOnPlatform)
		{
			this->m_bOnGround = 0;
			this->m_bOnPlatform = 0;
		}
//		return 0;
	}


	RECT rIntersect;
	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{ 
		if(pBase->GetType() == OBJ_PICKUP)// && !this->m_bOnGround && this->m_vSpeed.fY > 0.0f)
		{
			CSinglePlayerState::GetInstance()->SetProfileValues(1);
		}


		return true;
	}
	/*else
	{
		if(this->GetPosX() < (480 - this->GetHeight()) && this->m_bOnPlatform)
		{
			this->m_bOnGround = 0;
			this->m_bOnPlatform = 0;
		}
		return false;
	}*/

	return false;
}

void CPlayer::HandleEvent(CEvent* pEvent)
{
	/*if(pEvent->GetEventID() == )
	{
	}*/
}






/////////
// TEMP

tVector2D* CPlayer::GetJoyPos(void) {return &this->m_vJoyVecPos;}



bool CPlayer::GetMouseDown(void)	{return this->m_bMouseDown;}
void CPlayer::SetMouseDown(bool bMouseDown) {this->m_bMouseDown = bMouseDown;}


bool CPlayer::GetOnGround(void)	{return this->m_bOnGround;}

CGrapplingHook*	CPlayer::GetHookPointer(void)	{return this->m_pHook;}
void	CPlayer::SetHookPointer(CGrapplingHook*	pHook) {this->m_pHook = pHook;}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetSpeedY
////////////////////////////////////////////////////////////////////////////////////
void CPlayer::SetSpeedY(float nSpeedY)
{
	this->m_vSpeed.fY = nSpeedY;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetSpeedX
////////////////////////////////////////////////////////////////////////////////////
void CPlayer::SetSpeedX(float nSpeedX)
{
	this->m_vSpeed.fX = nSpeedX;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetOnGround
////////////////////////////////////////////////////////////////////////////////////
void CPlayer::SetOnGround(bool bOnGround)
{
	this->m_bOnGround = bOnGround;
}
/////////////////////

tVector2D	CPlayer::GetRotationPos(void){return this->m_vRotationCenter;}
void	CPlayer::SetRotationPosX(float fX) {this->m_vRotationCenter.fX = fX;}
void	CPlayer::SetRotationPosY(float fY) {this->m_vRotationCenter.fY = fY;}

float	CPlayer::GetRotation(void) {return this->m_fRotation;}
void	CPlayer::SetRotation(float fRotation) {this->m_fRotation = fRotation;}

void CPlayer::SetPlayerNumber( int nPlayer )
{ 
	m_pHud->SetPlayerNumber(nPlayer); 
}