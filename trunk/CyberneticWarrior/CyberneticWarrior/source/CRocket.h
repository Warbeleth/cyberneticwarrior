#ifndef _CROCKET_H_
#define _CROCKET_H_

#include "CBase.h"
#include "CEvent.h"
#include "IListener.h"

#define DEATH_TIME 7.0f

class CRocket : public CBase, public IListener
{
private:
	float m_fRotation;
	int	  m_nRocketState;
	float m_fDeathTimer;

protected:
	enum RocketStates { ROCKET_DIRECTIONAL, ROCKET_HOMING, ROCKET_MAX };

public:
	CRocket( void );
	~CRocket( void );

	void Update( float fElapsedTime);
	void Render( void );
	RECT GetRect( void ) const;
	bool CheckCollision( CBase* pBase );
	void HandleEvent( CEvent* pEvent );
};

#endif