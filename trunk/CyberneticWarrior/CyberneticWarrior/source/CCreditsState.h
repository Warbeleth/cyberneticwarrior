#ifndef _CCREDITSSTATE_H_
#define _CCREDITSSTATE_H_

#include "IGameState.h"
#include "CFont.h"

class CCreditsState : public IGameState
{
	CFont	m_CreditsFont;
	int		m_nScrollingId;

	int m_nCreditsBGId;

	CCreditsState( void );
	~CCreditsState( void );
	CCreditsState(CCreditsState&);
	CCreditsState& operator=(CCreditsState&);

	static CCreditsState* sm_pCreditsInstance;
public:
	bool Input( void );
	void Update( float fElapsedTime );
	void Render( void );
	void Enter( void );
	void Exit( void );

	static CCreditsState* GetInstance( void );
	static void DeleteInstance( void );
};

#endif