#ifndef _CROCKET_H_
#define _CROCKET_H_

#include "CBase.h"
#include "IListener.h"

class CEvent;

#define DEATH_TIME 7.0f

class CRocket : public CBase, public IListener
{
private:
	CBase*   m_pOwner;
	float m_fDirection;
	float m_fDeathTimer;
	int	  m_nRocketState;
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
	inline	CBase*		GetOwner(void) {return this->m_pOwner;}
	inline	void	SetOwner(CBase* pOwner) {this->m_pOwner = pOwner;}
};

#endif