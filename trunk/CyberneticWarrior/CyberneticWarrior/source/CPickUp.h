#ifndef CPICKUP_H_
#define CPICKUP_H_

#include "CBase.h"

enum { GRAPPLING_HOOK };
class CPickUp : public CBase
{
private:


	int m_nPickUpType;

	// temp
	bool bOn;

public:
	
	CPickUp(void);
	~CPickUp(void);

	int GetPickUpType(void);
	void SetPickUpType(int nType);

	void Update(float fElapsedTime);
	void Render(void);

	//RECT GetRect(void) const;

	bool CheckCollision(CBase* pBase);

};
#endif