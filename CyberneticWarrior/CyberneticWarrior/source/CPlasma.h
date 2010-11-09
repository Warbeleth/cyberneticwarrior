#ifndef CPLASMA_H_
#define CPLASMA_H_

#include "CBase.h"

class CPlasma : public CBase
{
private:
	float m_fDirection;
	int   m_nOwnerType;
	int	m_nDamage;
public:
	CPlasma(void);
	~CPlasma(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
	inline	int		GetOwnerType(void) {return this->m_nOwnerType;}
	inline	void	SetOwnerType(int nOwner) {this->m_nOwnerType = nOwner;}
};
#endif