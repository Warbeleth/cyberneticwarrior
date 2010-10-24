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
#include "CGame.h"
#include "CStackStateMachine.h"
#include "CSinglePlayerState.h"
#include "IGameState.h"

CStackStateMachine*	CStackStateMachine::sm_pStateStackMachineInstance = NULL;

CStackStateMachine::CStackStateMachine(void)
{
	this->m_pHead = NULL;
	this->m_nSize = 0;
}

CStackStateMachine::CStackStateMachine(const CStackStateMachine& assignment)
{
	this->m_pHead = NULL;
	operator=(assignment);
}

CStackStateMachine& CStackStateMachine::operator=(const CStackStateMachine& assignment)
{
	if(this != &assignment)
	{
		this->Clear();

		this->m_nSize = 0;

		this->recursive(assignment.m_pHead);
	}
	return *this;
}

CStackStateMachine::~CStackStateMachine(void)
{
	this->Clear();
}

CStackStateMachine*		CStackStateMachine::GetInstance(void)
{
	if(sm_pStateStackMachineInstance == NULL)
	{
		sm_pStateStackMachineInstance = new CStackStateMachine();
	}
	return sm_pStateStackMachineInstance;
}
void	CStackStateMachine::DeleteInstance(void)
{
	if(sm_pStateStackMachineInstance != NULL)
	{
		delete sm_pStateStackMachineInstance;
		sm_pStateStackMachineInstance = NULL;
	}
}

void	CStackStateMachine::Push_Back(IGameState*	pGameState)
{
	if(!pGameState){return;}
	else
	{
		tNode * pNewNode = new tNode;

		pNewNode->pData = pGameState;
		pNewNode->pNext = m_pHead;
		this->m_pHead = pNewNode;
			
		++this->m_nSize;

		this->m_pHead->pData->Enter();
		//delete pNewNode;
		/*pNewNode->pData = NULL;
		pNewNode->pNext = NULL;*/
		pNewNode = NULL;
	}
}

void	CStackStateMachine::Pop_back(void)
{
	if(!this->m_pHead) {return;}
	else if(!this->m_pHead->pNext)
	{
		this->m_pHead->pData->Exit();
		delete this->m_pHead;
		this->m_pHead = NULL;
		--this->m_nSize;
	}
	else
	{
		this->m_pHead->pData->Exit();
		tNode * pPopNode = this->m_pHead;
		this->m_pHead = this->m_pHead->pNext;
		pPopNode->pData = NULL;
		pPopNode->pNext = NULL;
		delete pPopNode;
		pPopNode = NULL;
		--this->m_nSize;
	}
	/*else if(m_pHead->pData == m_pTail->pData)
	{
		m_pHead->pData->Exit();
		m_pHead->pData = NULL;
		//m_tHead.pNext = NULL;
		m_pTail->pData = NULL;
		//m_tTail.pNext = NULL;
		--m_nSize;
	}*/
}



void	CStackStateMachine::ChangeState(IGameState*	pNewState)
{
	this->Clear();
	this->Push_Back(pNewState);
}


//IGameState*		CStackStateMachine::operator[](int nIndex)
//{
//	return m_tHead.pData;
//}

void	CStackStateMachine::RenderState(void)
{
	if(this->m_pHead->pData != NULL)
	{
		if(this->m_pHead->pNext !=  NULL)
		{
			if(this->m_pHead->pNext->pData !=  NULL)
			{
				this->m_pHead->pNext->pData->Render();
			}
		}
		this->m_pHead->pData->Render();
	}
}
void	CStackStateMachine::UpdateState(float fElapsedtime)
{
	if(this->m_pHead->pData != NULL)
	{
		/*if(((CSinglePlayerState*)this->m_pHead->pNext->pData)->GetInstance() == this->m_pHead->pNext->pData)
		{
			this->m_pHead->pNext->pData->Update(fElapsedtime);
		}*/
		this->m_pHead->pData->Update(fElapsedtime);
	}
}
bool	CStackStateMachine::Input(void)
{
	/*if(m_tHead.pData == CHudState::GetInstance() && m_tHead.pData != NULL)
	{
		return m_tHead.pNext->pData->Input();
	}
	else */
	if(this->m_pHead->pData != NULL)
	{
		return this->m_pHead->pData->Input();
	}

	return 1;
}

unsigned int	CStackStateMachine::Size(void)
{
	return this->m_nSize;
}
void	CStackStateMachine::Clear(void)
{
	if(this->Size() > 0)
	{
		while(this->Size() > 0)
		{
			this->Pop_back();
		}
	}
}

void CStackStateMachine::recursive(tNode * current)
{
	if(current != NULL)
	{
		this->recursive(current->pNext);
		this->Push_Back(current->pData);
	}
}

//void	CStackStateMachine::Erase(int nIndex)
//{
//}