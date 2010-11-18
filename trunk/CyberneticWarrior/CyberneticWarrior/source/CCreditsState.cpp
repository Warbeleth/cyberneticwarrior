#include "PrecompiledHeader.h"
#include "CCreditsState.h"
#include "CStackStateMachine.h"
#include "CMainMenuState.h"

CCreditsState* CCreditsState::sm_pCreditsInstance = NULL;

CCreditsState::CCreditsState( void )
{
	m_nCreditsBGId = -1;
	this->m_nBGMusic = -1;
	m_nCreditsBGId = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/CreditsBG.png");
}

CCreditsState::~CCreditsState( void )
{
	if(m_nCreditsBGId != -1)
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCreditsBGId);
}

bool CCreditsState::Input( void )
{
	if(CSGD_DirectInput::GetInstance()->KeyPressed(DIK_ESCAPE))
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());

	return true;
}

void CCreditsState::Update( float fElapsedTime )
{	
	m_CreditsFont.Update( fElapsedTime );
	m_CreditsFont.Input( DIK_SPACE, fElapsedTime, 0, 2 );
}

void CCreditsState::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw( m_nCreditsBGId, 0, 0 );

	m_CreditsFont.Draw("Credits", 300, 40, 1.5f,-1 );
	if(!m_CreditsFont.DrawScrolling("EXECUTIVE PRODUCERS:\n          John O'leske\n          Mike Wigand\n\n\n\nASSOCIATE PRODUCERS:\n          Sean Hathaway\n          Rob Martinez\n          Ron Powell\n\n\n\nPROGRAMMERS:\n          Patrick Alvarez\n          Anthony Muccio\n          Corey Ringer\n\n\n\nARTISTS:\n          Chris Jahosky\n          Anthony Muccio\n          Patrick Alvarez\n\n\n\nSOUND:\n          Robert Osborne\n          James Marantette\n\n\n\nDESIGN CONSULTANT:\n          Brian Arpidone\n\n\n\nASSISTANCE WITH TOOLS:\n\nMAP EDITOR:\n          Jamison Fullerton\n          Desiree Bard\n\n\n\nGAME LOGIC:\n          Jason Ditz\n          Bryan Evans\n          Scott Leierer\n          Mario Sanchez\n\n\n\n----------------------------------------------\n\nSPECIAL THANKS:\n          Tony Mcneil\n          Carlos Lugo\n          Barry Mccall\n          Tyler Berry\n          Patrick Stafford\n          Tyler Lamb\n          Jeremy Goodman\n          John Wileczeck\n\n\n\n\n\n\n          HOLY MELON STUDIOS",
		200, 529, .8f, -1, 100, 530, 0, 600, m_nScrollingId ))
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());
}

void CCreditsState::Enter( void )
{
	m_CreditsFont.InitFont("resource/fonts/CyberneticWarriorArcadeFont.png", "resource/fonts/CyberneticWarriorArcadeFont.fnt");
	
	m_nCreditsBGId = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/CreditsBG.png");

	this->m_nBGMusic = CSGD_WaveManager::GetInstance()->LoadWave("resource/sounds/JTM_EpicSong.wav");
	
	CSGD_WaveManager::GetInstance()->Play(this->m_nBGMusic);

	m_nScrollingId = m_CreditsFont.AddScrolling( 0, -150 );
}

void CCreditsState::Exit( void )
{	
	if(this->m_nBGMusic != -1)
	{
		CSGD_WaveManager::GetInstance()->Stop(this->m_nBGMusic);
		CSGD_WaveManager::GetInstance()->UnloadWave(this->m_nBGMusic);
		this->m_nBGMusic = -1;
	}
	if(m_nCreditsBGId != -1)
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCreditsBGId);
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