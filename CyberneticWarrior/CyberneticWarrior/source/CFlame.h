#ifndef CFLAME_H_
#define CFLAME_H_

#include "CBaseProjectile.h"

class CFlame : public CBaseProjectile
{
private:
	float	m_fLifeTime;
public:
	CFlame(void);
	~CFlame(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif