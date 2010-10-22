#ifndef MESSAGESYSTEM_H_
#define MESSAGESYSTEM_H_

#include "CBaseMessage.h"
#include <queue>
using std::queue;

typedef void (*MESSAGEPROC)(CBaseMessage*);

class CMessageSystem
{
private:

	queue<CBaseMessage*>	m_qMsgQueue;
	MESSAGEPROC				m_pfnMsgProc;

	inline CMessageSystem(void)	{ m_pfnMsgProc = NULL; }
	CMessageSystem(const CMessageSystem&);
	CMessageSystem& operator=(const CMessageSystem&);

	inline ~CMessageSystem() {};

	static CMessageSystem*		sm_pMessageSystemInstance;

public:

	static CMessageSystem*	GetInstance(void);
	static void DeleteInstance(void);

	int GetNumMessages(void);

	void InitMessageSystem(MESSAGEPROC pfnMsgProc);

	void SendMsg(CBaseMessage* pMsg);

	void ProcessMessages(void);

	void ClearMessages(void);

	void ShutdownSystem(void);

};
#endif