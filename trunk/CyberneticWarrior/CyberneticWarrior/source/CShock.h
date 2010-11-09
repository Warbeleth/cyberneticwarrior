#ifndef CSHOCK_H_
#define CSHOCK_H_

#include "CBase.h"

class CShock : public CBase
{
private:
	float m_fDirection;
	int   m_nOwnerType;
	int	m_nDamage;
public:
	CShock(void);
	~CShock(void);

	void Update(float fElapsedTime);
	void Render(void);
	RECT GetRect(void)const;
	bool CheckCollision(CBase* pBase);
	inline	int		GetOwnerType(void) {return this->m_nOwnerType;}
	inline	void	SetOwnerType(int nOwner) {this->m_nOwnerType = nOwner;}
};
#endif