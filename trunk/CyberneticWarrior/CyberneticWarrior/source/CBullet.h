#ifndef CBULLET_H_
#define CBULLET_H_

#include "CBaseProjectile.h"

class CBullet : public CBaseProjectile
{
public:
	CBullet(void);
	~CBullet(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif