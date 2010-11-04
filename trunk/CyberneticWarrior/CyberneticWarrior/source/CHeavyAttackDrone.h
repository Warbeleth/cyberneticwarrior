#ifndef HEAVYATTACKDRONE_H_
#define HEAVYATTACKDRONE_H_
#include "CPatrolEnemy.h"

class CHeavyAttackDrone : public CPatrolEnemy
{

public:
	CHeavyAttackDrone();
	CHeavyAttackDrone(	int nImageID,
				float PosX = 0, 
				float PosY = 0, 
				int Width = 192, 
				int Height = 192, 
				int nState = Patrol, 
				float fCurrentPatrolDistance = 0, 
				int nMaxHP = 100, 
				int nCurrentHP = 100,
				int nSightRange = 600, 
				int nAttackRange = 500, 
				int nGlobalType = Drone_Heavy,
				float fRateOfFire = 1.0f, 
				float fSpeed = 48.0f
				);

	~CHeavyAttackDrone();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors
	// 
	// Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////////////////////////////////

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
