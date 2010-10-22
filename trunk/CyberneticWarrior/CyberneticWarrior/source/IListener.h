#ifndef ILISTENER_H_
#define ILISTENER_H_

#include "CEvent.h"

class IListener
{
public:
	IListener(void) {}
	virtual ~IListener(void) {}

	virtual void HandleEvent(CEvent*	pEvent) = 0;
};
#endif