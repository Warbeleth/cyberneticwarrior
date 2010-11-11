#ifndef CFIRE_H_
#define CFIRE_H_

#include "CBaseProjectile.h"

class CFire : public CBaseProjectile
{
private:
	int	m_nDamage;
public:
	CFire(void);
	~CFire(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif