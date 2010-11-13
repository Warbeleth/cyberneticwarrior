#include "PrecompiledHeader.h"
#include "CHowToPlayState.h"
#include "CStackStateMachine.h"
#include "CGame.h"

CHowToPlayState* CHowToPlayState::sm_pHowToPlayInstance = NULL;

CHowToPlayState::CHowToPlayState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;
		
	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;

	m_bVisual = false;
	m_nScrollingID = -1;

	this->m_nBackgroundID		= -1;
	m_nVisualID = -1;
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
	m_bInput = false;

	if(this->m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		CStackStateMachine::GetInstance()->Pop_back();
		m_bInput = true;
	}

	if(m_pDI->GetInstance()->KeyDown(DIK_UP))
		m_OptionsFont.ChangeScrolling(0, -100, m_nScrollingID);
	else if(m_pDI->GetInstance()->KeyDown(DIK_DOWN))
		m_OptionsFont.ChangeScrolling(0, 100, m_nScrollingID);
	else
		m_OptionsFont.ChangeScrolling(0, 0, m_nScrollingID);

	if(m_pDI->GetInstance()->KeyPressed(DIK_SPACE))
		m_bVisual = !m_bVisual;

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
	m_nScrollingID = m_OptionsFont.AddScrolling( 0, 0 );

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/ControlsBG.png");
	m_nVisualID = m_pTM->LoadTexture("resource/graphics/HowToPlayVisual.png");
}
void	CHowToPlayState::Update(float fElapsedTime)
{
	m_OptionsFont.Update( fElapsedTime );
	AtractMode( fElapsedTime );
}
void	CHowToPlayState::Render(void)
{
	m_pTM->Draw(m_nBackgroundID, 0, 0);
	
	m_OptionsFont.Draw("HOW TO PLAY" , 280, 45, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_OptionsFont.DrawScrolling("KEYBOARD:\n     Jump - Space\n     Move Left - A\n     Move Right - D\n     Primary Weapon - Left Mouse Button\n     Secondary Weapon - Right Mouse Button\n     Swap Weapons - Tab\n     Head Slot(toggle) - Left Shift\n     Boot Slots:\n          No Boots - 1\n          Hover Boots - 2\n          Rocket Boots - 3\n     Climb Grapple Rope - W\n     Repel Grapple Rope - D\n\n\nGAMEPAD:\n     Movement - D-Pad / Left Joystick\n     Jumping - 2\n     Primary Weapon - 5\n     Secondary Weapon - 8",
		140, 100, .7f, D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f), 99, 530, 139, 700, m_nScrollingID );
	m_OptionsFont.Draw("(Space) Visual Controls Map     (Up Arrow) Scroll Up / (Down Arrow) Scroll Down" , 50, 580, .5f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	if(m_bVisual)
		m_pTM->Draw(m_nVisualID, 0, 0);
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