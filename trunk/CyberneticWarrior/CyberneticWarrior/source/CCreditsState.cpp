#include "PrecompiledHeader.h"
#include "CCreditsState.h"
#include "CStackStateMachine.h"

CCreditsState* CCreditsState::sm_pCreditsInstance = NULL;

CCreditsState::CCreditsState( void )
{
	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;
}

CCreditsState::~CCreditsState( void )
{
}

bool CCreditsState::Input( void )
{	
	m_bInput = false;

	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		m_bInput = true;
		CStackStateMachine::GetInstance()->Pop_back();
	}

	return true;
}

void CCreditsState::Update( float fElapsedTime )
{	
	m_CreditsFont.Update( fElapsedTime );
	AtractMode( fElapsedTime );
}

void CCreditsState::Render( void )
{
	if(m_CreditsFont.DrawScrolling("LOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\nLOLOLOL\n",
		50, 100, 0.5f, -1, 99, 449, 49, 499, m_nScrollingId ))
		m_bInput = true;
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