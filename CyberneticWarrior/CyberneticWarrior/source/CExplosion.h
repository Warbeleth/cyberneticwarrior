#ifndef _CEXPLOSION_H_
#define _CEXPLOSION_H_

#include "CBase.h"

class CExplosion : public CBase
{
public:
	CExplosion( void );
	~CExplosion( void );
	void Update( float fElapsedTime );
	void Render( void );
};

#endif