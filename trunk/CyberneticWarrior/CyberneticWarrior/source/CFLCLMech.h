#ifndef FLCLMECH_H_
#define FLCLMECH_H_
#include "CPatrolEnemy.h"

class CFLCLMech : public CPatrolEnemy
{
	bool m_bRevive;
	bool m_bReviving;
	float m_fReviveTime;
	float m_fTimeReviving;
	//int m_nCID;


public:
	CFLCLMech();
	CFLCLMech(	int nImageID,
				float PosX = 0, 
				float PosY = 0, 
				int Width = 64, 
				int Height = 128, 
				int nState = Patrol, 
				float fCurrentPatrolDistance = 0, 
				int nMaxHP = 350, 
				int nCurrentHP = 350,
				int nSightRange = 100, 
				int nAttackRange = 75, 
				int nGlobalType = Ground_FLCL,
				float fRateOfFire = 1.0f, 
				float fSpeed = 48.0f,
				float fReviveTime = 10.0f, 
				float fTimeReviving = 0.0f,
				bool bRevive = true
				);

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

	bool CheckCollision(CBase* pBase);
};



#endif
