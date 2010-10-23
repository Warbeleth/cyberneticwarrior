#include "CPlayer.h"

#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "CGame.h"
#include "CCamera.h"

#include "CSinglePlayerState.h"



CPlayer::CPlayer(void)
{
	this->m_vCameraPos.fX = this->m_vCameraPos.fY = 0.0f;
	this->SetType(OBJ_PLAYER);
	this->m_bOnGround = 1;
	this->m_bOnPlatform = 0;
	this->SetMouseDown(0);
	this->SetHookPointer(NULL);
	this->m_fRotation = 0.0f;
	this->m_vRotationCenter.fX = this->GetPosX()+this->GetWidth();
	this->m_vRotationCenter.fY = 0.0f;
	this->m_vVectorVelocity.fX = 0.0f;
	this->m_vVectorVelocity.fY = 0.0f;
	
	m_fHandRotation = 0.0f;
	m_bHomingOn = false;
	m_nHandID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/Arm.png" );

}

CPlayer::~CPlayer(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
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

	this->UpdateCamera(fElapsedTime);
	this->Input(fElapsedTime);





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

			this->m_vVectorVelocity = this->m_vVectorVelocity - vHook;


			//D3DXToDegree(
			//this->m_vVectorVelocity = this->m_vVectorVelocity * 500.0f;// this->m_vSpeed.fX;
			this->m_vVectorVelocity = Vector2DRotate(this->m_vVectorVelocity, this->GetRotation());

			this->SetBaseVelX(0.0f);
			this->SetBaseVelY(0.0f);

			this->SetPosX(this->m_vVectorVelocity.fX + vHook.fX);
			this->SetPosY(this->m_vVectorVelocity.fY + vHook.fY);
		}
		else
		{
			this->m_vVectorVelocity.fX = 0.0f;

			this->m_vVectorVelocity.fY = 0.0f;
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

	////////////////////////////////////////
	// Camera boundary checking
	////////////////////////////////////////
	//static int nPlayerCamROffset = CGame::GetInstance()->GetScreenWidth()- CAMOFFSET;
	if(this->GetPosX() >= CGame::GetInstance()->GetScreenWidth() - CAMOFFSET && (this->GetCamX() + CAMOFFSET) < 2048)
	{
		this->SetPosX((float)CGame::GetInstance()->GetScreenWidth() - CAMOFFSET);
	}
	if(this->GetPosX() <= CAMOFFSET && this->GetCamX() > CAMOFFSET)
	{
		this->SetPosX(CAMOFFSET);
	}

	if(this->GetCamX() < 0)
	{
		this->SetBaseVelX(0.0f);
		this->m_vSpeed.fX = 0.0f;
		this->SetCamX(0);
		this->SetPosX(0);
	}
	else if((this->GetCamX() + this->GetWidth())> 2048)
	{
		this->SetBaseVelX(0.0f);
		this->m_vSpeed.fX = 0.0f;
		this->SetCamX(2048- (float)this->GetWidth());
		this->SetPosX((float)CGame::GetInstance()->GetScreenWidth() - this->GetWidth());

	}
	////////////////////////////////////////

	tVector2D vecHandRotation;
	vecHandRotation.fX = 0.0f;
	vecHandRotation.fY = -1.0f;

	//vecHandRotation = Vector2DRotate( vecHandRotation,)

	tVector2D vecMouseVector;
	vecMouseVector.fX = CSGD_DirectInput::GetInstance()->MouseGetPosX() - GetPosX();
	vecMouseVector.fY = CSGD_DirectInput::GetInstance()->MouseGetPosY() - GetPosY();

	m_fHandRotation = AngleBetweenVectors( vecHandRotation, vecMouseVector );
	
	if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX() )
		m_fHandRotation = SGD_PI + (SGD_PI - m_fHandRotation);
}

void CPlayer::UpdateCamera(float fElapsedTime)
{
	///////////////////////////////
	// Camera Updates
	///////////////////////////////
	this->SetCamX(this->GetCamX() + this->GetBaseVelX() * fElapsedTime);
	this->SetCamY(this->GetCamY() + this->GetBaseVelY() * fElapsedTime);


	if((this->GetCamX() + this->GetWidth()) >= CCamera::GetInstance()->GetCameraRight()
		&& CCamera::GetInstance()->GetCameraRect().right <= 2048)
	{
		CCamera::GetInstance()->UpdateCam(((int)this->GetCamX() + this->GetWidth()), 1);
	}

	if(this->GetCamX() <= CCamera::GetInstance()->GetCameraLeft()
		&& CCamera::GetInstance()->GetCameraRect().left >= 0)
	{
		CCamera::GetInstance()->UpdateCam((int)this->GetCamX(), 0);
	}
	///////////////////////////////
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

	///////////////////////////////
}


void CPlayer::Render(void)
{

	RECT rDrawRect;
	rDrawRect.top = 0;
	rDrawRect.left = 0;
	rDrawRect.right = this->GetWidth();
	rDrawRect.bottom = this->GetHeight();
	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), (int)this->GetPosX(), (int)this->GetPosY(), 
		1.0f,1.0f, &rDrawRect);//, this->m_vRotationCenter.fX, this->m_vRotationCenter.fX, this->m_fRotation);//, (float)GetWidth()/2, (float)GetHeight());

	if(m_bHomingOn)
		CSGD_Direct3D::GetInstance()->DrawLine( (int)(GetPosX() + (GetWidth()/2)), (int)(GetPosY() + (GetHeight()/4)), CSGD_DirectInput::GetInstance()->MouseGetPosX()+8, CSGD_DirectInput::GetInstance()->MouseGetPosY()+8, 255, 0, 0 );

	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), (int)this->GetPosX(), (int)this->GetPosY(), 
		1.0f,1.0f, &rDrawRect, this->m_vRotationCenter.fX, this->m_vRotationCenter.fX, this->m_fRotation);

	CSGD_TextureManager::GetInstance()->Draw(m_nHandID, (int)(GetPosX() + (GetWidth()/2)), (int)(GetPosY() - (GetHeight()/4)), 0.6f, 0.6f, 0, 16, 64, m_fHandRotation, -1 );
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

void CPlayer::SetCamX(float nCamX)
{
	this->m_vCameraPos.fX = nCamX;
}

void CPlayer::SetCamY(float nCamY)
{
	this->m_vCameraPos.fY = nCamY;
}

float CPlayer::GetCamX(void)
{
	return this->m_vCameraPos.fX;
}

float CPlayer::GetCamY(void)
{
	return this->m_vCameraPos.fY;
}


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