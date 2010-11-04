#ifndef APPLEMECH_H_
#define APPLEMECH_H_
#include "CIdleEnemy.h"

class CAppleMech : public CIdleEnemy
{


public:
	CAppleMech();
	CAppleMech(int nImageID,
				float PosX = 0, 
				float PosY = 0, 
				int Width = 384, 
				int Height = 192, 
				int nState = Idle, 
				int nMaxHP = 80, 
				int nCurrentHP = 80,
				int nSightRange = 700, 
				int nAttackRange = 700, 
				int nGlobalType = Ground_Siege,
				float fRateOfFire = 5.0f, 
				float fSpeed = 0.0f
				);

	~CAppleMech();

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

};



#endif
