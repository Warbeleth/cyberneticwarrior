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
	this->m_bOnMovingPlatform = false;
	this->m_bJumped = false;
	this->m_bHovering =false;

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
	this->m_vRotationCenter.fX = this->GetPosX()+this->GetWidth();
	this->m_vRotationCenter.fY = 0.0f;
	this->m_vVectorVelocity.fX = 0.0f;
	this->m_vVectorVelocity.fY = 0.0f;
	this->m_vJoyVecPos.fX	   = 0.0f;
	this->m_vJoyVecPos.fY	   = 0.0f;
	this->m_fJoyRot = 0.0f;
	this->m_fWaitTime = 0.0f;
	this->m_fGravity = 900.0f;

	// Moving Platform Position Updates
	this->m_fMovingPlatformPosX = 0.0f;

	// sonic rifle stuff
	this->m_nCharge = 0;
	this->m_fChargeRate = 0.01f;


	// Health
	m_nRemainingHealth = 100;
	m_nTotalHealth = 100;

	// Energy
	m_nRemainingEnergy = 0;
	m_nTotalEnergy = 100;

	// Score
	m_nScore = 0;

	// Animation
	SetAnimations(new CAnimations());
	GetAnimations()->LoadBinary("resource/binary/PlayerAnimations.bae");
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

}

CPlayer::~CPlayer(void)
{
	this->m_bShutDown = false;
	this->m_bOnMovingPlatform = false;
	
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
	
	// sonic rifle stuff
	this->m_nCharge = 0;
	this->m_fChargeRate = 0.01f;

	delete m_pHud;
	delete GetAnimations();
	this->m_pHud = NULL;

	this->m_pMovingBlock = NULL;

	SetAnimations( NULL );
}


void CPlayer::Update(float fElapsedTime)
{
	// Check for players Input 
	// Notes: (May need to be moved around function call for proper checks)
	this->Input(fElapsedTime);


	// Check to see if game was paused if so destroy dynamic Items (example: Grappling hook)
	if(this->m_pHook && this->m_bShutDown)
	{
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this->m_pHook, this));
		this->m_bShutDown = false;
	}

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
	if(CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX() > this->GetPosX())
	{
		this->m_bForward = 1;
	}
	else
	{
		this->m_bForward = 0;
	}



	// used for gamepad controls
	// Notes: May be used later, probably not(Do not remove yet)

	//this->m_fWaitTime +=  fElapsedTime;

	//this->m_vJoyVecPos.fX = this->GetPosX();
	//this->m_vJoyVecPos.fY = this->GetPosY();
	//this->m_vJoyVecPos = Vector2DRotate(this->m_vJoyVecPos, this->m_fJoyRot);
	//this->m_vJoyVecPos.fX = this->GetPosX() + this->m_vJoyVecPos.fX;
	//this->m_vJoyVecPos.fY = this->GetPosY() - this->m_vJoyVecPos.fY;



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
			if(this->GetPosX() < this->m_pHook->GetPosX() && this->m_bFixSwing)
			{
				this->SetRotation(this->GetRotation() - 0.01f*fElapsedTime);
			}
			if(this->GetPosX() > this->m_pHook->GetPosX() && this->m_bFixSwing)
			{
				this->SetRotation(this->GetRotation() + 0.01f*fElapsedTime);
			}
			if(this->GetPosY() < this->m_pHook->GetPosY() + 10.0f && this->GetPosX() < this->m_pHook->GetPosX()-20.0f)
			{
				this->SetRotation(this->GetRotation() - 0.1f*fElapsedTime);
				//this->SetPosY(this->m_pHook->GetPosY() + 1.0f);
			}
			if(this->GetPosY() < this->m_pHook->GetPosY() + 10.0f && this->GetPosX() > this->m_pHook->GetPosX()+20.0f)
			{
				this->SetRotation(this->GetRotation() + 0.1f*fElapsedTime);
				//this->SetPosY(this->m_pHook->GetPosY() + 1.0f);
			}
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
			//D3DXToDegree(
			//this->SetRotation(/*this->GetRotation() + */AngleBetweenVectors(vHook, this->m_vVectorVelocity));
			//this->SetRotation(SGD_PI +(SGD_PI - this->GetRotation()));
			//this->m_vVectorVelocity = this->m_vVectorVelocity * 500.0f;// this->m_vSpeed.fX;
			this->m_vVectorVelocity = Vector2DRotate(this->m_vVectorVelocity, this->GetRotation());
			//this->SetBaseVelX(0.0f);
			//this->SetBaseVelY(0.0f);
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
			//this->m_vVectorVelocity.fX = 0.0f;

			//this->m_vVectorVelocity.fY = 0.0f;
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
		this->SetRotation(0.0f);
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
			this->m_vSpeed.fY = this->m_vSpeed.fY +( this->m_fGravity * fElapsedTime);
			this->SetBaseVelY(this->m_vSpeed.fY);
			//////////////////////////////////////////////////////////////////////////////
		}
	}


	//////////////////////////////////////////////////////////////////////////////
	// If player is lower that screen height clamp with to above ground
	// Notes: WILL HAVE TO BE REMOVED!@!#)!@
	//////////////////////////////////////////////////////////////////////////////
	if(this->GetPosY() > 600 - this->GetHeight())
	{
		this->m_bOnGround = 1;
		this->SetPosY(600 - (float)this->GetHeight());
	}
	//////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////
	// Players hand rotation thingy majigies
	//////////////////////////////////////////////////////////////////////////////

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
		m_fHandRotation = SGD_PI + (SGD_PI - m_fHandRotation);
	//////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////
	// Camera boundary checking
	// NOTES: WIll have to be altered for 
	//		  Release and new levels
	////////////////////////////////////////
	CCamera::GetInstance()->SetCameraOffsetX(int(this->GetPosX()-400));

	if(CCamera::GetInstance()->GetOffsetX() < 0)
		CCamera::GetInstance()->SetCameraOffsetX(0);

	CCamera::GetInstance()->SetCameraOffsetY(int(this->GetPosY()-430));

	if(CCamera::GetInstance()->GetOffsetY() < 0)
		CCamera::GetInstance()->SetCameraOffsetY(0);

	this->m_pHud->Update( fElapsedTime );
	////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Update Players Animations
	//////////////////////////////////////////////////////////////////////////////
	GetAnimations()->Update( fElapsedTime );
	//////////////////////////////////////////////////////////////////////////////
}

void CPlayer::Input(float fElapsedTime)
{

	//////////////////////////////////////////////////////////////////////////////
	// Check to see what weapon/equipment player changes too
	//////////////////////////////////////////////////////////////////////////////
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_1))
	{
		this->m_nSelectedWeapon = this->HAND_GUN;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_2))
	{
		this->m_nSelectedWeapon = this->ROCKET_LAUNCHER;
	}
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_3))
	{
		this->m_nSelectedWeapon = this->FLAME_THROWER;
	}
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
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_TAB))
	{
		this->m_nSelectedBootSlot--;
		if(this->m_nSelectedBootSlot > this->ROCKET_BOOTS)
		{
			this->m_nSelectedBootSlot = this->BOOTS;
		}
	}
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
	if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_W) )
	{
		if(this->m_pHook)
		{
			if(this->GetPosY() > this->m_pHook->GetPosY() + 30 && this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				if(this->GetPosX() < this->m_pHook->GetPosX())
				{
					this->m_bFixSwing = false;
					this->SetPosX(this->GetPosX() + 1.5f);
					this->SetPosY(this->GetPosY() - 1.5f);
				}
				if(this->GetPosX() > this->m_pHook->GetPosX() && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = false;
					this->SetPosX(this->GetPosX() - 1.5f);
					this->SetPosY(this->GetPosY() - 1.5f);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if(CSGD_DirectInput::GetInstance()->KeyDown(DIK_S) )
	{
		if(this->m_pHook)
		{
			if(this->GetPosY() > this->m_pHook->GetPosY() + 30 && this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				if(this->GetPosX() < this->m_pHook->GetPosX())
				{
					this->m_bFixSwing = false;
					this->SetPosX(this->GetPosX() - 1.5f);
					this->SetPosY(this->GetPosY() + 1.5f);
				}
				if(this->GetPosX() > this->m_pHook->GetPosX() && this->m_pHook->GetIfHooked() && !this->GetOnGround())
				{
					this->m_bFixSwing = false;
					this->SetPosX(this->GetPosX() + 1.5f);
					this->SetPosY(this->GetPosY() + 1.5f);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Check Input for Jumping
	//////////////////////////////////////////////////////////////////////////////
	static float fJumpSpeed = -550.0f;
	if((CSGD_DirectInput::GetInstance()->KeyPressed(DIK_SPACE)
		|| CSGD_DirectInput::GetInstance()->JoystickButtonPressed(1)))
	{
		if(this->m_nSelectedBootSlot == this->HOVER_BOOTS && this->m_bJumped)
		{
			this->m_bHovering = true;
		}
		
		
		if(!this->m_bJumped && this->m_bOnGround)
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
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_SPACE)
		|| CSGD_DirectInput::GetInstance()->JoystickButtonDown(1)) && !this->m_bOnGround)
	{
		if(this->m_nSelectedBootSlot == this->HOVER_BOOTS&& this->m_bHovering && this->m_vSpeed.fY > 0.0f)
		{
   			this->m_fGravity = 100.0f;
			this->m_bJumped = false;
		}
		
		if(this->m_nSelectedBootSlot == this->ROCKET_BOOTS && this->m_bJumped)
		{
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyReleased(DIK_SPACE)
		|| CSGD_DirectInput::GetInstance()->JoystickButtonReleased(1)))
	{
		if(this->m_nSelectedBootSlot == this->HOVER_BOOTS&& this->m_bHovering)
		{
			this->m_bHovering = false;
			this->m_fGravity = 900.0f;
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Check Input for player movement (Grappling hook included)
	//////////////////////////////////////////////////////////////////////////////
	static int nMoveSpeed = 10;
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_A) 
		|| CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_LEFT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_LEFT)))
	{
		if(this->m_vSpeed.fX > -200.0f)
		{
			this->m_vSpeed.fX -= nMoveSpeed;
		}
		this->m_pMovingBlock = NULL;
		this->m_bOnMovingPlatform = false;

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->GetPosY() > this->m_pHook->GetPosY())
			{
				this->m_pHook->SetRotation(this->m_pHook->GetRotation() + this->m_pHook->GetRotationRate() * fElapsedTime);
				if(!this->m_bOnGround)
				{
					this->m_bFixSwing = false;
					this->SetRotation(this->m_pHook->GetRotation());
				}
			}
			else if( this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->GetPosY() < this->m_pHook->GetPosY())
			{
				this->SetPosY(this->GetPosY() +1);
			}
		}

		GetAnimations()->SetCurrentAnimation(0);
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->KeyDown(DIK_D) 
		|| CSGD_DirectInput::GetInstance()->JoystickDPadDown(DIR_RIGHT)
		|| CSGD_DirectInput::GetInstance()->JoystickGetLStickDirDown(DIR_RIGHT)))
	{
		if(this->m_vSpeed.fX < 200.0f)
		{
			this->m_vSpeed.fX += nMoveSpeed;
		}
		this->m_pMovingBlock = NULL;
		this->m_bOnMovingPlatform = false;

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->GetPosY() > this->m_pHook->GetPosY())
			{
				this->m_pHook->SetRotation(this->m_pHook->GetRotation() - this->m_pHook->GetRotationRate() * fElapsedTime);
				if(!this->m_bOnGround)
				{
					this->m_bFixSwing = false;

					this->SetRotation(this->m_pHook->GetRotation());
				}
			}
			else if( this->m_pHook->GetIfHooked() && !this->GetOnGround() && this->GetPosY() < this->m_pHook->GetPosY())
			{
				this->SetPosY(this->GetPosY() +1);
			}
		}

		GetAnimations()->SetCurrentAnimation(0);
	}
	//////////////////////////////////////////////////////////////////////////////
	if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_D))
	{
		this->m_vSpeed.fX = 0.0f;
		this->SetBaseVelX(0.0f);

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				this->m_bFixSwing = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if(CSGD_DirectInput::GetInstance()->KeyReleased(DIK_A))
	{
		this->SetBaseVelX(0.0f);
		this->m_vSpeed.fX = 0.0f;

		if(this->m_pHook)
		{
			if(this->m_pHook->GetIfHooked() && !this->GetOnGround())
			{
				this->m_bFixSwing = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////


	
	




	//////////////////////////////////////////////////////////////////////////////
	// Check for secondary fire
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(MOUSE_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(4)))
	{
		this->SetMouseDown(1);
		if(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateHookMessage(this));
			this->m_bFixSwing = true;
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonReleased(MOUSE_RIGHT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(4)))
	{
		this->SetMouseDown(0);
		if(this->m_pHook)
		{
			this->m_pHook->SetHooked(0);
		}
		//CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CDestroyHookMessage(this));
	}
	//////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////
	// Check for primary fire
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonPressed(MOUSE_LEFT) || CSGD_DirectInput::GetInstance()->JoystickButtonPressed(7)))
	{
		switch(this->m_nSelectedWeapon)
		{
		case this->HAND_GUN:
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateBulletMessage(this));
			break;
		case this->ROCKET_LAUNCHER:
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateRocketMessage(this));
			break;
		case this->PLASMA_RIFLE:
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreatePlasmaMessage(this));
			break;
		case this->STICKY_GRENADE:
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateGrenadeMessage(this));
			break;
		
		};
	}
	//////////////////////////////////////////////////////////////////////////////
	if((CSGD_DirectInput::GetInstance()->MouseButtonDown(MOUSE_LEFT) || CSGD_DirectInput::GetInstance()->JoystickButtonDown(7)))
	{
		if(this->m_nSelectedWeapon == this->FLAME_THROWER)
		{
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateFlameMessage(this));
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
	if((CSGD_DirectInput::GetInstance()->MouseButtonReleased(MOUSE_LEFT) || CSGD_DirectInput::GetInstance()->JoystickButtonReleased(7)))
	{
		this->m_nCharge = 0;
		if(this->m_nSelectedWeapon == this->SONIC_RIFLE)
			CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateShockMessage(this));
	}
	//////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////
	// Check to see if homing is turned on
	//////////////////////////////////////////////////////////////////////////////
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_LSHIFT))
	{
		m_bHomingOn = !m_bHomingOn;
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
			0.7f * CCamera::GetInstance()->GetScale(), 0.7f * CCamera::GetInstance()->GetScale(), 
			&rRender, 64, 128, m_fHandRotation, -1 );
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nHandID, 
			(int)(((GetPosX() + (GetWidth()/2)) - OffsetX) * CCamera::GetInstance()->GetScale())+GetWidth()/2, 
			(int)(((GetPosY() - (GetHeight()/2)) - OffsetY) * CCamera::GetInstance()->GetScale()+25), 
			-0.7f * CCamera::GetInstance()->GetScale(), 0.7f * CCamera::GetInstance()->GetScale(), 
			&rRender, 64, 128, m_fHandRotation, -1 );
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
		rCharge.left = (LONG)(64 *(this->SONIC_RIFLE+1));
		rCharge.top  = (LONG)0;
		rCharge.right = rCharge.left + 64;
		rCharge.bottom = rCharge.top +(LONG)(64 *(this->m_fChargeRate*this->m_nCharge));
		CSGD_TextureManager::GetInstance()->Draw(CSinglePlayerState::GetInstance()->GetWeaponSelectionID(), 0, (int)(600-(64 *(this->m_fChargeRate*this->m_nCharge))),
			1.0f,1.0f,&rCharge);
	}
	//////////////////////////////////////////////////////////////////////////////
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
	static RECT rPrevRect;
	static CPoint Offset;
	
	RECT rPlayerFrame = GetAnimations()->GetFrame();
	if(rPrevRect.bottom != rPlayerFrame.bottom)
	{
		Offset.m_nY = rPlayerFrame.bottom - rPrevRect.bottom;
		SetPosY(GetPosY()-Offset.m_nY);
		rPrevRect = rPlayerFrame;
		m_bOnPlatform = false;
		m_bOnGround = false;
	}

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
			RECT rPlayerRect = GetAnimations()->GetCollisionFrame( (int)(GetPosX()), (int)(GetPosY()) );			

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
				this->SetPosY(hisY - (myBottom - myY));
				if( BLOCK->GetBlock() == BLOCK_MOVING)
				{
					if(!this->m_bOnMovingPlatform)
					{
						this->m_fMovingPlatformPosX = hisX;
						this->m_pMovingBlock = BLOCK;
						this->m_bOnMovingPlatform = true;
					}
				}

				CMapLoad::GetInstance()->m_bCollisionCheck = true;
				return true;
			}
			else if(this->m_bOnPlatform && CMapLoad::GetInstance()->m_bCollisionCheck == false)
			{
				this->m_bOnGround = 0;
				this->m_bOnPlatform = 0;
				this->m_bOnMovingPlatform = false;
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
				this->SetPosY(hisY - (myBottom - myY));
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
				this->SetPosY(hisBottom - (myBottom - myY));
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
				this->SetPosY(hisY - (myBottom - myY));
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

tVector2D CPlayer::GetSpeed(void)	{ return m_vSpeed; }

float	CPlayer::GetRotationRate(void)	{ return m_fRotationRate; }
