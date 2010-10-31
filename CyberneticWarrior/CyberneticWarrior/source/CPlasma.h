#ifndef CPLASMA_H_
#define CPLASMA_H_

#include "CBase.h"

class CPlasma : public CBase
{
private:
	int	m_nDamage;
public:
	CPlasma(void);
	~CPlasma(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
};
#endif