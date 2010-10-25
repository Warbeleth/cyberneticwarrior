#include "PrecompiledHeader.h"

#include "CSaveState.h"
#include "CStackStateMachine.h"
#include "CGame.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CEventSystem.h"
#include "CEvent.h"

CSaveState*	CSaveState::sm_pSaveMenuInstance = NULL;

CSaveState::CSaveState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->SAVE1;
	this->m_nSelectionPos		= this->SMENU_START;

	this->SetNewGame(0);
	this->SetDelete(0);
}

CSaveState::~CSaveState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	//this->m_nSFXID				= -1;

	this->m_nSelection			= this->SAVE1;
	this->m_nSelectionPos		= this->SMENU_START;
}


bool	CSaveState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP))
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->SAVE1)
		{
			this->m_nSelection = this->BACK;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN))
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->BACK)
		{
			this->m_nSelection = this->SAVE1;
		}
	}

			//this->m_pWM->Stop(this->m_nBGMusic);
			//CStackStateMachine::GetInstance()->Pop_back();
			
	if(this->m_pDI->KeyPressed(DIK_RETURN))
	{
		std::ofstream save;
		switch(this->m_nSelection)
		{
		case this->SAVE1:
			{
				//std::ifstream open;
				save.clear();
				save.open("Profile-1.bin", std::ios_base::out | std::ios_base::binary);
				if(save.is_open() && !this->GetNewGame())
				{
					save.write((const char*)&(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()->m_bHaveHook));
				}
				if(save.is_open() && this->GetDelete())
				{
					remove("Profile-1.bin");
					//CSinglePlayerState::GetInstance()->SetNewGame(0);
				}
				save.close();

				if(this->GetNewGame() && !this->GetDelete())
				{
					CSinglePlayerState::GetInstance()->SetProfileValues(0);
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
				}

				break;
			}
		case this->SAVE2:
			{
				//CStackStateMachine::GetInstance()->Push_Back(COptionsMenuState::GetInstance());
				save.clear();
				save.open("Profile-2.bin", std::ios_base::out | std::ios_base::binary);
				if(save.is_open() && !this->GetNewGame())
				{
					save.write((const char*)&(*CSinglePlayerState::GetInstance()->GetProfileValues()), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()));
				}

				if(save.is_open() && this->GetDelete())
				{
					remove("Profile-2.bin");
				}
				save.close();
				if(this->GetNewGame() && !this->GetDelete())
				{
					CSinglePlayerState::GetInstance()->SetProfileValues(0);
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
				}
				break;		
			}
		case this->SAVE3:
			{
				save.clear();
				save.open("Profile-3.bin", std::ios_base::out | std::ios_base::binary);
				if(save.is_open() && !this->GetNewGame() && !this->GetDelete())
				{
					save.write((const char*)&(*CSinglePlayerState::GetInstance()->GetProfileValues()), sizeof(CSinglePlayerState::GetInstance()->GetProfileValues()));
				}
				if(save.is_open() && this->GetDelete())
				{
					remove("Profile-3.bin");
				}
				save.close();
				if(this->GetNewGame() && !this->GetDelete())
				{
					CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
				}

				break;
			}
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

void	CSaveState::Enter(void)
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


	this->m_nSelection			= this->SAVE1;
	this->m_nSelectionPos		= this->SMENU_START;
}

void	CSaveState::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * SMENU_SPACE) + this->SMENU_START;
}

void	CSaveState::Render(void)
{
	
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	if(!this->GetDelete())
	{
	this->m_OptionsFont.Draw("SAVE", 250, 100, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	else
	{
	this->m_OptionsFont.Draw("DELETE", 250, 100, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	}
	this->m_OptionsFont.Draw("Slot 1", 225, (this->SAVE1 * SMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->SAVE1? 1.5f : 1.0f) ,
		(this->m_nSelection == this->SAVE1? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Slot 2", 225, (this->SAVE2 * SMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->SAVE2? 1.5f : 1.0f), 
		(this->m_nSelection == this->SAVE2? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Slot 3", 225, (this->SAVE3 * SMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->SAVE3? 1.5f : 1.0f), 
		(this->m_nSelection == this->SAVE3? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));

	this->m_OptionsFont.Draw("Return", 225, (this->BACK * SMENU_SPACE) + this->SMENU_START, 
		(this->m_nSelection == this->BACK? 1.5f : 1.0f), 
		(this->m_nSelection == this->BACK? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));

	
	
}

void	CSaveState::Exit(void)
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

CSaveState*	CSaveState::GetInstance(void)
{
	if(!sm_pSaveMenuInstance)
	{
		sm_pSaveMenuInstance = new CSaveState();
	}
	return sm_pSaveMenuInstance;
}
void CSaveState::DeleteInstance(void)
{
	if(sm_pSaveMenuInstance)
	{
		delete sm_pSaveMenuInstance;
		sm_pSaveMenuInstance = NULL;
	}
}