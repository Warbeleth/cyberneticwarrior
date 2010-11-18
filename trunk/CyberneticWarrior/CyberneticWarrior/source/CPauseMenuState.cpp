#include "PrecompiledHeader.h"
#include "CPauseMenuState.h"
#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "COptionsMenuState.h"
#include "CSinglePlayerState.h"
#include "CHowToPlayState.h"
#include "CLoadingState.h"
#include "CPlayer.h"
#include "CGame.h"
#include "CGameProfiler.h"

CPauseMenuState*	CPauseMenuState::sm_pPauseMenuInstance = NULL;

CPauseMenuState::CPauseMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->RESUME;
	this->m_nSelectionPos		= this->PMENU_START;
}

CPauseMenuState::~CPauseMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_fWaitTime	= 0.0f;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->RESUME;
	this->m_nSelectionPos		= this->PMENU_START;
}

bool	CPauseMenuState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP) || this->m_pDI->JoystickDPadPressed(DIR_UP,0)
		|| (this->m_pDI->JoystickGetLStickYAmount(0) < 0.0f && this->m_fWaitTime > 0.3f))
	{
		--this->m_nSelection;
		this->m_fWaitTime = 0.0f;
		if(this->m_nSelection < this->RESUME)
		{
			this->m_nSelection = this->MAIN_MENU;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN) || this->m_pDI->JoystickDPadPressed(DIR_DOWN,0)
		|| (this->m_pDI->JoystickGetLStickYAmount(0) > 0.0f && this->m_fWaitTime > 0.3f))
	{
		++this->m_nSelection;
		this->m_fWaitTime = 0.0f;
		if(this->m_nSelection > this->MAIN_MENU)
		{
			this->m_nSelection = this->RESUME;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(2,0) || this->m_pDI->JoystickButtonPressed(9,0))
	{
		CSinglePlayerState::GetInstance()->SetJamming(true);
		CStackStateMachine::GetInstance()->Pop_back();
		return 1;
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN) || this->m_pDI->JoystickButtonPressed(1,0) || this->m_pDI->JoystickButtonPressed(8,0))
	{
		switch(this->m_nSelection)
		{
		case this->RESUME:
			//this->m_pWM->Stop(this->m_nBGMusic);
			if(COptionsMenuState::GetInstance()->GetMute())
			{
				CSinglePlayerState::GetInstance()->SetJamming(true);
			}
			CStackStateMachine::GetInstance()->Pop_back();

			break;
		case this->RESET:
			CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
			break;
		case this->SAVE:
			CGameProfiler::GetInstance()->SetNewGame(0);
			CGameProfiler::GetInstance()->SetManagement(SAVE_GAME);
			CStackStateMachine::GetInstance()->Push_Back(CGameProfiler::GetInstance());
			break;
		case this->CONTROLS:
			CStackStateMachine::GetInstance()->Push_Back(CHowToPlayState::GetInstance());
			break;
		case this->OPTIONS:
			CStackStateMachine::GetInstance()->Push_Back(COptionsMenuState::GetInstance());
			break;
		case this->MAIN_MENU:
			//PostQuitMessage(0);
			//CSinglePlayerState::GetInstance()->GetPlayerPointer()->SetShutDown(true);
			CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());		
			break;
		default:
			break;
		}
	}
	return 1;
}

void	CPauseMenuState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/PauseMenuBG.png");
	this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/hook.png");
	//this->m_nMusicID			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");
	//this->m_nSFXID				= this->m_pWM->LoadWave("");

	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");


	this->m_nSelection			= this->RESUME;
	this->m_nSelectionPos		= this->PMENU_START;
}

void	CPauseMenuState::Update(float fElapsedTime)
{
	this->m_fWaitTime += fElapsedTime;
	this->m_nSelectionPos = (this->m_nSelection * PMENU_SPACE) + this->PMENU_START;
}

void	CPauseMenuState::Render(void)
{
	
	this->m_pTM->Draw(this->m_nBackgroundID, 150, 50, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DXCOLOR(1.0f,1.0f,1.0f,0.5f));//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	this->m_OptionsFont.Draw("-PAUSED-", 300, 100, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f));
	
	this->m_OptionsFont.Draw("Resume", 275, (this->RESUME * PMENU_SPACE) + this->PMENU_START, 
		(this->m_nSelection == this->RESUME? 1.1f : 1.0f) ,
		(this->m_nSelection == this->RESUME? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw("Restart", 275, (this->RESET * PMENU_SPACE) + this->PMENU_START, 
		(this->m_nSelection == this->RESET? 1.1f : 1.0f) ,
		(this->m_nSelection == this->RESET? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Save", 275, (this->SAVE * PMENU_SPACE) + this->PMENU_START, 
		(this->m_nSelection == this->SAVE? 1.1f : 1.0f) ,
		(this->m_nSelection == this->SAVE? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Controls", 275, (this->CONTROLS * PMENU_SPACE) + this->PMENU_START, 
		(this->m_nSelection == this->CONTROLS? 1.1f : 1.0f), 
		(this->m_nSelection == this->CONTROLS? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw("Options", 275, (this->OPTIONS * PMENU_SPACE) + this->PMENU_START, 
		(this->m_nSelection == this->OPTIONS? 1.1f : 1.0f),
		(this->m_nSelection == this->OPTIONS? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Main Menu", 275, (this->MAIN_MENU * PMENU_SPACE) + this->PMENU_START,
		(this->m_nSelection == this->MAIN_MENU? 1.1f : 1.0f),
		(this->m_nSelection == this->MAIN_MENU? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));
	
}

void	CPauseMenuState::Exit(void)
{
	
	this->m_OptionsFont.ShutdownFont();

	/*if(this->m_nSFXID > -1)
	{
		this->m_pWM->UnloadWave(this->m_nSFXID);
		this->m_nSFXID = NULL;
	}
	if(this->m_nMusicID > -1)
	{
		this->m_pWM->UnloadWave(this->m_nMusicID);
		this->m_nMusicID = NULL;
	}*/
	if(this->m_nCursorID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nCursorID);
		this->m_nCursorID = NULL;
	}
	if(this->m_nBackgroundID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundID);
		this->m_nBackgroundID = NULL;
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

CPauseMenuState*	CPauseMenuState::GetInstance(void)
{
	if(!sm_pPauseMenuInstance)
	{
		sm_pPauseMenuInstance = new CPauseMenuState();
	}
	return sm_pPauseMenuInstance;
}

void CPauseMenuState::DeleteInstance(void)
{
	if(sm_pPauseMenuInstance)
	{
		delete sm_pPauseMenuInstance;
		sm_pPauseMenuInstance = NULL;
	}
}