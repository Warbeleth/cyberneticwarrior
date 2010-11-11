#ifndef _CROCKET_H_
#define _CROCKET_H_

#include "CBaseProjectile.h"

#define DEATH_TIME 7.0f

class CRocket : public CBaseProjectile
{
private:
	float m_fDeathTimer;
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