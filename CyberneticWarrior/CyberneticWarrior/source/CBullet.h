#ifndef CBULLET_H_
#define CBULLET_H_

#include "CBase.h"

class CBullet : public CBase
{
private:
	float m_fDirection;
	int	m_nDamage;
public:
	CBullet(void);
	~CBullet(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif