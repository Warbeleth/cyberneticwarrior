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
	CSinglePlayerState::GetInstance()->SetProfileValues(1);
	
	// Setting Weapon Rects
	this->m_rWeapons[this->HG].top		= 12;
	this->m_rWeapons[this->HG].left		= 40;
	this->m_rWeapons[this->HG].right	= 80;
	this->m_rWeapons[this->HG].bottom	= 146;
	// Pivots
	m_ptPivots[HG].m_nX = 20;
	m_ptPivots[HG].m_nY = 126;
	// Offsets
	m_ptArmOffsets[HG].m_nX = -20;
	m_ptArmOffsets[HG].m_nY = -80;
	// Radius
	m_fStartPosRadius[HG] = -80.0f;

	this->m_rWeapons[this->RL].top		 = 264;
	this->m_rWeapons[this->RL].left		 = 138;
	this->m_rWeapons[this->RL].right	 = 206;
	this->m_rWeapons[this->RL].bottom	 = 442;
	// Pivots
	m_ptPivots[RL].m_nX = 50;
	m_ptPivots[RL].m_nY = 120;
	// Offsets
	m_ptArmOffsets[RL].m_nX = -30;
	m_ptArmOffsets[RL].m_nY = -80;
	// Radius
	m_fStartPosRadius[RL] = -80.0f;

	this->m_rWeapons[this->FT].top		 = 11;
	this->m_rWeapons[this->FT].left		 = 132;
	this->m_rWeapons[this->FT].right	 = 200;
	this->m_rWeapons[this->FT].bottom	 = 198;
	// Pivots
	m_ptPivots[FT].m_nX = 18;
	m_ptPivots[FT].m_nY = 173;
	// Offsets
	m_ptArmOffsets[FT].m_nX = -15;
	m_ptArmOffsets[FT].m_nY = -120;
	// Radius
	m_fStartPosRadius[FT] = -200.0f;

	this->m_rWeapons[this->PR].top		 = 12;
	this->m_rWeapons[this->PR].left		 = 244;
	this->m_rWeapons[this->PR].right	 = 300;
	this->m_rWeapons[this->PR].bottom	 = 206;
	// Pivots
	m_ptPivots[PR].m_nX = 14;
	m_ptPivots[PR].m_nY = 180;
	// Offsets
	m_ptArmOffsets[PR].m_nX = -15;
	m_ptArmOffsets[PR].m_nY = -125;
	// Radius
	m_fStartPosRadius[PR] = -115.0f;

	this->m_rWeapons[this->SR].top		 = 249;
	this->m_rWeapons[this->SR].left		 = 25;
	this->m_rWeapons[this->SR].right	 = 78;
	this->m_rWeapons[this->SR].bottom	 = 448;
	// Pivots
	m_ptPivots[SR].m_nX = 21;
	m_ptPivots[SR].m_nY = 184;
	// Offsets
	m_ptArmOffsets[SR].m_nX = -15;
	m_ptArmOffsets[SR].m_nY = -125;
	// Radius
	m_fStartPosRadius[SR] = -115.0f;

	this->m_rWeapons[this->SG].top		 = 266;
	this->m_rWeapons[this->SG].left		 = 252;
	this->m_rWeapons[this->SG].right	 = 317;
	this->m_rWeapons[this->SG].bottom	 = 338;
	// Pivots
	m_ptPivots[SG].m_nX = 46;
	m_ptPivots[SG].m_nY = 58;
	// Offsets
	m_ptArmOffsets[SG].m_nX = -30;
	m_ptArmOffsets[SG].m_nY = -40;
	// Radius
	m_fStartPosRadius[SG] = -30;

	this->m_fInputTime = 0.0f;

	this->m_nWeaponIndex = this->HG;

	// Type
	this->SetType(OBJ_PLAYER);

	// Jumping
	this->m_bOnGround = false;
	this->m_bOnPlatform = false;
	this->m_bOnMovingPlatform = false;
	this->m_bJumped = false;
	this->m_bHovering =false;
	this->m_bBoosting = false;
	this->m_bFDash = false;
	this->m_bBDash = false;

	this->m_bHookShot = false;
	this->m_bRotFoward = false;
	this->m_bRotBackward = false;
	this->m_bFirstSwing = true;

	// Facing Forward
	this->m_bForward = true;

	// Mouse Down
	this->SetMouseDown(false);

	// Hook
	this->SetHookPointer(NULL);

	// Moving Platform
	this->m_pMovingBlock = NULL;

	// Rotation
	this->m_fRotation = 0.0f;
	this->m_fRotationFixRate = 0.04f;

	this->m_vRotationCenter.fX = this->GetPosX()+this->GetWidth();
	this->m_vRotationCenter.fY = 0.0f;
	this->m_vVectorVelocity.fX = 0.0f;
	this->m_vVectorVelocity.fY = 0.0f;
	this->m_vJoyVecPos.fX	   = 0.0f;
	this->m_vJoyVecPos.fY	   = 0.0f;
	this->m_fJoyRot = 0.0f;
	this->m_fWaitTime = 0.0f;
	this->m_fGravity = 900.0f;

	this->m_fDash = 1200.f;


	// Moving Platform Position Updates
	this->m_fMovingPlatformPosX = 0.0f;

	// sonic rifle stuff
	this->m_nCharge = 0;
	this->m_fChargeRate = 0.009f;


	// Health
	this->m_fRemainingHealth = 777.0f;
	this->m_fTotalHealth = 777.0f;

	// Energy
	this->m_fRemainingEnergy = 100.0f;
	this->m_fTotalEnergy = 100.0f;

	// Score
	this->m_nScore = 0;


	// boost
	this->m_fBoostTime = 0.0f;

	// Animation
	SetAnimations(new CAnimations());
	GetAnimations()->LoadBinary("resource/binary/MainCharacter.bae");
	GetAnimations()->SetCurrentAnimation(1);

	// Currently Selected
	m_nSelectedWeapon = this->HAND_GUN;
	m_nSelectedHeadSlot = 3;
	m_nSelectedBootSlot = this->BOOTS;

	// Hand
	m_fHandRotation = 4.2f;
	m_bHomingOn = false;
	m_nHandID = CSGD_TextureManager::GetInstance()->LoadTexture( "resource/graphics/Weapons.png" );


	// Shut down players dynamic items check
	this->m_bShutDown = false;

	// Check if update should adjust players swinging
	this->m_bFixSwing = true;


	// HUD
	this->m_pHud = new CHud();
	this->m_pHud->SetPlayerPointer(this);

	if(CSinglePlayerState::GetInstance()->GetInputType())
	{
		this->m_fHandRotation = SGD_PI/2;
	}

	//Setup Float
	m_fFireDelay = 0.0f;

}

CPlayer::~CPlayer(void)
{
	this->m_bShutDown = false;
	this->m_bOnMovingPlatform = false;
	
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
	
	// sonic rifle stuff
	this->m_nCharge = 0;
	this->m_fChargeRate = 0.01f;
	this->m_bFDash = false;
	this->m_bBDash = false;
	this->m_bHookShot = false;

	delete m_pHud;
	delete GetAnimations();
	this->m_pHud = NULL;

	this->m_pMovingBlock = NULL;

	SetAnimations( NULL );
}


void CPlayer::Update(float fElapsedTime)
{
	// Check to see if game was paused if so destroy dynamic Items (example: Grappling hook)
	if(this->m_pHook && this->m_bShutDown)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this->m_pHook, this));
		this->m_bHookShot = false;
		this->m_bShutDown = false;
	}

	// so needed
	/*if(this->m_nSwingCount == 0)
		&& this->GetPosX() > (this->m_pHook->GetPosX() - 3.0f) 
		&& this->GetPosX() < (this->m_pHook->GetPosX() + 3.0f))
	{
		this->m_fRotationFixRate = 0.001f;
	}
	else if(this->m_nSwingCount == 1 )
	{
		this->m_fRotationFixRate = 0.01f;
	}*/

	
	// If player is swinging do not update based on CBase otherwise Carry on
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


	// If the player is on a moving platform update position 
	// according to the platforms movement
	if(this->m_bOnMovingPlatform && this->m_pMovingBlock)
	{
		float difference = (this->m_pMovingBlock->GetPosX() - this->m_fMovingPlatformPosX);
		float playerX = this->GetPosX();
		float newPX = this->GetPosX() + ((this->m_pMovingBlock->GetPosX() - this->m_fMovingPlatformPosX));
		this->SetPosX(this->GetPosX() + ((this->m_pMovingBlock->GetPosX() - this->m_fMovingPlatformPosX)));
	}


	// Check to see which direction player is currently facing
	if(CSinglePlayerState::GetInstance()->GetInputType() == 0)
	{
		if(CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX() > this->GetPosX())
		{
			this->m_bForward = 1;
		}
		else
		{
			this->m_bForward = 0;
		}
	}

	if(this->m_vSpeed.fX == this->m_fDash ||this->m_vSpeed.fX == -this->m_fDash) 
		this->m_fBoostTime += fElapsedTime;
	
	if(this->m_bBDash || this->m_bFDash)
	{
		this->m_fWaitTime += fElapsedTime;
	}
	
	// used for gamepad controls
	// Notes: May be used later, probably not(Do not remove yet)

	if(CSGD_DirectInput::GetInstance()->JoystickButtonDown(4) || CSGD_DirectInput::GetInstance()->JoystickButtonDown(5))
	{
		this->m_fInputTime +=  fElapsedTime;
	}
	//this->m_vJoyVecPos.fX = this->GetPosX();
	//this->m_vJoyVecPos.fY = this->GetPosY();
	//this->m_vJoyVecPos = Vector2DRotate(this->m_vJoyVecPos, this->m_fJoyRot);
	//this->m_vJoyVecPos.fX = this->GetPosX() + this->m_vJoyVecPos.fX;
	//this->m_vJoyVecPos.fY = this->GetPosY() - this->m_vJoyVecPos.fY;



	// Check for players Input 
	// Notes: (May need to be moved around function call for proper checks)
	this->Input(fElapsedTime);
	
	// Grappling hook
	// Notes: will probably get its own separate function
	if(this->m_pHook)
	{
		tVector2D vHook;

		// If player is swinging
		if(this->m_pHook->GetIfHooked() && !this->GetOnGround())// && this->GetRotation() <  3.14f/2&& this->GetRotation() > -3.14f*2)
		{
			//////////////////////////////////////////////////////////////////////////////
			// Adjust hook to player doesnt pass boundaries and he doesnt just stay still in the air
			//////////////////////////////////////////////////////////////////////////////
			
			if(this->GetPosX() > (this->m_pHook->GetPosX() - 2.0f) 
				&& this->GetPosX() < (this->m_pHook->GetPosX() + 2.0f) && this->m_bFixSwing)
			{
				this->SetRotation(0.0f);
				//this->m_bFirstSwing = true;
				this->m_bAllowSwing = true;
			}
			else if(this->GetPosX() < this->m_pHook->GetPosX() 
				&& this->m_bFixSwing && ((this->GetPosY() + this->GetHeight()) < this->m_fHookOffset - 10))
			{
				this->SetRotation(this->GetRotation() - (m_fRotationFixRate*fElapsedTime));
				//if(this->m_fRotationFixRate != 0.0f)
					//this->m_fRotationFixRate -= 0.001f;
			}
			else if(this->GetPosX() > this->m_pHook->GetPosX() && this->m_bFixSwing
				&& ((this->GetPosY() + this->GetHeight()) < this->m_fHookOffset - 10))
			{
				this->SetRotation(this->GetRotation() + (m_fRotationFixRate*fElapsedTime));
				//if(this->m_fRotationFixRate != 0.0f)
					//this->m_fRotationFixRate -= 0.001f;
			}
			//if(this->GetPosY() < (this->m_pHook->GetPosY() + 5.0f) && this->GetPosX() < (this->m_pHook->GetPosX()-5.0f))
			//{
			//	this->SetRotation(this->GetRotation() - m_fRotationFixRate*fElapsedTime);
			//	//this->SetPosY(this->m_pHook->GetPosY() + 1.0f);
			//}
			//if(this->GetPosY() < (this->m_pHook->GetPosY() + 5.0f) && this->GetPosX() > (this->m_pHook->GetPosX()+5.0f))
			//{
			//	this->SetRotation(this->GetRotation() + m_fRotationFixRate*fElapsedTime);
			//	//this->SetPosY(this->m_pHook->GetPosY() + 1.0f);
			//}
			/*else if( this->m_bFixSwing)
			{
				this->SetRotation(0.0f);
			}*/
			//////////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////////
			// Create vectors necessary for swinging
			//////////////////////////////////////////////////////////////////////////////

			this->m_vVectorVelocity.fX = this->GetPosX();
			this->m_vVectorVelocity.fY = this->GetPosY();
			vHook.fX = this->m_pHook->GetPosX() + this->m_pHook->GetWidth()/2;
			vHook.fY = this->m_pHook->GetPosY();

			//this->m_vVectorVelocity = Vector2DNormalize(this->m_vVectorVelocity);

			this->m_vVectorVelocity = this->m_vVectorVelocity - vHook;
			//////////////////////////////////////////////////////////////////////////////
			


			//////////////////////////////////////////////////////////////////////////////
			// Rotate hooks vector by either player input or players update
			//////////////////////////////////////////////////////////////////////////////
			this->m_vVectorVelocity = Vector2DRotate(this->m_vVectorVelocity, this->GetRotation());
			//////////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////////
			// Set players final velocity for when he lets go of hook
			//////////////////////////////////////////////////////////////////////////////
			this->m_vSpeed.fY = -this->m_vVectorVelocity.fY;
			//////////////////////////////////////////////////////////////////////////////

		
			//////////////////////////////////////////////////////////////////////////////
			// Update player to new position caused by swinging
			//////////////////////////////////////////////////////////////////////////////
			this->SetPosX(this->m_vVectorVelocity.fX + vHook.fX);
			this->SetPosY(this->m_vVectorVelocity.fY + vHook.fY);
			//////////////////////////////////////////////////////////////////////////////

		}
		else
		{
			//////////////////////////////////////////////////////////////////////////////
			// If player isnt swinging just update him like a real boy
			//////////////////////////////////////////////////////////////////////////////
			this->SetBaseVelX(this->m_vSpeed.fX);
			//////////////////////////////////////////////////////////////////////////////
		}


	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////
		// If no Hook even exists reset players rotation and update regularly
		//////////////////////////////////////////////////////////////////////////////
		//this->SetRotation(0.0f);
		this->SetBaseVelX(this->m_vSpeed.fX);
		//////////////////////////////////////////////////////////////////////////////
	}


	


	//////////////////////////////////////////////////////////////////////////////
	// check to see if player is on the Air, Jumping, or on a ground type object
	//////////////////////////////////////////////////////////////////////////////
	if(this->m_bOnGround)
	{
		//////////////////////////////////////////////////////////////////////////////
		// If player is on ground do this	
		//////////////////////////////////////////////////////////////////////////////
		this->m_vSpeed.fY = 0.0f;
		this->SetBaseVelY(this->m_vSpeed.fY);
		this->m_bJumped = false;
	//	if(m_vSpeed.fX < 0.0f)
			this->SetRotation(0.0);
	/*	else if(m_vSpeed.fX > 0.0f)
			this->SetRotation(-0.0f);*/

		//////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////
		// If hook exists and player is on a ground object
		//////////////////////////////////////////////////////////////////////////////
		if(this->m_pHook && this->m_bOnGround)
		{
			//////////////////////////////////////////////////////////////////////////////
			// if hook is not hooked on to solid object jump?
			//////////////////////////////////////////////////////////////////////////////
			if(!this->m_pHook->GetIfHooked())
			{
				if(this->m_vSpeed.fY <550.0f)
					this->m_vSpeed.fY = this->m_vSpeed.fY +( this->m_fGravity * fElapsedTime);
				this->SetBaseVelY(this->m_vSpeed.fY);
			}
			//////////////////////////////////////////////////////////////////////////////
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////////
			// Other wise jump anyways?
			//////////////////////////////////////////////////////////////////////////////
			if(this->m_vSpeed.fY <550.0f)
				this->m_vSpeed.fY = this->m_vSpeed.fY +( this->m_fGravity * fElapsedTime);
			this->SetBaseVelY(this->m_vSpeed.fY);
			//////////////////////////////////////////////////////////////////////////////
		}
		
	}



	
	
	//////////////////////////////////////////////////////////////////////////////
	// Players hand rotation thingy majigies
	//////////////////////////////////////////////////////////////////////////////
	if(CSinglePlayerState::GetInstance()->GetInputType() == 0)
	{
		// Create Hand Vector
		tVector2D vecHandRotation;
		vecHandRotation.fX = 0.0f;
		vecHandRotation.fY = -1.0f;

		//vecHandRotation = Vector2DRotate( vecHandRotation,)
		// Create Mouse Vector
		tVector2D vecMouseVector;
		vecMouseVector.fX = CSGD_DirectInput::GetInstance()->MouseGetPosX() - GetPosX() + CCamera::GetInstance()->GetOffsetX();
		vecMouseVector.fY = CSGD_DirectInput::GetInstance()->MouseGetPosY() - GetPosY() + CCamera::GetInstance()->GetOffsetY();

		// Find Initial rotation
		m_fHandRotation = AngleBetweenVectors( vecHandRotation, vecMouseVector );

		// Calculate final rotation for players hand
		if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < GetPosX()  - CCamera::GetInstance()->GetOffsetX() )
			m_fHandRotation = 2*SGD_PI - m_fHandRotation;
	}
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////
	// Camera boundary checking
	// NOTES: WIll have to be altered for 
	//		  Release and new levels
	////////////////////////////////////////
	CCamera::GetInstance()->SetCameraOffsetX(int(this->GetPosX()-400));

	int temp_Width = (CMapLoad::GetInstance()->m_gTileMap.m_nColumns * CMapLoad::GetInstance()->m_gTileMap.m_nWidth) - 800;
	int temp_Height = (CMapLoad::GetInstance()->m_gTileMap.m_nRows * CMapLoad::GetInstance()->m_gTileMap.m_nHeight) - 600;

	if(CCamera::GetInstance()->GetOffsetX() < 0)
		CCamera::GetInstance()->SetCameraOffsetX(0);
	else if(CCamera::GetInstance()->GetOffsetX() > temp_Width)
		CCamera::GetInstance()->SetCameraOffsetX(temp_Width);

	if( fabs(CCamera::GetInstance()->GetOffsetY()-(GetPosY()-300)) >= 6 || m_bJumped || m_bBoosting)
		CCamera::GetInstance()->SetCameraOffsetY(int(this->GetPosY()-(300)));

	if(CCamera::GetInstance()->GetOffsetY() < 0)
		CCamera::GetInstance()->SetCameraOffsetY(0);
	else if(CCamera::GetInstance()->GetOffsetY() > temp_Height)
		CCamera::GetInstance()->SetCameraOffsetY(temp_Height);


	this->m_pHud->Update( fElapsedTime ); 
	////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////
	// Update Players Animations
	//////////////////////////////////////////////////////////////////////////////
	GetAnimations()->Update( fElapsedTime );
	//////////////////////////////////////////////////////////////////////////////


	static bool bEnd = false;
 	if(this->GetHealth() <= 0.0f)
	{
		if(bEnd)
		{
			CSinglePlayerState::GetInstance()->SetDeath(true);
		}

		if(this->m_pHook)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this->m_pHook, this));
		}
		bEnd = true;
	}
	else
	{
		bEnd = false;
	}



	
	//////////////////////////////////////////////////////////////////////////////
	// Check for when player has progressed to a new level or area
	//////////////////////////////////////////////////////////////////////////////
	/*if(CSinglePlayerState::GetInstance()->GetCurrentLevel() == TUTORIAL)
	{
		if(this->GetPosX() > 2490 && this->GetPosY() > 638 && this->GetPosY() < 900)
		{
			CSinglePlayerState::GetInstance()->SetCurrentLevel(LEVEL1);
			CSinglePlayerState::GetInstance()->SetPreviousLevel(TUTORIAL);
			CSinglePlayerState::GetInstance()->SetChangeLevel(true);
		}
		if(this->GetPosX() > 1980 && this->GetPosX() < 2240 && this->GetPosY() > 1407)
		{
			CSinglePlayerState::GetInstance()->SetCurrentLevel(LEVEL2);
			CSinglePlayerState::GetInstance()->SetPreviousLevel(TUTORIAL);
			CSinglePlayerState::GetInstance()->SetChangeLevel(true);
		}
	}
	if(CSinglePlayerState::GetInstance()->GetCurrentLevel() == LEVEL1)
	{
		if(this->GetPosX() < 0 && this->GetPosY() > 638 && this->GetPosY() < 900)
		{
			CSinglePlayerState::GetInstance()->SetCurrentLevel(TUTORIAL);
			CSinglePlayerState::GetInstance()->SetPreviousLevel(LEVEL1);
			CSinglePlayerState::GetInstance()->SetChangeLevel(true);
		}
	}
	if(CSinglePlayerState::GetInstance()->GetCurrentLevel() == LEVEL2)
	{
		if(this->GetPosX() < 0 && this->GetPosY() < 0 )
		{
			CSinglePlayerState::GetInstance()->SetCurrentLevel(TUTORIAL);
			CSinglePlayerState::GetInstance()->SetPreviousLevel(LEVEL1);
			CSinglePlayerState::GetInstance()->SetChangeLevel(true);
		}
	}*/
	//////////////////////////////////////////////////////////////////////////////

	RECT rIntersect;
	RECT rTeleport = { 63*64, 38*64, 64*64, 39*64 };

	if(IntersectRect(&rIntersect, &rTeleport, &GetRect()))
	{
		SetPosX( 20*64 );
		SetPosY( 55*64 );
	}
	
	// 20 , 55
}

void CPlayer::Input(float fElapsedTime)
{

	//////////////////////////////////////////////////////////////////////////////
	// Check to see what weapon/equipment player changes too
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyPressed(CGame::GetInstance()->GetPlayerOneControls(6)) && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonDown(5) && this->m_fInputTime > 0.3f && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->m_fInputTime = 0.0f;
		switch(m_nSelectedWeapon)
		{
		case HAND_GUN:
			this->m_nSelectedWeapon = ROCKET_LAUNCHER;
			this->m_nWeaponIndex = this->RL;
			break;
		case ROCKET_LAUNCHER:
			this->m_nSelectedWeapon = FLAME_THROWER;
			this->m_nWeaponIndex = this->FT;
			break;
		case FLAME_THROWER:
			this->m_nSelectedWeapon = PLASMA_RIFLE;
			this->m_nWeaponIndex = this->PR;
			break;
		case PLASMA_RIFLE:
			this->m_nSelectedWeapon = STICKY_GRENADE;
			this->m_nWeaponIndex = this->SG;
			break;
		case STICKY_GRENADE:
			this->m_nSelectedWeapon = SONIC_RIFLE;
			this->m_nWeaponIndex = this->SR;
			break;
		case SONIC_RIFLE:
			this->m_nSelectedWeapon = HAND_GUN;
			this->m_nWeaponIndex = this->HG;
			break;
		};
	}

	if((CSGD_DirectInput::GetInstance()->JoystickButtonDown(4) && CSinglePlayerState::GetInstance()->GetInputType() == 1)
		|| (CSGD_DirectInput::GetInstance()->KeyDown(DIK_L) && CSinglePlayerState::GetInstance()->GetInputType() == 0 && m_fInputTime > 3.0f))
	{
		this->m_fInputTime = 0.0f;
		this->m_nSelectedBootSlot--;
		if(this->m_nSelectedBootSlot == -1)
		{
			this->m_nSelectedBootSlot = this->BOOTS;
		}
	
	}

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_1)&& CSinglePlayerState::GetInstance()->GetInputType() == 0)
	{
		this->m_nSelectedBootSlot = this->BOOTS;
		m_fGravity = 900.0f;
		m_bBoosting = false;
		m_bHovering = false;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_2)&& CSinglePlayerState::GetInstance()->GetInputType() == 0)
	{
		this->m_nSelectedBootSlot = this->HOVER_BOOTS;
		m_fGravity = 900.0f;
		m_bBoosting = false;
		m_bHovering = false;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_3)&& CSinglePlayerState::GetInstance()->GetInputType() == 0)
	{
		this->m_nSelectedBootSlot = this->ROCKET_BOOTS;
		m_fGravity = 900.0f;
		m_bBoosting = false;
		m_bHovering = false;
	}/*
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_4))
	{
		this->m_nSelectedWeapon = this->PLASMA_RIFLE;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_5))
	{
		this->m_nSelectedWeapon = this->STICKY_GRENADE;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_6))
	{
		this->m_nSelectedWeapon = this->SONIC_RIFLE;
	}
	*/

	/*if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_TAB))
	{
		this->m_nSelectedBootSlot--;
		if(this->m_nSelectedBootSlot < this->ROCKET_BOOTS)
		{
			this->m_nSelectedBootSlot = this->BOOTS;
		}
	}*/
	//if(CSGD_DirectInput::GetInstance()->MouseWheelMovement() >
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Reset animations to initial
	//////////////////////////////////////////////////////////////////////////////
	GetAnimations()->SetCurrentAnimation(1);
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Check input for grappling hook grapple towards or away from hook
	//////////////////////////////////////////////////////////////////////////////
	if(this->m_pHook)
	{
		if(this->GetPosY() < (this->m_pHook->GetPosY() + 3)
					&& this->m_pHook->GetIfHooked() && !this->GetOnGround())
		{
			this->SetPosY(this->GetPosY() + (0.5f*fElapsedTime));
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyDown(CGame::GetInstance()->GetPlayerOneControls(9)) &&CSinglePlayerState::GetInstance()->GetInputType() == 0)
		||((CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_UP) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_UP))
		&&  CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		if(this->m_pHook)
		{
			if(this->GetPosY() > (this->m_pHook->GetPosY() + 3) 
				&& this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				if(this->GetPosX() < this->m_pHook->GetPosX()  && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = true;
					this->SetPosX(this->GetPosX() + 75.0f * fElapsedTime);
					this->SetPosY(this->GetPosY() - 75.0f * fElapsedTime);
				}
				if(this->GetPosX() > this->m_pHook->GetPosX() && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = true;
					this->SetPosX(this->GetPosX() - 75.0f * fElapsedTime);
					this->SetPosY(this->GetPosY() - 75.0f * fElapsedTime);
				}
			}
			
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyDown(CGame::GetInstance()->GetPlayerOneControls(10)) &&CSinglePlayerState::GetInstance()->GetInputType() == 0)
		||((CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_DOWN) || CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_DOWN))
		&&  CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		if(this->m_pHook)
		{
			if( this->GetPosY() < (this->m_pHook->GetPosY() + 500)
				&& this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				if(this->GetPosX() > this->m_pHook->GetPosX()  && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = true;
					this->SetPosX(this->GetPosX() - 75.0f * fElapsedTime);
					this->SetPosY(this->GetPosY() + 75.0f * fElapsedTime);
				}
				if(this->GetPosX() < this->m_pHook->GetPosX() && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = true;
					this->SetPosX(this->GetPosX() + 75.0f * fElapsedTime);
					this->SetPosY(this->GetPosY() + 75.0f * fElapsedTime);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Check Input for Jumping
	//////////////////////////////////////////////////////////////////////////////

	static float fJumpSpeed = -600.0f;

	//static float fJumpSpeed = 0;//-550.0f;


	if((CSGD_DirectInput::GetInstance()->KeyPressed(CGame::GetInstance()->GetPlayerOneControls(0)) && 
		CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonPressed(1) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{		
		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && !this->m_bBoosting && this->m_fRemainingEnergy > 0.0f)
		{
			this->DecrementEnergy(5.0f);
		}
		if((this->m_nSelectedBootSlot == this->HOVER_BOOTS || this->m_nSelectedBootSlot == this->ROCKET_BOOTS) && this->m_bJumped)
		{
			this->m_bBoosting = true;
  			this->m_bHovering = true;

			if(m_nSelectedBootSlot == this->ROCKET_BOOTS)
			{
				CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(FLYROCKET), DSBPLAY_LOOPING);
			}
		}
		
		if(m_vSpeed.fX < 0.0f)
			this->SetRotation(0.005f);
		else if(m_vSpeed.fX > 0.0f)
			this->SetRotation(-0.005f);
		
		this->m_bJumped = true;
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_SPACE) && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonDown(1) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		if(this->m_nSelectedBootSlot == this->HOVER_BOOTS&& this->m_bHovering && this->m_vSpeed.fY >= 0.0f 
			&& !this->m_bOnGround && m_fRemainingEnergy != 0.0f)
		{
            this->m_fGravity = 50;
			this->DecrementEnergy(10*fElapsedTime);
		}
		
		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && this->m_bBoosting && this->m_fRemainingEnergy > 0.0f)
		{
			this->m_fGravity = 0.0f;

			fJumpSpeed = -100.0f;
			this->m_vSpeed.fY = fJumpSpeed;
			this->SetBaseVelY(this->m_vSpeed.fY);

			this->m_fRemainingEnergy -= 25.0f * fElapsedTime;

		}
		else if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && this->m_bBoosting && this->m_fRemainingEnergy <= 0.0f)
		{

			fJumpSpeed = -600.0f;

			this->m_bHovering = false;
			this->m_bBoosting = false;
			this->m_fGravity = 900.0f;
		}

		
		if(this->m_bOnGround && !this->m_bHovering && !this->m_bBoosting && this->m_fRemainingEnergy > 1.0f)
		{
			this->m_fGravity = 900.0f;
 			this->m_pMovingBlock = NULL;
			this->m_bOnGround = false;
			this->m_bOnMovingPlatform = false;
			this->m_vSpeed.fY = fJumpSpeed;
			this->SetBaseVelY(this->m_vSpeed.fY);
			this->m_bJumped = true;
		}
		
		
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyReleased(DIK_SPACE) && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonReleased(1) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
			fJumpSpeed = -600.0f;

			if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS)
				CSGD_WaveManager::GetInstance()->Stop(CSinglePlayerState::GetInstance()->GetSFX(FLYROCKET));
			
			this->m_bHovering = false;
			this->m_bBoosting = false;
			this->m_fGravity = 900.0f;
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Check Input for player movement (Grappling hook included)
	//////////////////////////////////////////////////////////////////////////////
	static int nMoveSpeed = 10;
	if(this->m_fBoostTime > 0.1f && this->m_nSelectedBootSlot == this->ROCKET_BOOTS)
	{
   		this->m_vSpeed.fX = 0.0f;
		this->m_fBoostTime = 0.0f;
		//this->m_bDash = false;
	}
		
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_A)  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| ((CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_LEFT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT))&& CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		if(this->m_vSpeed.fX > -300.0f)
		{
			this->m_vSpeed.fX -= nMoveSpeed;
		}

		this->m_pMovingBlock = NULL;
		//this->m_bJoyMove = true;
		this->m_bOnMovingPlatform = false;


		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->m_bAllowSwing)
			{
				if(this->GetRotation() < 0.0f && this->m_bRotFoward)
				{
					this->m_pHook->SetRotation(-this->m_pHook->GetRotation());
					this->m_bRotFoward = false;
				}
				
				this->m_bRotBackward = true;
				
				if(this->GetRotation() < 0.035f)
				{
					this->m_pHook->SetRotation(this->m_pHook->GetRotation() + (this->m_pHook->GetRotationRate()*fElapsedTime));
				}
				
				/*if(this->m_bFirstSwing == true && 
					(this->GetPosX() > (this->m_pHook->GetPosX() + 100) || this->GetPosX() < this->m_pHook->GetPosX() - 100))
				{
					this->m_pHook->SetRotation(0.0f);
					this->m_bFirstSwing = false;
					this->m_bAllowSwing = false;
				}*/

				if( this->GetPosY() < (this->m_pHook->GetPosY() + this->m_pHook->GetSwingMax()))
				{
					this->m_pHook->SetRotation(0.0f);
					this->m_bAllowSwing = false;
				}
				if(!this->m_bOnGround && this->m_bAllowSwing)
				{
					this->m_bFixSwing = false;
					this->SetRotation(this->m_pHook->GetRotation());
				}
				else if(!this->m_bOnGround && !this->m_bAllowSwing)
				{
					this->m_bFixSwing = true;
					this->SetRotation(this->m_pHook->GetRotation());
				}
			}
		}

		GetAnimations()->SetCurrentAnimation(0);
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyDown(CGame::GetInstance()->GetPlayerOneControls(2))  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| ((CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_RIGHT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT)) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		if(this->m_vSpeed.fX < 300.0f)
		{
			this->m_vSpeed.fX += nMoveSpeed;
		}
		this->m_pMovingBlock = NULL;
		this->m_bOnMovingPlatform = false;

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->m_bAllowSwing)
			{
				if(this->GetRotation() > 0.0f && this->m_bRotBackward)
				{
					this->m_pHook->SetRotation(-this->m_pHook->GetRotation());
					this->m_bRotBackward = false;
				}
				this->m_bRotFoward = true;
				if(this->GetRotation() > -0.035f)
				{
					this->m_pHook->SetRotation(this->m_pHook->GetRotation() - (this->m_pHook->GetRotationRate()*fElapsedTime));
				}
				/*if(this->m_bFirstSwing == true && 
					(this->GetPosX() > (this->m_pHook->GetPosX() + 100) || this->GetPosX() < this->m_pHook->GetPosX() - 100))
				{
					this->m_pHook->SetRotation(0.0f);
					this->m_bFirstSwing = false;
					this->m_bAllowSwing = false;
				}*/
				if( this->GetPosY() < (this->m_pHook->GetPosY() + this->m_pHook->GetSwingMax()))
				{
					this->m_pHook->SetRotation(0.0f);
					this->m_bAllowSwing = false;
				}
				if(!this->m_bOnGround && this->m_bAllowSwing)
				{
					this->m_bFixSwing = false;
					this->SetRotation(this->m_pHook->GetRotation());
				}
				else if(!this->m_bOnGround && !this->m_bAllowSwing)
				{
					this->m_bFixSwing = true;
					this->SetRotation(this->m_pHook->GetRotation());
				}
			}
		}

		GetAnimations()->SetCurrentAnimation(0);
	}

	if((CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT) 
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT)) && CSinglePlayerState::GetInstance()->GetInputType() == 1)
	{
		this->m_bJoyMove = true;
	}
	
	//////////////////////////////////////////////////////////////////////////////
	//if(CSGD_DirectInput::GetInstance()->KeyReleased(CGame::GetInstance()->GetPlayerOneControls(2)))
	if((this->m_fBoostTime > 0.05f && (this->m_bFDash||this->m_bBDash))|| this->m_fWaitTime > 0.25f) 
	{
		this->m_bFDash = false;
		this->m_bBDash = false;
		this->m_fBoostTime = 0.0f;
		this->m_fWaitTime = 0.0f;
		//this->m_vSpeed.fX = 0.0f;
	}
	
	if((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_A)  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		||( CSGD_DirectInput::GetInstance()->JoystickDPadPressed(DIR_LEFT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_LEFT) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		/*if(this->m_bOnGround)
		{
			CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(RUNNING), DSBPLAY_LOOPING);
		}*/

		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && this->m_bBDash && this->m_fRemainingEnergy > 20.0f)
		{
			this->m_vSpeed.fX = -this->m_fDash;
			this->m_bBDash = false;
			this->DecrementEnergy(30.0f);
		}
		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && !this->m_bBDash)
		{
			this->m_bBDash = true;
		}
		/*else
		{
			this->m_bDash = false;
		}*/
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_D)  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| ((CSGD_DirectInput::GetInstance()->JoystickDPadPressed(DIR_RIGHT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirPressed(DIR_RIGHT)) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		/*if(this->m_bOnGround)
		{
			CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(RUNNING), DSBPLAY_LOOPING);
		}*/

		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && this->m_bFDash && this->m_fRemainingEnergy > 20.0f)
		{
			this->m_vSpeed.fX = this->m_fDash;
			this->m_bFDash = false;
			this->DecrementEnergy(30.0f);
		}
		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && !this->m_bFDash)
		{
			this->m_bFDash = true;
		}
		/*else
		{
			this->m_bDash = false;
		}*/
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyReleased(DIK_D)  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickDPadReleased(DIR_RIGHT) || 
		(CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount() > -15.0f 
		&& CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount() < 15.0f && this->m_bJoyMove) 
		&& CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->m_vSpeed.fX = 0.0f;
		this->SetBaseVelX(0.0f);
		this->m_bJoyMove = false;

	//	CSGD_WaveManager::GetInstance()->Stop(CSinglePlayerState::GetInstance()->GetSFX(RUNNING));

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				this->m_vSpeed.fX = 0.0f;
				this->m_bFixSwing = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyReleased(CGame::GetInstance()->GetPlayerOneControls(1)) && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickDPadReleased(DIR_LEFT) || 
		(CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount() > -15.0f 
		&& CSGD_DirectInput::GetInstance()->JoystickGetLStickXAmount() < 15.0f && this->m_bJoyMove) 
		&& CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->SetBaseVelX(0.0f);
		this->m_vSpeed.fX = 0.0f;
		this->m_bJoyMove = false;

		//CSGD_WaveManager::GetInstance()->Stop(CSinglePlayerState::GetInstance()->GetSFX(RUNNING));
		
		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				this->m_vSpeed.fX = 0.0f;
				this->m_bFixSwing = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////


	
	




	//////////////////////////////////////////////////////////////////////////////
	// Check for secondary fire
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(CGame::GetInstance()->GetPlayerOneControls(4))  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonPressed(6) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->SetMouseDown(1);
		if(/*CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook &&*/ !this->m_bHookShot)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateHookMessage(this));
			this->m_bHookShot = true;
			this->m_bFixSwing = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonReleased(CGame::GetInstance()->GetPlayerOneControls(4)) && CSinglePlayerState::GetInstance()->GetInputType() == 0) 
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonReleased(7) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->SetMouseDown(0);
		if(this->m_pHook)
		{
			this->m_pHook->SetHooked(0);
		}
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this->m_pHook, this));
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Check for primary fire
	//////////////////////////////////////////////////////////////////////////////
		if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(CGame::GetInstance()->GetPlayerOneControls(3)) && CSinglePlayerState::GetInstance()->GetInputType() == 0) 
			|| (CSGD_DirectInput::GetInstance()->JoystickButtonPressed(7) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
		{
			switch(this->m_nSelectedWeapon)
			{
			case this->HAND_GUN:
				CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(HGUN));
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateBulletMessage(this));
				break;
			case this->ROCKET_LAUNCHER:
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
				break;
			case this->PLASMA_RIFLE:
				CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(PLASMA_SHOT));
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreatePlasmaMessage(this));
				break;
			case this->STICKY_GRENADE:
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateGrenadeMessage(this));
				break;
			case this->FLAME_THROWER:
				if(m_fRemainingEnergy > 5 && m_fFireDelay >= 0.07f)
				{
					CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(FLAMETHROWER), DSBPLAY_LOOPING);
					m_fFireDelay = 0.0f;
				}
				else
					CSGD_WaveManager::GetInstance()->Stop(CSinglePlayerState::GetInstance()->GetSFX(FLAMETHROWER));
					
				break;
			
			};
		}
	//////////////////////////////////////////////////////////////////////////////
		if((CSGD_DirectInput::GetInstance()->MouseButtonDown(CGame::GetInstance()->GetPlayerOneControls(3))  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
			|| (CSGD_DirectInput::GetInstance()->JoystickButtonDown(7) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
		{
			if(this->m_nSelectedWeapon == this->FLAME_THROWER)
			{
				m_fFireDelay += fElapsedTime;
				if(m_fRemainingEnergy > 5 && m_fFireDelay >= 0.07f)
				{
					CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateFlameMessage(this));
					DecrementEnergy(1.0f);
				}
			}
			if(this->m_nSelectedWeapon == this->SONIC_RIFLE)
			{
				if(fElapsedTime > 0.01f && this->m_nCharge <= 100)
				{
					this->m_nCharge++;
				}
			}
		}
	//////////////////////////////////////////////////////////////////////////////
		if((CSGD_DirectInput::GetInstance()->MouseButtonReleased(CGame::GetInstance()->GetPlayerOneControls(3))  && CSinglePlayerState::GetInstance()->GetInputType() == 0)
			|| (CSGD_DirectInput::GetInstance()->JoystickButtonReleased(7) && CSinglePlayerState::GetInstance()->GetInputType() == 1))
		{
			if(this->m_nSelectedWeapon == this->SONIC_RIFLE)
			{
				CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateShockMessage(this));
		
				CSGD_WaveManager::GetInstance()->Play(CSinglePlayerState::GetInstance()->GetSFX(SHOCK_WAVE));
			}

			if(this->m_nSelectedWeapon == this->FLAME_THROWER)
			{
				CSGD_WaveManager::GetInstance()->Stop(CSinglePlayerState::GetInstance()->GetSFX(FLAMETHROWER));
			}

		}
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Check to see if homing is turned on
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyPressed(CGame::GetInstance()->GetPlayerOneControls(7)) && CSinglePlayerState::GetInstance()->GetInputType() == 0)
		|| (CSGD_DirectInput::GetInstance()->JoystickButtonPressed(3) &&CSinglePlayerState::GetInstance()->GetInputType() == 1))
	{
		this->m_bHomingOn = !this->m_bHomingOn;
	}
	//////////////////////////////////////////////////////////////////////////////

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

	//////////////////////////////////////////////////////////////////////////////
	// Check for Gamepad input
	//////////////////////////////////////////////////////////////////////////////
	if(CSinglePlayerState::GetInstance()->GetInputType())
	{
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = 0.0f;
			this->m_bForward = 1;
			this->m_fHandRotation = SGD_PI/2;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = 0.0f;
			this->m_bForward = 0;
			this->m_fHandRotation = (3*SGD_PI)/2;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP))
		{
			this->m_vJoyVecPos.fX = 0.0f;
			this->m_vJoyVecPos.fY = -1.0f;
			this->m_fHandRotation = 2*SGD_PI;
		}
		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN))
		{
			this->m_vJoyVecPos.fX = 0.0f;
			this->m_vJoyVecPos.fY = 1.0f;
			this->m_fHandRotation = SGD_PI;
		}

		if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = 1.0f;
			this->m_bForward = 1;
			this->m_fHandRotation = (3*SGD_PI)/4;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_DOWN) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = 1.0f;
			this->m_bForward = 0;
			this->m_fHandRotation = (5*SGD_PI)/4;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_RIGHT))
		{
			this->m_vJoyVecPos.fX = 1.0f;
			this->m_vJoyVecPos.fY = -1.0f;
			this->m_bForward = 1;
			this->m_fHandRotation = SGD_PI/4;
		}
		else if(CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_UP) && CSGD_DirectInput::GetInstance()->JoystickGetRStickDirDown(DIR_LEFT))
		{
			this->m_vJoyVecPos.fX = -1.0f;
			this->m_vJoyVecPos.fY = -1.0f;
			this->m_bForward = 0;
			this->m_fHandRotation = (7*SGD_PI)/4;
		}

	}
	//////////////////////////////////////////////////////////////////////////////




}

void CPlayer::Render(void)
{
	//////////////////////////////////////////////////////////////////////////////
	// Access Camera Offset
	//////////////////////////////////////////////////////////////////////////////
	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Render homing light if on
	//////////////////////////////////////////////////////////////////////////////
	if(m_bHomingOn)
		CSGD_Direct3D::GetInstance()->DrawLine( (int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale()),
		(int)(((GetPosY() + (GetHeight()/4)) - OffsetY) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosX()+8) * CCamera::GetInstance()->GetScale()), 
		int((CSGD_DirectInput::GetInstance()->MouseGetPosY()+8) * CCamera::GetInstance()->GetScale()), 
		255, 0, 0 );

	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Check to see which direction player should face and render
	//////////////////////////////////////////////////////////////////////////////
	if(this->m_bForward)
		GetAnimations()->Render( (int)GetPosX()+GetAnimations()->GetFrameWidth()/2, (int)GetPosY()+GetAnimations()->GetFrameHeight());
	else
		GetAnimations()->Render( (int)GetPosX()+3*GetAnimations()->GetFrameWidth()/2, (int)GetPosY()+GetAnimations()->GetFrameHeight(), -1.0f );
	//////////////////////////////////////////////////////////////////////////////

	//RECT rRender = { 340, 164, 550, 234 };

	//////////////////////////////////////////////////////////////////////////////
	// Draw players Hand/weapons
	//////////////////////////////////////////////////////////////////////////////
	RECT _currentFrame = GetAnimations()->GetCollisionFrame((int)GetPosX(), (int)GetPosY());
	if(this->m_bForward)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
			(int)((_currentFrame.left + m_ptArmOffsets[m_nWeaponIndex].m_nX + GetAnimations()->GetPivotPoint().m_nX - OffsetX) * CCamera::GetInstance()->GetScale()), 
			(int)((_currentFrame.top + m_ptArmOffsets[m_nWeaponIndex].m_nY + GetAnimations()->GetPivotPoint().m_nY - OffsetY) * CCamera::GetInstance()->GetScale()), 
			0.7f * CCamera::GetInstance()->GetScale(), 0.7f * CCamera::GetInstance()->GetScale(), 
			&this->m_rWeapons[m_nWeaponIndex], (float)m_ptPivots[m_nWeaponIndex].m_nX, (float)m_ptPivots[m_nWeaponIndex].m_nY, m_fHandRotation, -1 );
}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
			(int)((_currentFrame.left - m_ptArmOffsets[m_nWeaponIndex].m_nX + (GetAnimations()->GetFrameWidth() - GetAnimations()->GetPivotPoint().m_nX) - OffsetX) * CCamera::GetInstance()->GetScale()), 
			(int)((_currentFrame.top + m_ptArmOffsets[m_nWeaponIndex].m_nY + GetAnimations()->GetPivotPoint().m_nY - OffsetY) * CCamera::GetInstance()->GetScale()), 
			-0.7f * CCamera::GetInstance()->GetScale(), 0.7f * CCamera::GetInstance()->GetScale(), 
			&this->m_rWeapons[m_nWeaponIndex], (float)m_ptPivots[m_nWeaponIndex].m_nX, (float)m_ptPivots[m_nWeaponIndex].m_nY, m_fHandRotation, -1 );

	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Draw Gameplay HUD
	//////////////////////////////////////////////////////////////////////////////
	this->m_pHud->Render();

	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Render charge bar for sonic rifle
	//////////////////////////////////////////////////////////////////////////////
	if(this->m_nSelectedWeapon == this->SONIC_RIFLE)
	{
		RECT rCharge;
		rCharge.left = (LONG)(64 *9);
		rCharge.top  = (LONG)64;
		rCharge.right = rCharge.left + 64;
		rCharge.bottom = rCharge.top +(LONG)(70 *(this->m_fChargeRate*this->m_nCharge));
		CSGD_TextureManager::GetInstance()->Draw(CSinglePlayerState::GetInstance()->GetWeaponSelectionID(), 80, (int)(600-(70 *(this->m_fChargeRate*this->m_nCharge))-60),
			1.0f,1.0f,&rCharge);
	}
	//////////////////////////////////////////////////////////////////////////////
}

RECT CPlayer::GetRect(void) const
{
	RECT rCollision;
	rCollision.top = (LONG)(this->GetPosY());//- (GetHeight()>>1));
	rCollision.left = (LONG)(this->GetPosX() );//- (GetWidth()>>1));
	rCollision.bottom = rCollision.top + this->GetHeight();
	rCollision.right = rCollision.left + this->GetWidth();

	return rCollision;
}

bool CPlayer::CheckCollision(CBase* pBase)
{
	static RECT rPrevRect;
	static CPoint Offset;
	
	RECT rPlayerFrame = GetAnimations()->GetFrame();
	if(rPrevRect.bottom != rPlayerFrame.bottom)
	{
		Offset.m_nX = rPlayerFrame.right - rPrevRect.right;

		rPrevRect = rPlayerFrame;
	}

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
						m_bOnGround = true;
						m_bJumped = false;
						m_bOnPlatform = true;

						this->m_fHookOffset = (float)rMyRect.bottom;
							
						if( BLOCK->GetBlock() == BLOCK_MOVING)
						{
							if(!m_bOnMovingPlatform)
							{
								m_fMovingPlatformPosX = (float)rHisRect.left;
								m_pMovingBlock = BLOCK;
								m_bOnMovingPlatform = true;
							}
						}
						else if(BLOCK->GetBlock() == BLOCK_UNSTABLE)
						{
							BLOCK->SetStable(false);
						}
						CMapLoad::GetInstance()->m_bCollisionCheck = true;

					}
					else if(rMyRect.top < rHisRect.bottom && rMyRect.bottom > rHisRect.top)
					{
						this->m_vSpeed.fY = 0.0f;
						this->SetBaseVelY(0.0f);
						SetPosY((float)rHisRect.bottom);
					}

				}
				else if(BLOCK->GetBlock() == BLOCK_TRAP)
				{
					this->m_bOnGround = true;
					this->m_bJumped = false;
					this->m_bOnPlatform = true;
					this->DecrementHealth(20);
				}

			}
			else if((rIntersect.right-rIntersect.left) < (rIntersect.bottom-rIntersect.top))
			{
				if(BLOCK->GetBlock() == BLOCK_SOLID || BLOCK->GetBlock() == BLOCK_MOVING || BLOCK->GetBlock() == BLOCK_PARTIAL || BLOCK->GetBlock() == BLOCK_UNSTABLE)
				{
					if(rMyRect.right > rHisRect.left && rMyRect.left < rHisRect.left)
						SetPosX( (float)rHisRect.left - GetWidth() );
					else if(rMyRect.left < rHisRect.right && rMyRect.right > rHisRect.left)
						SetPosX( (float)rHisRect.right);
				}
				else if(BLOCK->GetBlock() == BLOCK_TRAP)
				{
					m_bOnGround = true;
					m_bJumped = false;
					m_bOnPlatform = true;
					DecrementHealth(20);
				}
			}
			return true;
		}
	}
	else if(pBase->GetType() == OBJ_PICKUP) 
	{
		RECT rIntersect;
		if(IntersectRect(&rIntersect, &GetRect(), &(pBase->GetRect())))
		{
			CSinglePlayerState::GetInstance()->SetProfileValues(1);
			return true;
		}
	}
	
	if(m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
	{
		m_bOnGround = false;
		m_bOnPlatform = false;
		m_bOnMovingPlatform = false;
	}

	return false;
}

void CPlayer::HandleEvent(CEvent* pEvent)
{

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

tVector2D CPlayer::GetSpeed(void)	{ return m_vSpeed; }

float	CPlayer::GetRotationRate(void)	{ return m_fRotationRate; }

CPoint CPlayer::GetBulletStartPos( void )
{
	CPoint ptStartingPos;
	tVector2D vecStartingPoint;	
	RECT _currentFrame = GetAnimations()->GetCollisionFrame((int)GetPosX(), (int)GetPosY());

	vecStartingPoint.fX = 0;
	vecStartingPoint.fY = m_fStartPosRadius[m_nWeaponIndex];

	vecStartingPoint = Vector2DRotate( vecStartingPoint, m_fHandRotation );

	ptStartingPos.m_nX = (int)vecStartingPoint.fX + (int)((_currentFrame.left + GetAnimations()->GetPivotPoint().m_nX ) * CCamera::GetInstance()->GetScale());
	ptStartingPos.m_nY = (int)vecStartingPoint.fY + (int)((_currentFrame.top + GetAnimations()->GetPivotPoint().m_nY ) * CCamera::GetInstance()->GetScale());

	return ptStartingPos;
}