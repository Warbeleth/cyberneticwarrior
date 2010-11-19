#ifndef _CEXPLOSION_H_
#define _CEXPLOSION_H_

#include "CBase.h"

class CExplosion : public CBase
{
	DWORD m_dwColor;
public:
	CExplosion( void );
	~CExplosion( void );
	void Update( float fElapsedTime );
	void Render( void );

	void SetColor( DWORD dwColor ) { m_dwColor = dwColor; }
};

#endif