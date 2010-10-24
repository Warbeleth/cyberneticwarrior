#include "PrecompiledHeader.h"

#include "CHowToPlayState.h"
#include "CStackStateMachine.h"

CHowToPlayState* CHowToPlayState::sm_pHowToPlayInstance = NULL;

CHowToPlayState::CHowToPlayState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
}

CHowToPlayState::~CHowToPlayState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
}

bool	CHowToPlayState::Input(void)
{
	if(this->m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		CStackStateMachine::GetInstance()->Pop_back();
	}
	return 1;
}
void	CHowToPlayState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM	=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/3d_city.png");
}
void	CHowToPlayState::Update(float fElapsedTime)
{
	
}
void	CHowToPlayState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);

	this->m_OptionsFont.Draw("-HOW TO PLAY-"	, 50, 25, 1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("-KEYBOARD-"		, 50, 100, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("WASD - Movement"	, 50, 150, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("Space - Jump"		, 50, 200, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("Left Mouse Button - Primary Shot", 50, 250, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("Right Mouse Button - Secondary Shot", 50, 300, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("-Game Pad-"		, 50, 350, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("D-Pad/Left JoyStick - Movement", 50, 400, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("(2) - Jump"		, 50, 450, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("[ 5 ] - Primary Shot", 50, 500, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("[ 8 ] - Secondary Shot", 50, 550, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	
}
void	CHowToPlayState::Exit(void)
{
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
CHowToPlayState*	CHowToPlayState::GetInstance(void)
{
	if(!sm_pHowToPlayInstance)
	{
		sm_pHowToPlayInstance = new CHowToPlayState();
	}
	return sm_pHowToPlayInstance;
}
void CHowToPlayState::DeleteInstance(void)
{
	if(sm_pHowToPlayInstance)
	{
		delete sm_pHowToPlayInstance;
		sm_pHowToPlayInstance = NULL;
	}
}