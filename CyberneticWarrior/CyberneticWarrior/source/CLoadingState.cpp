#include "PrecompiledHeader.h"
#include "CLoadingState.h"
#include "CStackStateMachine.h"
#include "CSinglePlayerState.h"
#include "CGame.h"

CLoadingState*	CLoadingState::sm_pLoadingInstance = NULL;

CLoadingState::CLoadingState(void)
{
	m_pD3D	= NULL;
	m_pDI	= NULL;
	m_pTM	= NULL;
	m_pWM	= NULL;
	m_pDS	= NULL;

	this->m_fRotationRate	= 0.07f;
	this->m_fRotateTime		= 0.0f;
	this->m_fRotation		= 0.0f;
	this->m_nBackgroundID	= -1;
	this->m_nLoadingID		= -1;
}

CLoadingState::~CLoadingState(void)
{
	m_pD3D	= NULL;
	m_pDI	= NULL;
	m_pTM	= NULL;
	m_pWM	= NULL;
	m_pDS	= NULL;

	this->m_fRotateTime		= 0.0f;
	this->m_nBackgroundID	= -1;
	this->m_nLoadingID		= -1;
}

bool	CLoadingState::Input(void)
{
	if(this->m_pDI->CheckKeys())
	{
		CStackStateMachine::GetInstance()->Pop_back();
	}
	return 1;
}
void	CLoadingState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_nBackgroundID = this->m_pTM->LoadTexture("resource/graphics/CyberneticHarmony-800.png");
	this->m_nLoadingID = this->m_pTM->LoadTexture("resource/graphics/CyberneticHarmony.png");
	this->m_nLoadFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");
	
}
void	CLoadingState::Update(float fElapsedTime)
{
	this->m_fRotateTime += fElapsedTime;

	if(this->m_fRotateTime > 0.07f)
	{
		this->m_fRotation += this->m_fRotationRate;
		this->m_fRotateTime = 0.0f;
	}
}
void	CLoadingState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	this->m_pTM->Draw(this->m_nLoadingID, 0, 0, 1.0f, 1.0f, 0, 
		(float)CGame::GetInstance()->GetScreenWidth()/2, (float)CGame::GetInstance()->GetScreenHeight()/2,
		this->m_fRotation);
	this->m_nLoadFont.Draw("Press Any Key to Continue", 150, 250, 1.2f, D3DXCOLOR(1.0f,1.0f,0.4f,0.7f));
}
void	CLoadingState::Exit(void)
{
	if(this->m_nLoadingID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nLoadingID);
		this->m_nLoadingID = -1;
	}
	if(this->m_nBackgroundID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundID);
		this->m_nBackgroundID = -1;
	}
	if(this->m_pDS)
	{
		this->m_pDS = NULL;
	}
	if(this->m_pWM)
	{
		this->m_pWM = NULL;
	}
	if(this->m_pTM)
	{
		this->m_pTM = NULL;
	}
	if(this->m_pDI)
	{
		this->m_pDI = NULL;
	}
	if(this->m_pD3D)
	{
		this->m_pD3D = NULL;
	}
}

CLoadingState*	CLoadingState::GetInstance(void)
{
	if(!sm_pLoadingInstance)
	{
		sm_pLoadingInstance = new CLoadingState();
	}
	return sm_pLoadingInstance;
}
void	CLoadingState::DeleteInstance(void)
{
	if(sm_pLoadingInstance)
	{
		delete sm_pLoadingInstance;
		sm_pLoadingInstance = NULL;
	}
}

bool	CLoadingState::GetReady(void)	{return this->m_bReady;}
void	CLoadingState::SetReady(bool bReady)	{this->m_bReady = bReady;}