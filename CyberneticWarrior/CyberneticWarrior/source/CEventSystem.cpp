#include "CEventSystem.h"

CEventSystem*	CEventSystem::sm_pEventSystemInstance = NULL;


CEventSystem*	CEventSystem::GetInstance(void)
{
	if(!sm_pEventSystemInstance)
	{
		sm_pEventSystemInstance = new CEventSystem();
	}
	return sm_pEventSystemInstance;
}

void	CEventSystem::DeleteInstance(void)
{
	if(sm_pEventSystemInstance)
	{
		delete sm_pEventSystemInstance;
		sm_pEventSystemInstance = NULL;
	}
}

void CEventSystem::RegisterClient(EVENTID eventID, IListener *pClient)
{
	if(!pClient || this->AlreadyRegistered(eventID, pClient))
	{
		return;
	}

	this->m_Clients.insert(std::make_pair(eventID, pClient));
}

void CEventSystem::UnregisterClient(EVENTID eventID, IListener *pClient)
{
	pair<multimap<EVENTID, IListener*>::iterator,
		multimap<EVENTID, IListener*>::iterator>range;

	range = this->m_Clients.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator iter = range.first; 
		iter != range.second; iter++)
	{
		if((*iter).second == pClient)
		{
			iter = this->m_Clients.erase(iter);
			break;
		}
	}
}

void CEventSystem::UnregisterAllClients(IListener* pClient)
{
	multimap<int, IListener*>::iterator iter = this->m_Clients.begin();

	while(iter != this->m_Clients.end())
	{
		if((*iter).second != pClient)
		{
			iter != this->m_Clients.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void CEventSystem::SendOutEvent(CEvent *pEvent)
{
	pair<multimap<EVENTID, IListener*>::iterator,
		multimap<EVENTID, IListener*>::iterator> rangeIter;

	rangeIter = this->m_Clients.equal_range(pEvent->GetEventID());

	for(multimap<EVENTID, IListener*>::iterator iter = rangeIter.first;
		iter != rangeIter.second; iter++)
	{
		(*iter).second->HandleEvent(pEvent);
	}
}

bool CEventSystem::AlreadyRegistered(EVENTID eventID, IListener *pClient)
{
	bool bAlreadyRegistered = false;

	pair<multimap<EVENTID, IListener*>::iterator,
		multimap<EVENTID, IListener*>::iterator> rangeIter;

	rangeIter = this->m_Clients.equal_range(eventID);

	for(multimap<EVENTID, IListener*>::iterator iter = rangeIter.first;
		iter != rangeIter.second; iter++)
	{
		if((*iter).second == pClient)
		{
			bAlreadyRegistered = true;
			break;
		}
	}
	return bAlreadyRegistered;
}

void CEventSystem::SendEvent(EVENTID eventID, void *pData)
{
	CEvent newEvent(eventID, pData);

	this->m_lEvents.push_back(newEvent);
}

void CEventSystem::ProcessEvents(void)
{
	while(this->m_lEvents.size())
	{
		this->SendOutEvent(&this->m_lEvents.front());
		this->m_lEvents.pop_front();
	}
}

void CEventSystem::ClearEvents(void)
{
	this->m_lEvents.clear();
}

void CEventSystem::ShutdownEventSystem(void)
{
	this->m_Clients.clear();
}