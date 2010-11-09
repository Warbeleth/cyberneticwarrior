#ifndef CGRENADE_H_
#define CGRENADE_H_

#include "CBase.h"

class CGrenade : public CBase
{
private:
	tVector2D m_vVelocity;
	CBase*   m_pOwner;
	float m_fDirection;
	float m_fBoomTime;
	int	m_nDamage;
	int m_nBounceCount;
public:
	CGrenade(void);
	~CGrenade(void);

	inline void SetYVelocity(float fY) { this->m_vVelocity.fY = fY; }
	
	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
	inline	CBase*		GetOwner(void) {return this->m_pOwner;}
	inline	void	SetOwner(CBase* pOwner) {this->m_pOwner = pOwner;}
};
#endif