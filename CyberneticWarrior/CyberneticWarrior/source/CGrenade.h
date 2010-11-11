#ifndef CGRENADE_H_
#define CGRENADE_H_

#include "CBaseProjectile.h"

class CGrenade : public CBaseProjectile
{
private:
	tVector2D m_vVelocity;
	float m_fBoomTime;
	int m_nBounceCount;
public:
	CGrenade(void);
	~CGrenade(void);

	inline void SetYVelocity(float fY) { this->m_vVelocity.fY = fY; }
	
	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif