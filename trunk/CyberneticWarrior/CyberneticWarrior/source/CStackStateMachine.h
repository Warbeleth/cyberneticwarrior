//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : CStackStateMachine.h
//
//	Author : Patrick Alvarez 
//
//	Purpose : Stack state machines holds a singly linked list 
//			  of states and switches them based on user input.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	STACKSTATEMACHINE_H_
#define STACKSTATEMACHINE_H_

#include "IGameState.h"

class IGameState;

class CStackStateMachine
{
private:

	struct tNode
	{
		IGameState*		pData;
		tNode*			pNext;
	}*m_pHead;

	unsigned int m_nSize;

	static CStackStateMachine*		sm_pStateStackMachineInstance;


	CStackStateMachine(void);
	~CStackStateMachine(void);
	CStackStateMachine(const CStackStateMachine& assignment);
	CStackStateMachine& operator=(const CStackStateMachine& assignment);

	void recursive(tNode * current);

public:

	unsigned int					Size(void);
	
	bool							Input(void);
	

	void							Push_Back(IGameState*	pGameState);
	void							Pop_back(void);

	void							ChangeState(IGameState*	pNewState);

	void							RenderState(void);
	void							UpdateState(float fElapsedtime);

	void							Clear(void);


	static CStackStateMachine*		GetInstance(void);
	static void						DeleteInstance(void);
	//void							Erase(int nIndex);
	//IGameState*					operator[](int nIndex);
};
#endif