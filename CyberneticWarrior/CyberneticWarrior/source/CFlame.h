#ifndef CFLAME_H_
#define CFLAME_H_

#include "CBaseProjectile.h"

class CFlame : public CBaseProjectile
{
public:
	CFlame(void);
	~CFlame(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif