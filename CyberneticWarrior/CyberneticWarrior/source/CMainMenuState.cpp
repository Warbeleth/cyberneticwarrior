#include "CMainMenuState.h"

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"

#include "CEventSystem.h"
#include "CEvent.h"

#include "CStackStateMachine.h"
#include "CSinglePlayerState.h"
#include "CSinglePlayerMenuState.h"
#include "COptionsMenuState.h"

CMainMenuState*	CMainMenuState::sm_pMainMenuInstance = NULL;

CMainMenuState::CMainMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	this->m_nBGMusic			= -1;

	this->m_nSelectionPos		= this->MMENU_START;
	this->m_nSelection			= this->SINGLE_PLAYER;

}

CMainMenuState::~CMainMenuState(void)
{

	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	this->m_nBGMusic			= -1;
}

CMainMenuState*	CMainMenuState::GetInstance(void)
{
	if(sm_pMainMenuInstance == NULL)
	{
		sm_pMainMenuInstance = new CMainMenuState();
	}
	return sm_pMainMenuInstance;
}

void	CMainMenuState::DeleteInstance(void)
{
	if(sm_pMainMenuInstance != NULL)
	{
		delete sm_pMainMenuInstance;
		sm_pMainMenuInstance = NULL;
	}
}
	
void	CMainMenuState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		= 		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();
	
	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/BackGroundMenu.png");
	this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/MainMenuCursor.png");
	this->m_nBGMusic			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");

	this->m_MenuFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");
	
	//this->m_pWM->Play(this->m_nBGMusic, DSBPLAY_LOOPING);

	this->m_nSelectionPos		= this->MMENU_START;
	this->m_nSelection			= this->SINGLE_PLAYER;
}	

bool	CMainMenuState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP)|| this->m_pDI->JoystickDPadPressed(DIR_UP, 0))
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->SINGLE_PLAYER)
		{
			this->m_nSelection = this->EXIT_GAME;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN)|| this->m_pDI->JoystickDPadPressed(DIR_DOWN, 0))
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->EXIT_GAME)
		{
			this->m_nSelection = this->SINGLE_PLAYER;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN) || this->m_pDI->JoystickButtonPressed((0||1||2||9), 0))
	{
		switch(this->m_nSelection)
		{
		case this->SINGLE_PLAYER:
			this->m_pWM->Stop(this->m_nBGMusic);
			//CStackStateMachine::GetInstance()->Push_Back(CSinglePlayerState::GetInstance());
			CStackStateMachine::GetInstance()->Push_Back(CSinglePlayerMenuState::GetInstance());
			break;
		case this->MM_OPTIONS:
			this->m_pWM->Stop(this->m_nBGMusic);
			CStackStateMachine::GetInstance()->Push_Back(COptionsMenuState::GetInstance());
			break;
		case this->EXIT_GAME:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}



	return 1;
}


void	CMainMenuState::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * MMENU_SPACE) + this->MMENU_START;
}


void	CMainMenuState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);

	this->m_MenuFont.Draw("Cybernetic Warrior", 50, 25, 2.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	
	this->m_MenuFont.Draw("Single Player", 225, (this->SINGLE_PLAYER * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->SINGLE_PLAYER? 1.5f : 1.0f) ,
		(this->m_nSelection == this->SINGLE_PLAYER? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Multi Player", 225, (this->MULTI_PLAYER * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->MULTI_PLAYER? 1.5f : 1.0f), 
		(this->m_nSelection == this->MULTI_PLAYER? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Leader Boards", 225, (this->LEADER_BOARDS * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->LEADER_BOARDS? 1.5f : 1.0f),
		(this->m_nSelection == this->LEADER_BOARDS? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Achievements", 225, (this->ACHIEVEMENTS * MMENU_SPACE) + this->MMENU_START,
		(this->m_nSelection == this->ACHIEVEMENTS? 1.5f : 1.0f),
		(this->m_nSelection == this->ACHIEVEMENTS? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Options", 225, (this->MM_OPTIONS * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->MM_OPTIONS? 1.5f : 1.0f), 
		(this->m_nSelection == this->MM_OPTIONS? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Controls", 225, (this->MM_CONTROLS * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->MM_CONTROLS? 1.5f : 1.0f),
		(this->m_nSelection == this->MM_CONTROLS? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Credits", 225, (this->CREDITS * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->CREDITS? 1.5f : 1.0f),
		(this->m_nSelection == this->CREDITS? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_MenuFont.Draw("Exit Game", 225, (this->EXIT_GAME * MMENU_SPACE) + this->MMENU_START, 
		(this->m_nSelection == this->EXIT_GAME? 1.5f : 1.0f),
		(this->m_nSelection == this->EXIT_GAME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_pTM->Draw(this->m_nCursorID,this->MMCURSOR_POS,this->m_nSelectionPos);
}

void	CMainMenuState::Exit(void)
{
	/*if(  > -1)
	{
		
	}*/
	this->m_MenuFont.ShutdownFont();

	if(this->m_nBGMusic > -1)
	{
		//this->m_pWM->Stop(this->m_nBGMusic);
		this->m_pWM->UnloadWave(this->m_nBGMusic);
		this->m_nBGMusic = -1;
	}

	if(this->m_nCursorID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nCursorID);
		this->m_nCursorID = -1;
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

