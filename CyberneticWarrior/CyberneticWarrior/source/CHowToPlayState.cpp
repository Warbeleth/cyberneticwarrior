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
	m_bInput = false;

	if(this->m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE))
	{
		CStackStateMachine::GetInstance()->Pop_back();
		m_bInput = true;
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
	if(m_pDI->KeyPressed(DIK_UP))
	{
		m_nSelection--;
		if(m_nSelection < 0)
			m_nSelection = 10;
	}
	else if(m_pDI->KeyPressed(DIK_DOWN))
	{
		m_nSelection++;
		if(m_nSelection > 10)
			m_nSelection = 0;
	}
	else if((m_pDI->CheckKeys() && !m_pDI->KeyPressed(DIK_RETURN) && !m_pDI->KeyPressed(DIK_ESCAPE)) || m_pDI->MouseButtonPressed(MOUSE_LEFT) || m_pDI->MouseButtonPressed(MOUSE_RIGHT))
	{
		if(m_nSelection != AIM_WEAPON && m_nSelection != SECONDARY_FIRE && m_nSelection != FIRE_WEAPON )
		{
			CGame::GetInstance()->SetPlayerOneControls(m_nSelection, m_pDI->GetBufferedDIKCodeEx());
		}
		else if( m_nSelection == FIRE_WEAPON )
		{
			if(m_pDI->MouseButtonDown(MOUSE_LEFT))
				CGame::GetInstance()->SetPlayerOneControls(m_nSelection, MOUSE_LEFT);
			else if(m_pDI->MouseButtonDown(MOUSE_RIGHT))
				CGame::GetInstance()->SetPlayerOneControls(m_nSelection, MOUSE_RIGHT);

		}
		else if( m_nSelection == SECONDARY_FIRE )
		{	
			if(m_pDI->MouseButtonDown(MOUSE_LEFT))
				CGame::GetInstance()->SetPlayerOneControls(m_nSelection, MOUSE_LEFT);
			else if(m_pDI->MouseButtonDown(MOUSE_RIGHT))
				CGame::GetInstance()->SetPlayerOneControls(m_nSelection, MOUSE_RIGHT);
		}

		m_nSelection = -1;
	}

	AtractMode( fElapsedTime );
}
void	CHowToPlayState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	
	this->m_OptionsFont.Draw("-HOW TO PLAY-"	, 50, 25, 1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("-KEYBOARD-"		, 50, 100, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	
	m_OptionsFont.Draw("Player One - JUMP", 50, (JUMP * MENU_SPACE) + MMENU_START, 
		(m_nSelection == JUMP ? .75f : .5f) ,
		(m_nSelection == JUMP ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));

	m_OptionsFont.Draw("Player One - LEFT", 50, (LEFT * MENU_SPACE) + MMENU_START, 
		(m_nSelection == LEFT ? .75f : .5f), 
		(m_nSelection == LEFT ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - RIGHT", 50, (RIGHT * MENU_SPACE) + MMENU_START, 
		(m_nSelection == RIGHT ? .75f : .5f),
		(m_nSelection == RIGHT ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - FIRE WEAPON", 50, (FIRE_WEAPON * MENU_SPACE) + MMENU_START,
		(m_nSelection == FIRE_WEAPON ? .75f : .5f),
		(m_nSelection == FIRE_WEAPON ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - SECONDARY FIRE", 50, (SECONDARY_FIRE * MENU_SPACE) + MMENU_START, 
		(m_nSelection == SECONDARY_FIRE ? .75f : .5f), 
		(m_nSelection == SECONDARY_FIRE ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - SWAP WEAPON", 50, ((SWAP_WEAPON-1) * MENU_SPACE) + MMENU_START, 
		(m_nSelection == SWAP_WEAPON ? .75f : .5f),
		(m_nSelection == SWAP_WEAPON ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - HEAD SLOT", 50, ((HEAD_SLOT-1) * MENU_SPACE) + MMENU_START, 
		(m_nSelection == HEAD_SLOT ? .75f : .5f),
		(m_nSelection == HEAD_SLOT ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - BOOT SLOT", 50, ((BOOT_SLOT-1) * MENU_SPACE) + MMENU_START, 
		(m_nSelection == BOOT_SLOT ? .75f : .5f),
		(m_nSelection == BOOT_SLOT ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - CLIMB ROPE", 50, ((CLIMB-1) * MENU_SPACE) + MMENU_START, 
		(m_nSelection == CLIMB ? .75f : .5f),
		(m_nSelection == CLIMB ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	m_OptionsFont.Draw("Player One - REPEL ROPE", 50, ((REPEL-1) * MENU_SPACE) + MMENU_START, 
		(m_nSelection == REPEL ? .75f : .5f),
		(m_nSelection == REPEL ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("-Game Pad-"		, 50, 11*MENU_SPACE + MMENU_START, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("D-Pad/Left JoyStick - Movement", 50, 121*MENU_SPACE + MMENU_START, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("(2) - Jump"		, 50, 13*MENU_SPACE + MMENU_START, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("[ 5 ] - Primary Shot", 50, 14*MENU_SPACE + MMENU_START, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	this->m_OptionsFont.Draw("[ 8 ] - Secondary Shot", 50, 15*MENU_SPACE + MMENU_START, 1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

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