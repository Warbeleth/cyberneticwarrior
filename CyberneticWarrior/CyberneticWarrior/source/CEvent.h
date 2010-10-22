#ifndef EVENT_H_
#define EVENT_H_

typedef unsigned char* EVENTID;

class CEvent
{
private:

	EVENTID m_nEventID;

	void*	m_pParameter;
	
public:

	CEvent(EVENTID eventID, void *pParameter);
	~CEvent();

	EVENTID	GetEventID(void);
	void*	GetParameter(void);

};
#endif