#ifndef CFLAME_H_
#define CFLAME_H_

#include "CBase.h"

class CFlame : public CBase
{
private:
	CBase*   m_pOwner;
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
	inline	CBase*		GetOwner(void) {return this->m_pOwner;}
	inline	void	SetOwner(CBase* pOwner) {this->m_pOwner = pOwner;}
};
#endif