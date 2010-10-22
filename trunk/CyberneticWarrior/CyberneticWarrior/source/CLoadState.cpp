#include "CLoadState.h"

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"

#include "CStackStateMachine.h"

#include "CMainMenuState.h"
#include "CSinglePlayerState.h"

#include "CEventSystem.h"
#include "CEvent.h"

#include <fstream>

CLoadState*	CLoadState::sm_pLoadMenuInstance = NULL;

CLoadState::CLoadState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->LOAD1;
	this->m_nSelectionPos		= this->LMENU_START;
}

CLoadState::~CLoadState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->LOAD1;
	this->m_nSelectionPos		= this->LMENU_START;
}

bool	CLoadState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP))
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->LOAD1)
		{
			this->m_nSelection = this->BACK;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN))
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->BACK)
		{
			this->m_nSelection = this->LOAD1;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN))
	{
		std::ifstream open;
		switch(this->m_nSelection)
		{
		case this->LOAD1:
			//CSinglePlayerState::GetInstance()->se
			//this->m_pWM->Stop(this->m_nBGMusic);
			//CStackStateMachine::GetInstance()->Pop_back();

			open.clear();
			open.open("Profile-1.bin", std::ios_base::in | std::ios_base::binary);
			if(open.is_open())
			{
				CSinglePlayerState::GetInstance()->SetProfileValues(open.read((char*)&(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook)));
					
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
			}

			break;
		case this->LOAD2:
			//CStackStateMachine::GetInstance()->Push_Back(COptionsMenuState::GetInstance());
			open.clear();
			open.open("Profile-2.bin", std::ios_base::in | std::ios_base::binary);
			if(open.is_open())
			{
				CSinglePlayerState::GetInstance()->SetProfileValues(open.read((char*)&(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook)));
					
				//if(!CSinglePlayerState::GetInstance()->GetNewGame())
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
			}
			break;
		case this->LOAD3:
			open.clear();
			open.open("Profile-3.bin", std::ios_base::in | std::ios_base::binary);
			if(open.is_open())
			{
				CSinglePlayerState::GetInstance()->SetProfileValues(open.read((char*)&(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook)));
					
				//if(!CSinglePlayerState::GetInstance()->GetNewGame())
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
			}
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

void	CLoadState::Enter(void)
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


	this->m_nSelection			= this->LOAD1;
	this->m_nSelectionPos		= this->LMENU_START;
}

void	CLoadState::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * LMENU_SPACE) + this->LMENU_START;
}

void	CLoadState::Render(void)
{
	
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	this->m_OptionsFont.Draw("LOAD", 250, 100, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	
	this->m_OptionsFont.Draw("Profile-1", 225, (this->LOAD1 * LMENU_SPACE) + this->LMENU_START, 
		(this->m_nSelection == this->LOAD1? 1.5f : 1.0f) ,
		(this->m_nSelection == this->LOAD1? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Profile-2", 225, (this->LOAD2 * LMENU_SPACE) + this->LMENU_START, 
		(this->m_nSelection == this->LOAD2? 1.5f : 1.0f), 
		(this->m_nSelection == this->LOAD2? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Profile-3", 225, (this->LOAD3 * LMENU_SPACE) + this->LMENU_START, 
		(this->m_nSelection == this->LOAD3? 1.5f : 1.0f), 
		(this->m_nSelection == this->LOAD3? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Return", 225, (this->BACK * LMENU_SPACE) + this->LMENU_START, 
		(this->m_nSelection == this->BACK? 1.5f : 1.0f), 
		(this->m_nSelection == this->BACK? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));

	
	
}

void	CLoadState::Exit(void)
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

CLoadState*	CLoadState::GetInstance(void)
{
	if(!sm_pLoadMenuInstance)
	{
		sm_pLoadMenuInstance = new CLoadState();
	}
	return sm_pLoadMenuInstance;
}
void CLoadState::DeleteInstance(void)
{
	if(sm_pLoadMenuInstance)
	{
		delete sm_pLoadMenuInstance;
		sm_pLoadMenuInstance = NULL;
	}
}