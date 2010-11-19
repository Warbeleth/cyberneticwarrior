#ifndef DEATHPIRATE_H_
#define DEATHPIRATE_H_
#include "CIdleEnemy.h"

class CDeathPirate : public CIdleEnemy
{
	CBaseEnemy* Spawn_01;
	CBaseEnemy* Spawn_02;
	float m_fTimeWaited;
	

public:
	CDeathPirate();
	CDeathPirate(int nImageID,
				float PosX = 0, 
				float PosY = 0, 
				int Width = 230, 
				int Height = 200, 
				int nState = Idle, 
				int nMaxHP = 3000, 
				int nCurrentHP = 3000,
				int nSightRange = 700, 
				int nAttackRange = 700, 
				int nGlobalType = Boss_Pirate,
				float fRateOfFire = 5.0f, 
				float fSpeed = 0.0f
				);

	~CDeathPirate();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors
	// 
	// Purpose : Returns the specified type.

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers 
	// 
	// Purpose: Modifies the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Update
	//
	//	Purpose : To update CBaseEnemy functionality.
	////////////////////////////////////////////////////////////////////////////////////
	void Update(float fElapsedTime);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Render
	//
	//	Purpose : To Render CBaseEnemy Objects to the Screen.
	////////////////////////////////////////////////////////////////////////////////////
	void Render(void);

	bool CheckCollision(CBase* pBase);
};



#endif
