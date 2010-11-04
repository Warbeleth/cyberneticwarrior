#ifndef CFLAME_H_
#define CFLAME_H_

#include "CBase.h"

class CFlame : public CBase
{
private:
	float m_fDirection;
	float	m_fLifeTime;
	int		m_nDamage;
public:
	CFlame(void);
	~CFlame(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif