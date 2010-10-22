#ifndef	EVENTSYSTEM_H_
#define EVENTSYSTEM_H_

#include "CEvent.h"
#include "IListener.h"

#include <map>
#include <list>
using std::list;
using std::multimap;
using std::pair;

class CEventSystem
{
private:
	
	multimap<EVENTID, IListener*> m_Clients;

	list<CEvent> m_lEvents;

	void	SendOutEvent(CEvent*	pEvent);
	bool	AlreadyRegistered(EVENTID eventID, IListener* pClient);

	inline CEventSystem() {};
	CEventSystem(const CEventSystem&);
	CEventSystem& operator=(const CEventSystem&);

	inline ~CEventSystem() {};
	static CEventSystem*	sm_pEventSystemInstance;

public:

	static CEventSystem*	GetInstance(void);
	static void	DeleteInstance(void);

	void RegisterClient(EVENTID eventID, IListener*	pClient);
	void UnregisterClient(EVENTID eventID, IListener* pClient);

	void UnregisterAllClients(IListener* pClient);
	void SendEvent(EVENTID eventID, void* pData);

	void ProcessEvents(void);

	void ClearEvents(void);

	void ShutdownEventSystem(void);



};
#endif