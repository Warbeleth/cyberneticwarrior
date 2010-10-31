#ifndef CICE_H_
#define CICE_H_

#include "CBase.h"

class CIce : public CBase
{
private:
	float	m_fLifeTime;
	int	m_nDamage;
public:
	CIce(void);
	~CIce(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif