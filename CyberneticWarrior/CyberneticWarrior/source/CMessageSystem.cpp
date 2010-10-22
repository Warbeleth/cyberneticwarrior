#include "CMessageSystem.h"

CMessageSystem*	CMessageSystem::sm_pMessageSystemInstance = NULL;

CMessageSystem*	CMessageSystem::GetInstance(void)
{
	if(NULL == sm_pMessageSystemInstance)
	{
		sm_pMessageSystemInstance = new CMessageSystem();
	}
	return sm_pMessageSystemInstance;
}

void CMessageSystem::DeleteInstance(void)
{
	if(sm_pMessageSystemInstance)
	{
		delete sm_pMessageSystemInstance;
		sm_pMessageSystemInstance = NULL;
	}
}

int CMessageSystem::GetNumMessages(void)	{ return (int)m_qMsgQueue.size(); }

void CMessageSystem::InitMessageSystem(MESSAGEPROC pfnMsgProc)
{
	if(!pfnMsgProc) {return;}

	this->m_pfnMsgProc = pfnMsgProc; 
}

void CMessageSystem::SendMsg(CBaseMessage *pMsg)
{
	if(!pMsg)	{return;}

	this->m_qMsgQueue.push(pMsg);
}

void CMessageSystem::ClearMessages(void)
{
	while(!this->m_qMsgQueue.empty())
	{
		delete this->m_qMsgQueue.front();
		this->m_qMsgQueue.pop();
	}
}

void CMessageSystem::ShutdownSystem(void)
{
	this->ClearMessages();
	this->m_pfnMsgProc = NULL;
}

void CMessageSystem::ProcessMessages(void)
{
	if(!this->m_pfnMsgProc)	{return;}

	while(!this->m_qMsgQueue.empty())
	{
		this->m_pfnMsgProc(this->m_qMsgQueue.front());
		delete this->m_qMsgQueue.front();
		this->m_qMsgQueue.pop();
	}
}