#include "PrecompiledHeader.h"
#include "CGameProfiler.h"
#include "CStackStateMachine.h"
#include "CGame.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CControlSelectState.h"
#include <fstream>

CGameProfiler*	CGameProfiler::sm_pGameProfilerInstance = NULL;

CGameProfiler::CGameProfiler(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID			= -1;
	this->m_nProfileItemID			= -1;
	//this->m_nCursorID				= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection				= this->OP1;
	this->m_nSelectionPos			= this->MENU_START;

	this->m_nFileName[this->OP1] = "New Game";
	this->m_nFileName[this->OP2] = "New Game";
	this->m_nFileName[this->OP3] = "New Game";


	this->SetNewGame(0);
	this->SetManagement(0);
}

CGameProfiler::~CGameProfiler(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID			= -1;
	this->m_nProfileItemID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection				= this->OP1;
	this->m_nSelectionPos			= this->MENU_START;
	this->SetManagement(0);
	this->SetNewGame(1);
}


bool	CGameProfiler::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP) || this->m_pDI->JoystickDPadPressed(DIR_UP) || this->m_pDI->JoystickGetLStickYAmount(0) > 0.0f)
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->OP1)
		{
			this->m_nSelection = this->BACK;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_DOWN) || this->m_pDI->JoystickDPadPressed(DIR_DOWN) || this->m_pDI->JoystickGetLStickYAmount(0) < 0.0f)
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->BACK)
		{
			this->m_nSelection = this->OP1;
		}
	}

	//this->m_pWM->Stop(this->m_nBGMusic);

	if(this->m_pDI->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(8, 0) || this->m_pDI->JoystickButtonPressed(2, 0))
	{
		CStackStateMachine::GetInstance()->Pop_back();
		return 1;
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN) || this->m_pDI->JoystickButtonPressed(9, 0) || this->m_pDI->JoystickButtonPressed(1, 0))
	{
		std::fstream fManager;
		const char * szFileName;
		bool bReturn = 0;
		switch(this->m_nSelection)
		{
		case this->OP1:
			{
				szFileName = "Profile-1.bin";
			}
			break;
		case this->OP2:
			{
				szFileName = "Profile-2.bin";
			}
			break;
		case this->OP3:
			{
				szFileName = "Profile-3.bin";
			}
			break;
		case this->BACK:
			//PostQuitMessage(0);
			bReturn = 1;
			CStackStateMachine::GetInstance()->Pop_back();
			return 1;
			break;
		default:
			break;
		}

		if(!bReturn)
		{
			if(this->GetManagement() != LOAD_GAME)
			{
				fManager.clear();
				fManager.open(szFileName, std::ios_base::out | std::ios_base::binary);
				// Saving
				if(fManager.is_open() && !this->GetNewGame()
					&& this->GetManagement() == SAVE_GAME)
				{
					fManager.write((const char*)&this->m_bNewGame, sizeof(bool));				
					fManager.write((const char*)&(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook), 
						sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook));
				}
				// Deleting
				if(fManager.is_open()
					&& this->GetManagement() == DELETE_PROFILE)
				{
					fManager.write((const char*)&this->m_bNewGame, sizeof(bool));
				}
				fManager.close();	
			}
			else
			{
				fManager.clear();
				fManager.open(szFileName, std::ios_base::in | std::ios_base::binary);
				// Loading
				if(fManager.is_open() && this->GetManagement() == LOAD_GAME)
				{
					bool thisBool = 0;
					fManager.read((char*)&thisBool, sizeof(bool));
					if(!thisBool)				
					{
						fManager.read((char*)&thisBool, sizeof(bool));
						CSinglePlayerState::GetInstance()->SetProfileValues(thisBool);
						CStackStateMachine::GetInstance()->Push_Back(CControlSelectState::GetInstance());
					}
				}
				fManager.close();	
			}
			if(this->GetNewGame() && this->GetManagement() == SAVE_GAME)
			{
				CSinglePlayerState::GetInstance()->SetProfileValues(0);
				CStackStateMachine::GetInstance()->Push_Back(CControlSelectState::GetInstance());
			}
		}
	}

	return 1;
}

void	CGameProfiler::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/ProfileBG.png");
	//this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/hook.png");
	this->m_nProfileItemID		= this->m_pTM->LoadTexture("resource/graphics/ProfileMenuItem.png");
	//this->m_nMusicID			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");
	//this->m_nSFXID				= this->m_pWM->LoadWave("");

	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");


	this->m_nSelection			= this->OP1;
	this->m_nSelectionPos		= this->MENU_START;
}

void	CGameProfiler::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * GMENU_SPACE) + this->MENU_START;
}

void	CGameProfiler::Render(void)
{
	RECT Profiles = { 0, 0, 346, 83 };
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP1 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP1? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP1? D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 0.3f)));
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP2 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP2? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP2? D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 0.3f)));
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP3 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP3? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP3? D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 0.3f)));

	if(this->GetManagement() == SAVE_GAME)
	{
		this->m_OptionsFont.Draw("SAVE", 335, 40, 1.2f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if(this->GetManagement() == DELETE_PROFILE)
	{
		this->m_OptionsFont.Draw("DELETE", 310, 40, 1.2f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if(this->GetManagement() == LOAD_GAME)
	{
		this->m_OptionsFont.Draw("LOAD", 335, 40, 1.2f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	this->m_OptionsFont.Draw(this->m_nFileName[this->OP1], 275, (this->OP1 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP1? 1.5f : 1.0f) ,
		(this->m_nSelection == this->OP1? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw(this->m_nFileName[this->OP2], 275, (this->OP2 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP2? 1.5f : 1.0f), 
		(this->m_nSelection == this->OP2? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw(this->m_nFileName[this->OP3], 275, (this->OP3 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP3? 1.5f : 1.0f), 
		(this->m_nSelection == this->OP3? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw("Return", 325, (this->BACK * GMENU_SPACE) + this->MENU_START, 
		(this->m_nSelection == this->BACK? 1.5f : 1.0f), 
		(this->m_nSelection == this->BACK? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f)));
}

void	CGameProfiler::Exit(void)
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

	if(this->m_nProfileItemID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nProfileItemID);
		this->m_nProfileItemID = NULL;
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

CGameProfiler*	CGameProfiler::GetInstance(void)
{
	if(!sm_pGameProfilerInstance)
	{
		sm_pGameProfilerInstance = new CGameProfiler();
	}
	return sm_pGameProfilerInstance;
}

void CGameProfiler::DeleteInstance(void)
{
	if(sm_pGameProfilerInstance)
	{
		delete sm_pGameProfilerInstance;
		sm_pGameProfilerInstance = NULL;
	}
}