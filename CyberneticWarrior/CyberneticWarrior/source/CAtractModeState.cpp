#include "PrecompiledHeader.h"

#include "CAtractModeState.h"
#include "CStackStateMachine.h"

CAtractModeState* CAtractModeState::sm_pAtractModeInstance = NULL;

CAtractModeState::CAtractModeState( void )
{
	m_nCurrentImage = 0;
	m_fTotalTime = 0.0f;
	m_nImageIds[0] = -1;
	m_nImageIds[1] = -1;
	m_nImageIds[2] = -1;
	m_nBGMusicId   = -1;
}

CAtractModeState::~CAtractModeState( void )
{
}

bool CAtractModeState::Input( void )
{
	if(CSGD_DirectInput::GetInstance()->CheckKeys())
		CStackStateMachine::GetInstance()->Pop_back();

	return true;
}

void CAtractModeState::Update( float fElapsedTime )
{
	m_fTotalTime += fElapsedTime;

	if( m_fTotalTime >= 10.0f )
	{
		++m_nCurrentImage;
		m_fTotalTime = 0;

		if( m_nCurrentImage >= 3 )
			m_nCurrentImage = 0;
	}
}

void CAtractModeState::Render( void )
{
	if(m_nImageIds[m_nCurrentImage] != -1)
		CSGD_TextureManager::GetInstance()->Draw(m_nImageIds[m_nCurrentImage], 0, 0 );
}

void CAtractModeState::Enter( void )
{	
	m_nCurrentImage = 0;
	m_fTotalTime = 0.0f;

	m_nImageIds[0] = -1;
	m_nImageIds[0] = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/AtractMode1.png");

	m_nImageIds[1] = -1; 
	m_nImageIds[1] = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/AtractMode2.png");

	m_nImageIds[2] = -1; 
	m_nImageIds[2] = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/AtractMode3.png");

	m_nBGMusicId   = -1;
	//m_nBGMusicId   = CSGD_WaveManager::GetInstance()->LoadWave("resource/sounds/Jak2_Haven_City.wav");
	//CSGD_WaveManager::GetInstance()->Play(m_nBGMusicId, DSBPLAY_LOOPING);
}

void CAtractModeState::Exit( void )
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageIds[0]);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageIds[1]);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageIds[2]);

	//CSGD_WaveManager::GetInstance()->Stop(m_nBGMusicId);
	//CSGD_WaveManager::GetInstance()->UnloadWave(m_nBGMusicId);
}

CAtractModeState* CAtractModeState::GetInstance( void )
{
	if(!sm_pAtractModeInstance)
	{
		sm_pAtractModeInstance = new CAtractModeState();
	}
	return sm_pAtractModeInstance;
}

void CAtractModeState::DeleteInstance( void )
{
	if(sm_pAtractModeInstance)
	{
		delete sm_pAtractModeInstance;
		sm_pAtractModeInstance = NULL;
	}
}