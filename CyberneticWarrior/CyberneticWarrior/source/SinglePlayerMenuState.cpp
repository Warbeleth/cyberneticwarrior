#include "PrecompiledHeader.h"

#include "CSinglePlayerMenuState.h"
#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CSaveState.h"
#include "CLoadState.h"
#include "CEventSystem.h"
#include "CEvent.h"

CSinglePlayerMenuState*	CSinglePlayerMenuState::sm_pSinglePlayerMenuInstance = NULL;

CSinglePlayerMenuState::CSinglePlayerMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->NEW_GAME;
	this->m_nSelectionPos		= this->SMENU_START;
}

CSinglePlayerMenuState::~CSinglePlayerMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->NEW_GAME;
	this->m_nSelectionPos		= this->SMENU_START;
}

bool	CSinglePlayerMenuState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP))
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->NEW_GAME)
		{
			this->m_nSelection = this->BACK;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN))
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->BACK)
		{
			this->m_nSelection = this->NEW_GAME;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(this->m_nSelection)
		{
		case this->NEW_GAME:
			//this->m_pWM->Stop(this->m_nBGMusic);
			CSaveState::GetInstance()->SetNewGame(1);
			CSaveState::GetInstance()->SetDelete(0);
			CStackStateMachine::GetInstance()->Push_Back(CSaveState::GetInstance());
			break;
		case this->LOAD:
			CStackStateMachine::GetInstance()->Push_Back(CLoadState::GetInstance());
			break;
		case this->DELETE_PROF:
			CSaveState::GetInstance()->SetNewGame(1);
			CSaveState::GetInstance()->SetDelete(1);
			CStackStateMachine::GetInstance()->Push_Back(CSaveState::GetInstance());
			break;
		case this->BACK:
			//PostQuitMessage(0);
			CStackStateMachine::GetInstance()->Pop_back();
			break;
		default:
			break;
		}
	}
	return 1;
}

void	CSinglePlayerMenuState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/BackGroundMenu.png");
	this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/hook.png");
	//this->m_nMusicID			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");
	//this->m_nSFXID				= this->m_pWM->LoadWave("");

	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");


	this->m_nSelection			= this->NEW_GAME;
	this->m_nSelectionPos		= this->SMENU_START;
}

void	CSinglePlayerMenuState::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * SPMENU_SPACE) + this->SMENU_START;
}

void	CSinglePlayerMenuState::Render(void)
{
	
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	this->m_OptionsFont.Draw("Single Player", 250, 100, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	
	this->m_OptionsFont.Draw("New", 225, (this->NEW_GAME * SPMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->NEW_GAME? 1.5f : 1.0f) ,
		(this->m_nSelection == this->NEW_GAME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Load", 225, (this->LOAD * SPMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->LOAD? 1.5f : 1.0f), 
		(this->m_nSelection == this->LOAD? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Delete", 225, (this->DELETE_PROF * SPMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->DELETE_PROF? 1.5f : 1.0f), 
		(this->m_nSelection == this->DELETE_PROF? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Back", 225, (this->BACK * SPMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->BACK? 1.5f : 1.0f), 
		(this->m_nSelection == this->BACK? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));

	
	
}

void	CSinglePlayerMenuState::Exit(void)
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

CSinglePlayerMenuState*	CSinglePlayerMenuState::GetInstance(void)
{
	if(!sm_pSinglePlayerMenuInstance)
	{
		sm_pSinglePlayerMenuInstance = new CSinglePlayerMenuState();
	}
	return sm_pSinglePlayerMenuInstance;
}
void CSinglePlayerMenuState::DeleteInstance(void)
{
	if(sm_pSinglePlayerMenuInstance)
	{
		delete sm_pSinglePlayerMenuInstance;
		sm_pSinglePlayerMenuInstance = NULL;
	}
}