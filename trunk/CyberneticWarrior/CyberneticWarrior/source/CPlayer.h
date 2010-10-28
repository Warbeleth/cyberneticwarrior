#ifndef	PLAYER_H_
#define PLAYER_H_

#include "CBase.h"
#include "IListener.h"

class CEvent;
class CGrapplingHook;
class CHud;

enum Input{KEYBOARD = 0, GAMEPAD};
class CPlayer : public CBase, public IListener
{
private:

	tVector2D	m_vRotationCenter;
	tVector2D	m_vSpeed;
	tVector2D	m_vVectorVelocity;
	
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

	float m_fTerminalVelocity;
	float m_fGravity;

	bool m_bOnGround;
	bool m_bOnPlatform;

	bool m_bHomingOn;

	bool m_bForward;

	CGrapplingHook*		m_pHook;
	/////////////////////////////////
	// Temp
	/////////////////////////////////
	tVector2D m_vJoyVecPos;
	bool m_bMouseDown;
	/////////////////////////////////

	friend class CHud;

public:

	CPlayer(void);
	~CPlayer(void);

	tVector2D GetSpeed(void);
	tVector2D* GetJoyPos(void);

	float	GetVectorMagnitude(void);
	float	GetRotationRate(void);

	void Update(float fElapsedTime);
	void Render(void);

	RECT GetRect(void) const;

	bool CheckCollision(CBase* pBase);

	void HandleEvent(CEvent* pEvent);

	void Input(float fElapsedTime);


	bool GetMouseDown(void);
	void SetMouseDown(bool bMouseDown);

	bool GetOnGround(void);


	CGrapplingHook*	GetHookPointer(void);
	void	SetHookPointer(CGrapplingHook*	pHook);
	

	bool	GetHoming( void )	{ return m_bHomingOn; }


	tVector2D	GetRotationPos(void);
	float	GetRotation(void);
	
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
	void IncrementScore( int nScore ) { m_nScore += nScore; }
	void DecrementScore( int nScore ) { m_nScore -= nScore; }
	void IncrementHealth( float nRemainingHealth ) { m_nRemainingHealth += nRemainingHealth; (m_nRemainingHealth > 100.0f) ? m_nRemainingHealth = 100.0f : m_nRemainingHealth = m_nRemainingHealth; }
	void DecrementHealth( float nRemainingHealth ) { m_nRemainingHealth -= nRemainingHealth; (m_nRemainingHealth < 0.0f) ? m_nRemainingHealth = 0.0f : m_nRemainingHealth = m_nRemainingHealth; }
	void IncrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy += nRemainingEnergy; (m_nRemainingEnergy > 100.0f) ? m_nRemainingEnergy = 100.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
	void DecrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy -= nRemainingEnergy; (m_nRemainingEnergy < 0.0f) ? m_nRemainingEnergy = 0.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
};
#endif