#ifndef CGRENADE_H_
#define CGRENADE_H_

#include "CBase.h"

class CGrenade : public CBase
{
private:
	int	m_nDamage;
public:
	CGrenade(void);
	~CGrenade(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif