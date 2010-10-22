#ifndef FLCLMECH_H_
#define FLCLMECH_H_
#include "CPatrolEnemy.h"

class CFLCLMech : public CPatrolEnemy
{
	bool m_bRevive;
	bool m_bReviving;
	float m_fReviveTime;
	float m_fTimeReviving;


public:
	CFLCLMech();
	CFLCLMech(bool bRevive, float fReviveTime, float fTimeReviving,int nState, int nCurrentPatrolDistance, 
				 int nMaxPatrolDistance, int nType, int nImageID, int nMaxHP, int nCurrentHP,int nSightRange, 
				 int nAttackRange, float fRateOfFire, float fSpeed, int PosX, int PosY, int Width, int Height);
	~CFLCLMech();

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Accessors
	// 
	// Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool GetRevive() { return m_bRevive; };
	float GetReviveTime() { return m_fReviveTime; };
	float GetTimeDead() { return m_fTimeReviving; };

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers 
	// 
	// Purpose: Modifies the specified type. 
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetRevive(bool Value) { m_bRevive = Value; };
	void SetReviveTime(float Value) { m_fReviveTime = Value; };
	void SetTimeDead(float Value) { m_fTimeReviving = Value; };

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
