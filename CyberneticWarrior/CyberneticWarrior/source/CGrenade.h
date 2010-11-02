#ifndef CGRENADE_H_
#define CGRENADE_H_

#include "CBase.h"

class CGrenade : public CBase
{
private:
	tVector2D m_vVelocity;
	float m_fBoomTime;
	int	m_nDamage;
	int m_nBounceCount;
public:
	CGrenade(void);
	~CGrenade(void);

	inline void SetYVelocity(float fY) { this->m_vVelocity.fY = fY; }
	
	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif