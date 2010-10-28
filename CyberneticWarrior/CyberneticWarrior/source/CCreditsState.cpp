#include "CCreditsState.h"
#include "CStackStateMachine.h"

CCreditsState* CCreditsState::sm_pCreditsInstance = NULL;

bool CCreditsState::Input( void )
{
	return true;
}

void CCreditsState::Update( float fElapsedTime )
{	
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CStackStateMachine::GetInstance()->Pop_back();
		
	m_CreditsFont.Update( fElapsedTime );
}

void CCreditsState::Render( void )
{
	m_CreditsFont.DrawScrolling("LOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\n",
		50, 100, 0.5f, -1, 99, 449, 49, 499, m_nScrollingId );
}

void CCreditsState::Enter( void )
{
	m_CreditsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");
	m_nScrollingId = m_CreditsFont.AddScrolling( 0, -150 );
}

void CCreditsState::Exit( void )
{
	m_CreditsFont.ShutdownFont();
}

CCreditsState* CCreditsState::GetInstance( void )
{
	if(!sm_pCreditsInstance)
	{
		sm_pCreditsInstance = new CCreditsState();
	}
	return sm_pCreditsInstance;
}

void CCreditsState::DeleteInstance( void )
{
	if(sm_pCreditsInstance)
	{
		delete sm_pCreditsInstance;
		sm_pCreditsInstance = NULL;
	}
}