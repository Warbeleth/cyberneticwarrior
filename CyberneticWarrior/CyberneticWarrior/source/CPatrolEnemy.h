//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: �CPatrolEnemy.h�
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the base object for all enemies that have idle and active -
//			states to derive off of
//////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PATROLENEMY_H_
#define PATROLENEMY_H_
#include "CBaseEnemy.h"

enum PatrolStates { Patrol = 0, pActive, pDead };


class CPatrolEnemy : public CBaseEnemy
{
	int m_nState;
	float m_fCurrentPatrolDistance;
	float m_fMaxPatrolDistance;

public:
	CPatrolEnemy(void);
	CPatrolEnemy(int nState, float fCurrentPatrolDistance, float fMaxPatrolDistance, int nType, int nImageID, 
				 int nMaxHP, int nCurrentHP,int nSightRange, int nAttackRange, float fRateOfFire, 
			     float fSpeed, float PosX, float PosY, int Width, int Height);

	~CPatrolEnemy(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors
	// 
	// Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	float GetCurrentDist() { return m_fCurrentPatrolDistance; };
	float GetMaxDist() { return m_fMaxPatrolDistance; };

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers 
	// 
	// Purpose: Modifies the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetCurrentDist(float Value) { m_fCurrentPatrolDistance = Value; };
	void SetMaxDist(float Value) { m_fMaxPatrolDistance = Value; };

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
	//	Function : ChangeAIState
	//
	//	Purpose : Changes the AI's current state based on update checks
	////////////////////////////////////////////////////////////////////////////////////
	void ChangeAIState(int nNewState);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : ReturnAIState
	//
	//	Purpose : Gets the current AI state
	////////////////////////////////////////////////////////////////////////////////////
	int ReturnAIState();
};

#endif