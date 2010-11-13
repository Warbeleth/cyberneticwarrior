#ifndef _CROCKET_H_
#define _CROCKET_H_

#include "CBaseProjectile.h"

class CRocket : public CBaseProjectile
{
private:
	int	  m_nRocketState;

protected:
	enum RocketStates { ROCKET_DIRECTIONAL, ROCKET_HOMING, ROCKET_MAX };

public:
	CRocket( void );
	~CRocket( void );

	void Update( float fElapsedTime);
	bool CheckCollision( CBase* pBase );
};

#endif