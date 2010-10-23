//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �CBaseEnemy.h�
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies to derive off of
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CBASEENEMY_H_
#define CBASEENEMY_H_
#include <windows.h>

//Imports for update checking
#include "CPlayer.h"
#include "CSinglePlayerState.h"
//Remaining includes
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "SGD Wrappers\SGD_Math.h"
#include "IBaseInterface.h"
#include "CBase.h"

//Camera
#include "CCamera.h"

//Enum usage
#include "CMapLoad.h"


class CBaseEnemy : /*public IBaseInterface, */public CBase
{
	int m_nGlobalType;
	int m_nType;
	int m_nImageID;
	int m_nMaxHP;
	int m_nCurrentHP;
	int m_nSightRange;
	int m_nAttackRange;
	int m_nPosX;
	int m_nPosY;
	int m_nWidth;
	int m_nHeight;
	float m_fRateOfFire;
	float m_fSpeed;
	bool m_bSinglePlayer;
	tVector2D m_vTargetPosition;

	unsigned int m_uiRefCount;


public:
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Default Constructor
	//
	//	Purpose : Set generic values if override isn't used.
	////////////////////////////////////////////////////////////////////////////////////
	CBaseEnemy();

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Constructor
	//
	//	Purpose : Setup the enemy with the following values
	////////////////////////////////////////////////////////////////////////////////////
	CBaseEnemy(int nType, int nImageID, int nMaxHP, int nCurrentHP, 
			   int nSightRange, int nAttackRange, float fRateOfFire, 
			   float fSpeed, int PosX, int PosY, int Width, int Height);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Destructor
	//
	//	Purpose : Clean up after the enemy.
	////////////////////////////////////////////////////////////////////////////////////
	~CBaseEnemy();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors
	// 
	// Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetType() { return m_nType; };
	int GetImageID() { return m_nImageID; };
	int GetMaxHP() { return m_nMaxHP; };
	int GetCurrentHP() { return m_nCurrentHP; };
	int GetSightRange() { return m_nSightRange; };
	int GetAttackRange() { return m_nAttackRange; };
	int GetPosX() { return m_nPosX; };
	int GetPosY() { return m_nPosY; };
	int GetWidth() { return m_nWidth; };
	int GetHeight() { return m_nHeight; };
	float GetRateOfFire() { return m_fRateOfFire; };
	float GetSpeed() { return m_fSpeed; };
	tVector2D GetTargetPosition() { return m_vTargetPosition; };
	bool GetGameType() { return m_bSinglePlayer; };
	virtual RECT GetRect();

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers 
	// 
	// Purpose: Modifies the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetType(int Value) { m_nType = Value; };
	void SetImageID(int Value) { m_nImageID = Value; };
	void SetMaxHP(int Value) { m_nMaxHP = Value; };
	void SetCurrentHP(int Value) { m_nCurrentHP = Value; };
	void SetSightRange(int Value) { m_nSightRange = Value; };
	void SetAttackRange(int Value) { m_nAttackRange = Value; };
	void SetPosX(int Value) { m_nPosX = Value; };
	void SetPosY(int Value) { m_nPosY = Value; };
	void SetWidth(int Value) { m_nWidth = Value; };
	void SetHeight(int Value) { m_nHeight = Value; };
	void SetRateOfFire(float Value) { m_fRateOfFire = Value; };
	void SetSpeed(float Value) { m_fSpeed = Value; };

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Update
	//
	//	Purpose : To update CBaseEnemy functionality.
	////////////////////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Render
	//
	//	Purpose : To Render CBaseEnemy Objects to the Screen.
	////////////////////////////////////////////////////////////////////////////////////
	virtual void Render(void);
	
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : AddRef
	//
	//	Purpose : To Add Reference which is used for Object Manager.
	////////////////////////////////////////////////////////////////////////////////////
	void AddRef(void);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Release
	//
	//	Purpose : To release a reference from the object manager.
	////////////////////////////////////////////////////////////////////////////////////
	void Release(void);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : CheckCollision
	//
	//	Purpose : To check for collision agaisnt passed in based object 
	//			  and return whether if collided to the invoking object.
	////////////////////////////////////////////////////////////////////////////////////
	virtual bool CheckCollision(CBase* pBase);
};
#endif