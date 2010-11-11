#ifndef CICE_H_
#define CICE_H_

#include "CBaseProjectile.h"

class CIce : public CBaseProjectile
{
private:
	float	m_fLifeTime;
public:
	CIce(void);
	~CIce(void);

	void Update(float fElapsedTime);
	bool CheckCollision(CBase* pBase);
};
#endif