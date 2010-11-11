#ifndef CPLASMA_H_
#define CPLASMA_H_

#include "CBaseProjectile.h"

class CPlasma : public CBaseProjectile
{
public:
	CPlasma(void);
	~CPlasma(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif