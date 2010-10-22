#include "CEvent.h"

CEvent::CEvent(EVENTID eventID, void *pParameter)
{
	this->m_nEventID = eventID;
	this->m_pParameter = pParameter;
}

CEvent::~CEvent(void) {}

EVENTID	CEvent::GetEventID(void)		{ return this->m_nEventID; }
void*	CEvent::GetParameter(void)		{ return this->m_pParameter; }