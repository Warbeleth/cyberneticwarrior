#include "PrecompiledHeader.h"

#include "CPlayer.h"
#include "CGame.h"
#include "CCamera.h"
#include "CSinglePlayerState.h"
#include "CGrapplingHook.h"
#include "CHud.h"
#include "CBlock.h"
#include "CAnimationLoad.h"
#include "CMapLoad.h"

CPlayer::CPlayer(void)
{
	// Type
	this->SetType(OBJ_PLAYER);

	// Jumping
	this->m_bOnGround = false;
	this->m_bOnPlatform = false;

	// Facing Forward
	this->m_bForward = true;

	// Mouse Down
	this->SetMouseDown(false);

	// Hook
	this->SetHookPointer(NULL);

	// Rotation
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

	// Animation
	SetAnimations( new CAnimations() );
	GetAnimations()->LoadBinary("resource/binary/PlayerAnimations.bae");
	GetAnimations()->SetCurrentAnimation(1);

	// Currently Selected
	m_nSelectedWeapon = 6;
	m_nSelectedHeadSlot = 3;
	m_nSelectedBootSlot = 2;

	// Hand
	m_fHandRotation = 4.2f;
	m_bHomingOn = false;
	m_nHandID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/Weapons.png" );

	// HUD
	this->m_pHud = new CHud();
	this->m_pHud->SetPlayerPointer(this);
}

CPlayer::~CPlayer(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
	delete m_pHud;
	delete GetAnimations();
	SetAnimations( NULL );
}

tVector2D CPlayer::GetSpeed(void)	{ return m_vSpeed; }

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


	if(CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX() > this->GetPosX())
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
		/*if(this->GetBaseVelX() > 0.0f)
		{*/
		//if(this->m_pHook->GetIfHooked() 
		//&& this->m_pHook->GetPosX() < this->GetPosX() 
		//&& this->m_pHook->GetRotation() < SGD_PI*2 && this->m_pHook->GetRotation() > (3 *SGD_PI)/2)
		//{
		//this->m_pHook->SetRotation(this->m_pHook->GetRotation() + this->m_pHook->GetRotationRate() * fElapsedTime);
		//if(!this->m_bOnGround)
		//{
		//this->SetRotation(this->m_pHook->GetRotation());
		//}

		//}
		///*}
		//else if(this->GetBaseVelX() < 0.0f)
		//{*/
		//if(this->m_pHook->GetIfHooked() 
		//&& this->m_pHook->GetPosX() > this->GetPosX() 
		//&& this->m_pHook->GetRotation() > SGD_PI&& this->m_pHook->GetRotation() < (3 *SGD_PI)/2)
		//{
		//this->m_pHook->SetRotation(this->m_pHook->GetRotation() - this->m_pHook->GetRotationRate() * fElapsedTime);
		//if(!this->m_bOnGround)
		//{
		//this->SetRotation(this->m_pHook->GetRotation());
		//}
		//}

		//if(this->GetRotation() > 3.15)// && this->GetRotation() > 0)
		//{
		//	this->SetRotation(this->GetRotation()-1.0f);
		//}
		/*if(this->GetRotation() < 0 && this->GetRotation() > SGD_PI*2)
		{
			this->SetRotation(SGD_PI);
		}*/
		//}
		//if(this->GetRotation() > 0.0f)
		//{
		//	this->SetRotation(/*this->GetRotation() + SGD_PI * fElapsedTime);
		//}*/

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
			//this->SetRotation(/*this->GetRotation() + */AngleBetweenVectors(vHook, this->m_vVectorVelocity));
			//this->SetRotation(SGD_PI +(SGD_PI - this->GetRotation()));
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
	GetAnimations()->Update( fElapsedTime );
}

void CPlayer::Input(float fElapsedTime)
{
	GetAnimations()->SetCurrentAnimation(1);

	///////////////////////////////
	// Input Checking
	///////////////////////////////
	static float fJumpSpeed = -550.0f;
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

		GetAnimations()->SetCurrentAnimation(0);
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

		GetAnimations()->SetCurrentAnimation(0);
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
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateGrenadeMessage(this));
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
	RECT rPlayerRect = GetAnimations()->GetFrame( (int)(GetPosX()-CCamera::GetInstance()->GetOffsetX()), (int)(GetPosY()-CCamera::GetInstance()->GetOffsetY()) );
	CSGD_Direct3D::GetInstance()->DrawLine(rPlayerRect.left, rPlayerRect.top, rPlayerRect.right, rPlayerRect.top, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine(rPlayerRect.right, rPlayerRect.top, rPlayerRect.right, rPlayerRect.bottom, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine(rPlayerRect.right, rPlayerRect.bottom, rPlayerRect.left, rPlayerRect.bottom, 255, 0, 0 );
	CSGD_Direct3D::GetInstance()->DrawLine(rPlayerRect.left, rPlayerRect.bottom, rPlayerRect.left, rPlayerRect.top, 255, 0, 0 );
	
	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();

	if(m_bHomingOn)
		CSGD_Direct3D::GetInstance()->DrawLine( (int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale()),
		(int)(((GetPosY() + (GetHeight()/4)) - OffsetY) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosX()+8) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosY()+8) * CCamera::GetInstance()->GetScale()), 
		255, 0, 0 );

	GetAnimations()->Render( (int)GetPosX()+GetAnimations()->GetFrameWidth()/2, (int)GetPosY()+GetAnimations()->GetFrameHeight());

	RECT rRender = { 340, 164, 550, 234 };
/*
	RECT rRender;
	rRender.top = 256;
	rRender.left = 130;
	rRender.bottom = 450;
	rRender.right = 220;
	if(this->m_bForward)
	{
	CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
		(int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale()-10), 
		(int)(((GetPosY() - (GetHeight()/2)) - OffsetY) * CCamera::GetInstance()->GetScale()+25), 
		0.6f * CCamera::GetInstance()->GetScale(), 0.6f * CCamera::GetInstance()->GetScale(), 
		&rRender, 64, 128, m_fHandRotation, -1 );
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
		(int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale())+GetWidth()/2, 
		(int)(((GetPosY() - (GetHeight()/2)) - OffsetY) * CCamera::GetInstance()->GetScale()+25), 
		-0.6f * CCamera::GetInstance()->GetScale(), 0.6f * CCamera::GetInstance()->GetScale(), 
		&rRender, 64, 128, m_fHandRotation, -1 );
	}
*/
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
	if(pBase->GetType() == OBJ_BLOCK)
	{
		CBlock* BLOCK = (CBlock*)pBase;
		//Descriptive replacement variables
		float myX;
		float myY;
		float myRight;
		float myBottom;
		float hisX = BLOCK->GetPosX();
		float hisY = BLOCK->GetPosY();
		float hisRight = hisX + BLOCK->GetWidth();
		float hisBottom = hisY + BLOCK->GetHeight();

		if(GetAnimations())
		{
			RECT rPlayerRect = GetAnimations()->GetFrame( (int)(GetPosX()), (int)(GetPosY()) );
			myX = (float)rPlayerRect.left;
			myY = (float)rPlayerRect.top;
			myRight = (float)rPlayerRect.right;
			myBottom = (float)rPlayerRect.bottom;
		}
		else
		{
			myX = GetPosX();
			myY = GetPosY();
			myRight = myX + GetWidth();
			myBottom = myY + GetHeight();
		}

		if(BLOCK->GetBlock() == BLOCK_SOLID)
		if(BLOCK->GetBlock() == BLOCK_SOLID || BLOCK->GetBlock() == BLOCK_MOVING)
		{
			if(myBottom >= hisY  
				&& (myBottom) < (hisBottom)
				&& (myRight) > hisX
				&& (myX) < (hisRight)
				&& !this->m_bOnGround
				&& this->m_vSpeed.fY >= 0.0f)
			{
				this->m_bOnGround = 1;
				this->m_bOnPlatform = 1;
				this->SetPosY(hisY - this->GetHeight());
				CMapLoad::GetInstance()->m_bCollisionCheck = true;
				return true;
			}
			else if(this->m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
			{
				this->m_bOnGround = 0;
				this->m_bOnPlatform = 0;
				return false;
			}
		}
		else if(BLOCK->GetBlock() == BLOCK_PARTIAL)
		{
			if((myBottom*0.5f) >= hisY  
				&& (myBottom*0.5f) < (hisBottom)
				&& (myRight) > hisX
				&& (myX) < (hisRight)
				&& !this->m_bOnGround
				&& this->m_vSpeed.fY >= 0.0f)
			{
				this->m_bOnGround = 1;
				this->m_bOnPlatform = 1;
				this->SetPosY(hisY - this->GetHeight());
				CMapLoad::GetInstance()->m_bCollisionCheck = true;
				return true;
			}
			else if(this->m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
			{
				this->m_bOnGround = 0;
				this->m_bOnPlatform = 0;
				return false;
			}
		}
		else if(BLOCK->GetBlock() == BLOCK_TRAP)
		{
			if(myBottom >= hisY  
				&& (myBottom) < (hisBottom)
				&& (myRight) > hisX
				&& (myX) < (hisRight)
				&& !this->m_bOnGround
				&& this->m_vSpeed.fY >= 0.0f)
			{
				this->m_bOnGround = 1;
				this->m_bOnPlatform = 1;
				this->SetPosY(hisBottom - this->GetHeight());
				this->DecrementHealth(20);
				CMapLoad::GetInstance()->m_bCollisionCheck = true;
				return true;
			}
			else if(this->m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
			{
				this->m_bOnGround = 0;
				this->m_bOnPlatform = 0;
				return false;
			}
		}
		else if(BLOCK->GetBlock() == BLOCK_UNSTABLE)
		{
			if(myBottom >= hisY  
				&& (myBottom) < (hisBottom)
				&& (myRight) > hisX
				&& (myX) < (hisRight)
				&& !this->m_bOnGround
				&& this->m_vSpeed.fY >= 0.0f)
			{
				this->m_bOnGround = 1;
				this->m_bOnPlatform = 1;
				BLOCK->SetStable(false);
				this->SetPosY(hisY - this->GetHeight());
				CMapLoad::GetInstance()->m_bCollisionCheck = true;
				return true;
			}
			else if(this->m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
			{
				this->m_bOnGround = 0;
				this->m_bOnPlatform = 0;
				return false;
			}
		}
		
		
	}

	//if(GetAnimations()->CheckCollision( pBase ) )
	//	return true;

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