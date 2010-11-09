#ifndef CBULLET_H_
#define CBULLET_H_

#include "CBase.h"

class CBullet : public CBase
{
private:
	CBase*   m_pOwner;
	float m_fDirection;
	int	m_nDamage;
public:
	CBullet(void);
	~CBullet(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
	inline	CBase*		GetOwner(void) {return this->m_pOwner;}
	inline	void	SetOwner(CBase* pOwner) {this->m_pOwner = pOwner;}
};
#endif