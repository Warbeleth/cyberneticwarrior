#ifndef GRAPPLINGHOOK_H_
#define GRAPPLINGHOOK_H_

#include "CBase.h"
#include "IListener.h"

class CEvent;

class CGrapplingHook : public CBase, public IListener
{
private:

	float m_fRotation;
	float m_fRotationRate;
	bool m_bHooked;

public:

	CGrapplingHook(void);
	~CGrapplingHook(void);

	float	GetRotation(void)const;
	void	SetRotation(float fRotation);

	float	GetRotationRate(void)const;
	inline void	SetRotationRate(float fRotationRate) {this->m_fRotationRate = fRotationRate;}

	bool	GetIfHooked(void)const;
	void	SetHooked(bool bHooked);

	void Update(float fElapsedTime);
	void Render(void);

	RECT GetRect(void) const;

	bool CheckCollision(CBase* pBase);
	
	void HandleEvent(CEvent* pEvent);

};
#endif