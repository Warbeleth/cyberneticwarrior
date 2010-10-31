#ifndef CFIRE_H_
#define CFIRE_H_

#include "CBase.h"

class CFire : public CBase
{
private:
	int	m_nDamage;
public:
	CFire(void);
	~CFire(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif