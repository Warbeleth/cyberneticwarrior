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
}

void CCreditsState::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw( m_nCreditsBGId, 0, 0 );

	m_CreditsFont.Draw("Credits", 300, 40, 1.5f,-1 );
	if(!m_CreditsFont.DrawScrolling("ASSOCIATE PRODUCER:\n          Sean Hathaway\n\n\n\nPROGRAMMERS:\n          Patrick Alvarez\n          Anthony Muccio\n          Corey Ringer\n\n\n\nASSISTANCE WITH TOOLS \nPROGRAMMING:\n\nMAP EDITOR:\n          Jamison Fullerton\n          Desiree Bard\n\n\n\nGAME LOGIC:\n          Jason Ditz\n          Bryan Evans\n          Scott Leierer\n          Mario Sanchez",
		250, 530, 0.6f, -1, 100, 540, 240, 560, m_nScrollingId ))
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());
}

void CCreditsState::Enter( void )
{
	m_CreditsFont.InitFont("resource/fonts/CyberneticWarriorArcadeFont.png", "resource/fonts/CyberneticWarriorArcadeFont.fnt");
	
	m_nCreditsBGId = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/CreditsBG.png");

	this->m_nBGMusic = CSGD_WaveManager::GetInstance()->LoadWave("resource/sounds/JTM_EpicSong.wav");
	
	CSGD_WaveManager::GetInstance()->Play(this->m_nBGMusic);

	m_nScrollingId = m_CreditsFont.AddScrolling( 0, -100 );
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