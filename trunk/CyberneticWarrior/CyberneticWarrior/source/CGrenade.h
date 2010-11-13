#ifndef CGRENADE_H_
#define CGRENADE_H_

#include "CBaseProjectile.h"

#define BOUNCE_COOLDOWN 0.01f

class CGrenade : public CBaseProjectile
{
private:
	int m_nBounceCount;
	float m_fBounceCooldownTimer;
	bool m_bJustBounced;
	bool m_bStuckToTarget;
	CBase* m_pTarget;
public:
	CGrenade(void);
	~CGrenade(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif