#ifndef	PLAYER_H_
#define PLAYER_H_

#include "CBase.h"
#include "IListener.h"

class CEvent;
class CGrapplingHook;

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




	CGrapplingHook*		m_pHook;
	/////////////////////////////////
	// Temp
	/////////////////////////////////
	tVector2D m_vJoyVecPos;
	bool m_bMouseDown;
	/////////////////////////////////


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
	void	SetRotationPosX(float fX);
	void	SetRotationPosY(float fY);

	float	GetRotation(void);
	void	SetRotation(float fRotation);
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : SetSpeedY
	//
	//	Purpose : Mutator that sets m_vSpeed.fY to the recieved parameter
	////////////////////////////////////////////////////////////////////////////////////
	void SetSpeedY(float nSpeedY);
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : SetSpeedX
	//
	//	Purpose : Mutator that sets m_vSpeed.fX to the recieved parameter
	////////////////////////////////////////////////////////////////////////////////////
	void SetSpeedX(float nSpeedX);
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : SetOnGround
	//
	//	Purpose : Mutator that sets m_bOnGround to the recieved parameter
	////////////////////////////////////////////////////////////////////////////////////
	void SetOnGround(bool bOnGround);
	/////////////////////////////////////

};
#endif