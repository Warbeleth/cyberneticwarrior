#ifndef CSHOCK_H_
#define CSHOCK_H_

#include "CBaseProjectile.h"

class CShock : public CBaseProjectile
{
public:
	CShock(void);
	~CShock(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif