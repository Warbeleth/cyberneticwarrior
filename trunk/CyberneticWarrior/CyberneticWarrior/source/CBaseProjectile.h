#ifndef _CBASEPROJECTILE_H_
#define _CBASEPROJECTILE_H_

#include "CBase.h"
//OBJ_GRENADE, OBJ_SHOCK, OBJ_ROCKET, OBJ_FLAME, OBJ_FIRE, OBJ_ICE, OBJ_BULLET, OBJ_PLASMA

class CBaseProjectile : public CBase
{
	CBase*  m_pOwner;
	float m_fAge;
	int	m_nDamage;
	float m_fLifeTime;
protected:
	RECT	m_rRender;
	bool	m_bDead;

public:
	CBaseProjectile( void );
	virtual~CBaseProjectile( void );
	virtual void Update( float fElapsedTime );
	virtual void Render( void );
	virtual RECT GetRect( void );
	virtual bool CheckCollision(CBase *pBase);
	virtual inline CBase* GetOwner(void) {return this->m_pOwner;}
	virtual	void SetOwner(CBase* pOwner) {this->m_pOwner = pOwner;}
	inline void SetLife(float fLife) {this->m_fLifeTime = fLife;}
	inline int  GetLife(void) {return this->m_fLifeTime;}
	inline void SetDamage( int nDamage ) { m_nDamage = nDamage; }
	inline int GetDamage( void ) { return m_nDamage; }
	inline void SetTime( float fDeathTime ) { m_fDeathTime = fDeathTime; }
};

#endif