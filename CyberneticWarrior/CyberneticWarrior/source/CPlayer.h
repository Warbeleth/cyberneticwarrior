//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : CPlayer.h
//
//	Author : Patrick Alvarez, Anthony Muccio, Corey Ringer
//
//	Purpose : Games Main Character and his Functionality
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	PLAYER_H_
#define PLAYER_H_

#include "CBase.h"
#include "IListener.h"

class CEvent;
class CGrapplingHook;
class CHud;
class CBlock;

enum Input{KEYBOARD = 0, GAMEPAD};
class CPlayer : public CBase, public IListener
{
private:

	enum WeaponSelection{HAND_GUN = 6, ROCKET_LAUNCHER = 8, FLAME_THROWER = 10, 
		PLASMA_RIFLE = 12, SONIC_RIFLE = 14, STICKY_GRENADE = 16};

	tVector2D	m_vRotationCenter;
	tVector2D	m_vSpeed;
	tVector2D	m_vVectorVelocity;

	CBlock*			m_pMovingBlock;
	
	float m_fAcceleration;
	float m_fRotationRate;

	float m_fRotation;
	float m_fJoyRot;

	// HUD VARIABLES
	// Health
	float m_nRemainingHealth;
	float m_nTotalHealth;

	// Energy
	float m_nRemainingEnergy;
	float m_nTotalEnergy;

	// Score
	int m_nScore;

	CHud* m_pHud;

	// Currently Selected
	int m_nSelectedWeapon;
	int m_nSelectedHeadSlot;
	int m_nSelectedBootSlot;

	///////////////////
	// Hand
	int		m_nHandID;
		
	float m_fWaitTime;
	float m_fHandRotation;
	float m_fShotTime;
	float m_fThrowTime;
	float m_fMovingPlatformPosX;

	float m_fTerminalVelocity;
	float m_fGravity;

	//////////////////////////
	// used for Sonic Rifle
	//////////////////////////
	int m_nCharge;
	float m_fChargeRate;
	//////////////////////////


	bool m_bOnGround;
	bool m_bOnPlatform;
	bool m_bOnMovingPlatform;

	bool m_bHomingOn;

	bool m_bForward;

	bool m_bFixSwing;

	bool m_bShutDown;

	CGrapplingHook*		m_pHook;
	
	tVector2D m_vJoyVecPos;
	bool m_bMouseDown;

	friend class CHud;

public:

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : CPlayer (Constructor)
	//
	//	Purpose : To initialize all of CPlayer's many data class members.
	////////////////////////////////////////////////////////////////////////////////////
	CPlayer(void);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : ~CPlayer (Deconstructor)
	//
	//	Purpose : To destroy all of CPlayer's many data class members.
	////////////////////////////////////////////////////////////////////////////////////
	~CPlayer(void);

	
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Check Collision
	//
	//	Purpose : To Check players character collisions among any other game object
	////////////////////////////////////////////////////////////////////////////////////
	bool CheckCollision(CBase* pBase);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Update
	//
	//	Purpose : To update all of the players character functionality
	////////////////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Render
	//
	//	Purpose : To Render players character to game camera and screen
	////////////////////////////////////////////////////////////////////////////////////
	void Render(void);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Handle Event
	//
	//	Purpose : To Handle any players character events
	////////////////////////////////////////////////////////////////////////////////////
	void HandleEvent(CEvent* pEvent);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Input
	//
	//	Purpose : To Check the player to character input
	////////////////////////////////////////////////////////////////////////////////////
	void Input(float fElapsedTime);


	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Accessors
	//
	//	Purpose : Access a value of a variable
	///////////////////////////////////////////////////////////////////////////////////
	RECT GetRect(void) const;

	tVector2D GetSpeed(void);
	tVector2D GetRotationPos(void);
	tVector2D* GetJoyPos(void);

	CGrapplingHook*	GetHookPointer(void);

	float	GetVectorMagnitude(void);
	float	GetRotation(void);
	float	GetRotationRate(void);

	bool GetMouseDown(void);
	bool GetOnGround(void);
	inline bool GetHoming( void )	{ return m_bHomingOn; }

	
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Mutators
	//
	//	Purpose : Change a value of a variable
	///////////////////////////////////////////////////////////////////////////////////
	void SetOnGround(bool bOnGround);
	void SetSpeedX(float nSpeedX);
	void SetSpeedY(float nSpeedY);
	void SetRotation(float fRotation);
	void SetRotationPosX(float fX);
	void SetRotationPosY(float fY);
	void SetPlayerNumber( int nPlayer );

	inline void SetShutDown(bool bShutDown) { this->m_bShutDown = bShutDown;}

	void	SetHookPointer(CGrapplingHook*	pHook);
	
	void SetMouseDown(bool bMouseDown);

	//float	GetArmRotation(void)	{return this->m_fHandRotation;}

	inline void IncrementScore( int nScore ) { m_nScore += nScore; }
	inline void DecrementScore( int nScore ) { m_nScore -= nScore; }
	inline void IncrementHealth( float nRemainingHealth ) { m_nRemainingHealth += nRemainingHealth; (m_nRemainingHealth > 100.0f) ? m_nRemainingHealth = 100.0f : m_nRemainingHealth = m_nRemainingHealth; }
	inline void DecrementHealth( float nRemainingHealth ) { m_nRemainingHealth -= nRemainingHealth; (m_nRemainingHealth < 0.0f) ? m_nRemainingHealth = 0.0f : m_nRemainingHealth = m_nRemainingHealth; }
	inline void IncrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy += nRemainingEnergy; (m_nRemainingEnergy > 100.0f) ? m_nRemainingEnergy = 100.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
	inline void DecrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy -= nRemainingEnergy; (m_nRemainingEnergy < 0.0f) ? m_nRemainingEnergy = 0.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
};
#endif