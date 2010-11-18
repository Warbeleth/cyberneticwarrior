#include "PrecompiledHeader.h"
#include "CGameProfiler.h"
#include "CStackStateMachine.h"
#include "CGame.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CLoadingState.h"
#include <fstream>

CGameProfiler*	CGameProfiler::sm_pGameProfilerInstance = NULL;

CGameProfiler::CGameProfiler(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_fWaitTime = 0.0f;

	this->m_bGetName = false;

	this->m_nBackgroundID			= -1;
	this->m_nProfileItemID			= -1;
	//this->m_nCursorID				= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection				= this->OP1;
	this->m_nSelectionPos			= this->MENU_START;

	this->m_szFileName[this->OP1] = "New Game";
	this->m_szFileName[this->OP2] = "New Game";
	this->m_szFileName[this->OP3] = "New Game";

	this->m_sPlayerNames[this->OP1] = "No Saved File";
	this->m_sPlayerNames[this->OP2] = "No Saved File";
	this->m_sPlayerNames[this->OP3] = "No Saved File";

	this->m_bReturn = false;

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
	if(this->m_bGetName)
	{
		this->CreatePlayerName();
	}
	
	if(this->m_bReturn)
	{
		this->m_bReturn = false;
		return 1;
	}

	if(this->m_pDI->KeyPressed(DIK_UP) || this->m_pDI->JoystickDPadPressed(DIR_UP) || this->m_pDI->JoystickGetLStickYAmount(0) < 0.0f && this->m_fWaitTime > 0.3f)
	{
		--this->m_nSelection;
		this->m_fWaitTime = 0.0f;

		if(this->m_nSelection < this->OP1)
		{
			this->m_nSelection = this->BACK;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_DOWN) || this->m_pDI->JoystickDPadPressed(DIR_DOWN) || this->m_pDI->JoystickGetLStickYAmount(0) > 0.0f && this->m_fWaitTime > 0.3f)
	{
		++this->m_nSelection;
		this->m_fWaitTime = 0.0f;

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
				if(this->GetNewGame())
				{
					this->m_bGetName = true;
				}
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
			CSinglePlayerState::GetInstance()->SetCurrentLevel(TUTORIAL);
			CSinglePlayerState::GetInstance()->SetPreviousLevel(-1);
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
						CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
					}
				}
				fManager.close();	
			}
			/*if(this->GetNewGame() && this->GetManagement() == SAVE_GAME)
			{
				CSinglePlayerState::GetInstance()->SetProfileValues(0);
				CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
			}*/
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

	//this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");
	this->m_OptionsFont.InitFont("resource/fonts/CyberneticWarriorArcadeFont.png", "resource/fonts/CyberneticWarriorArcadeFont.fnt");

	this->m_nSelection			= this->OP1;
	this->m_nSelectionPos		= this->MENU_START;
}

void	CGameProfiler::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * GMENU_SPACE) + this->MENU_START;
	this->m_fWaitTime += fElapsedTime;
}

void	CGameProfiler::Render(void)
{
	RECT Profiles = { 0, 0, 346, 83 };
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP1 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP1? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP1? D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 0.3f)));
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP2 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP2? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP2? D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 0.3f)));
	this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP3 * GMENU_SPACE) + this->MENU_START,1.4f,
		(this->m_nSelection == this->OP3? 0.75f : 0.7f),&Profiles,0.0f,0.0f,0.0f,
		(this->m_nSelection == this->OP3? D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 0.3f)));

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
	this->m_OptionsFont.Draw(this->m_szFileName[this->OP1], 275, (this->OP1 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP1? 1.5f : 1.0f) ,
		(this->m_nSelection == this->OP1? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw(this->m_szFileName[this->OP2], 275, (this->OP2 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP2? 1.5f : 1.0f), 
		(this->m_nSelection == this->OP2? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw(this->m_szFileName[this->OP3], 275, (this->OP3 * GMENU_SPACE) + this->MENU_START + 15, 
		(this->m_nSelection == this->OP3? 1.5f : 1.0f), 
		(this->m_nSelection == this->OP3? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f)));

	this->m_OptionsFont.Draw("Return", 325, (this->BACK * GMENU_SPACE) + this->MENU_START, 
		(this->m_nSelection == this->BACK? 1.5f : 1.0f), 
		(this->m_nSelection == this->BACK? D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f) : D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f)));

	if(this->m_bGetName)
	{
		this->m_pTM->Draw(this->m_nProfileItemID,150, (this->OP2 * GMENU_SPACE) + this->MENU_START,1.4f,
		0.75f ,&Profiles,0.0f,0.0f,0.0f,D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f));
		this->m_OptionsFont.Draw(this->m_sPlayerNames[this->m_nSelection].c_str(), 250, 252, 1.0f, D3DXCOLOR(0.2f, 0.2f, 1.0f, 1.0f));
	}
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

void	CGameProfiler::CreatePlayerName(void)
{
	static bool bSaveName = false;
	static int nCharCount = 0;
	
	if(!bSaveName)
	{
		switch(this->m_pDI->GetBufferedDIKCodeEx())
		{
		case DIK_RETURN:
			bSaveName = true;
			break;
		case DIK_ESCAPE:
			this->m_bGetName = false;
			this->m_sPlayerNames[this->m_nSelection].clear();
			nCharCount = 0;
			break;
		case DIK_BACKSPACE:
			if(nCharCount>0)
			{
				this->m_sPlayerNames[this->m_nSelection].clear();
				nCharCount = 0;		
			}
			break;
		case DIK_A:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("A");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("a");
				}	
				nCharCount++;
			}
			break;
		case DIK_B:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("B");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("b");
				}	
				nCharCount++;
			}
			break;
		case DIK_C:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("C");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("c");
				}	
				nCharCount++;
			}
			break;
		case DIK_D:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("D");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("c");
				}	
				nCharCount++;
			}
			break;
		case DIK_E:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("E");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("e");
				}	
				nCharCount++;
			}
			break;
		case DIK_F:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("F");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("f");
				}	
				nCharCount++;
			}
			break;
		case DIK_G:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("G");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("g");
				}	
				nCharCount++;
			}
			break;
		case DIK_H:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("H");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("h");
				}	
				nCharCount++;
			}
			break;
		case DIK_I:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("I");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("i");
				}	
				nCharCount++;
			}
			break;
		case DIK_J:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("J");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("j");
				}	
				nCharCount++;
			}
			break;
		case DIK_K:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("K");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("k");
				}	
				nCharCount++;
			}
			break;
		case DIK_L:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("L");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("l");
				}	
				nCharCount++;
			}
			break;
		case DIK_M:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("M");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("m");
				}	
				nCharCount++;
			}
			break;
		case DIK_N:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("N");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("n");
				}	
				nCharCount++;
			}
			break;
		case DIK_O:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("O");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("o");
				}	
				nCharCount++;
			}
			break;
		case DIK_P:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("P");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("p");
				}	
				nCharCount++;
			}
			break;
		case DIK_Q:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("Q");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("q");
				}	
				nCharCount++;
			}
			break;
		case DIK_R:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("R");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("r");
				}	
				nCharCount++;
			}
			break;
		case DIK_S:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("S");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("s");
				}	
				nCharCount++;
			}
			break;
		case DIK_T:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("T");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("t");
				}	
				nCharCount++;
			}
			break;
		case DIK_U:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("U");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("u");
				}	
				nCharCount++;
			}
			break;
		case DIK_V:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("V");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("v");
				}	
				nCharCount++;
			}
			break;
		case DIK_W:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("W");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("w");
				}	
				nCharCount++;
			}
			break;
		case DIK_X:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("X");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("x");
				}	
				nCharCount++;
			}
			break;
		case DIK_Y:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("Y");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("y");
				}	
				nCharCount++;
			}
			break;
		case DIK_Z:
			if(nCharCount<8)
			{
				if(nCharCount == 0)
				{
					this->m_sPlayerNames[this->m_nSelection].clear();
					this->m_sPlayerNames[this->m_nSelection].append("Z");
				}
				else
				{				
					this->m_sPlayerNames[this->m_nSelection].append("z");
				}	
				nCharCount++;
			}
		break;
		}

	}
	else
	{

		CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
		this->m_bGetName = false;
		this->m_bReturn = true;
		bSaveName = false;
	}
}
void	CGameProfiler::SetPlayerName(void)
{
}