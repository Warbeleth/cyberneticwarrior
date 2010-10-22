#include "COptionsMenuState.h"

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"

#include "CStackStateMachine.h"

#include "CEventSystem.h"
#include "CEvent.h"

COptionsMenuState*	COptionsMenuState::sm_pOptionsMenuInstance = NULL;

COptionsMenuState::COptionsMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	this->m_nSFXID				= -1;

	this->m_nMusicVolume		= 75;
	this->m_nSFXVolume			= 75;

	this->m_nSelection			= this->MUSIC_VOLUME;
	this->m_nSelectionPos		= this->OMENU_START;
	
}

COptionsMenuState::~COptionsMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	this->m_nSFXID				= -1;

	this->m_nSelection			= this->MUSIC_VOLUME;
	this->m_nSelectionPos		= this->OMENU_START;
}

COptionsMenuState*	COptionsMenuState::GetInstance(void)
{
	if(!sm_pOptionsMenuInstance)
	{
		sm_pOptionsMenuInstance = new COptionsMenuState();
	}
	return sm_pOptionsMenuInstance;
}

void	COptionsMenuState::DeleteInstance(void)
{
	if(sm_pOptionsMenuInstance)
	{
		delete sm_pOptionsMenuInstance;
		sm_pOptionsMenuInstance = NULL;
	}
}

void	COptionsMenuState::Enter(void)
{
	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		=		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();

	this->m_nBackgroundID		= this->m_pTM->LoadTexture("resource/graphics/BackGround.png");
	this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/hook.png");
	this->m_nMusicID			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");
	//this->m_nSFXID				= this->m_pWM->LoadWave("");

	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");


	this->m_nSelection			= this->MUSIC_VOLUME;
	this->m_nSelectionPos		= this->OMENU_START;
}

bool	COptionsMenuState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_UP))
	{
		--this->m_nSelection;

		if(this->m_nSelection < this->MUSIC_VOLUME)
		{
			this->m_nSelection = this->EXIT_OMENU;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN))
	{
		++this->m_nSelection;

		if(this->m_nSelection > this->EXIT_OMENU)
		{
			this->m_nSelection = this->MUSIC_VOLUME;
		}
	}

	if(this->m_pDI->KeyDown(DIK_RIGHT))
	{
		this->m_nMusicVolume++;

	}

	if(this->m_pDI->KeyDown(DIK_LEFT))
	{
		this->m_nMusicVolume--;
		
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(this->m_nSelection)
		{
		case this->MUSIC_VOLUME:
			//this->m_pWM->Stop(this->m_nBGMusic);
			//CStackStateMachine::GetInstance()->Push_Back(CSinglePlayerState::GetInstance());
			break;
		case this->SFX_VOLUME:
			break;
		case this->MUTE:
			break;
		case this->EXIT_OMENU:
			CStackStateMachine::GetInstance()->Pop_back();
			//PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	return 1;
}

void	COptionsMenuState::Update(float fElapsedTime)
{
	this->m_nSelectionPos = (this->m_nSelection * OMENU_SPACE) + this->OMENU_START;
}

void	COptionsMenuState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	this->m_OptionsFont.Draw("-OPTIONS MENU-", 200, 25, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	
	this->m_OptionsFont.Draw("Music Volume", 225, (this->MUSIC_VOLUME * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->MUSIC_VOLUME? 1.5f : 1.0f) ,
		(this->m_nSelection == this->MUSIC_VOLUME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("SFX Volume", 225, (this->SFX_VOLUME * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->SFX_VOLUME? 1.5f : 1.0f), 
		(this->m_nSelection == this->SFX_VOLUME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Mute", 250, (this->MUTE * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->MUTE? 1.5f : 1.0f),
		(this->m_nSelection == this->MUTE? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Exit", 250, (this->EXIT_OMENU * OMENU_SPACE) + this->OMENU_START,
		(this->m_nSelection == this->EXIT_OMENU? 1.5f : 1.0f),
		(this->m_nSelection == this->EXIT_OMENU? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
}

void	COptionsMenuState::Exit(void)
{
	this->m_OptionsFont.ShutdownFont();

	if(this->m_nSFXID > -1)
	{
		this->m_pWM->UnloadWave(this->m_nSFXID);
		this->m_nSFXID = NULL;
	}
	if(this->m_nMusicID > -1)
	{
		this->m_pWM->UnloadWave(this->m_nMusicID);
		this->m_nMusicID = NULL;
	}
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

int	COptionsMenuState::GetMusicVolume(void)	{return this->m_nMusicVolume;}
void COptionsMenuState::SetMusicVolume(int nMusicVolume) {this->m_nMusicVolume = nMusicVolume;}

int COptionsMenuState::GetSFXVolume(void)	{return this->m_nSFXVolume;}
void COptionsMenuState::SetSFXVolume(int nSFXVolume) {this->m_nSFXVolume = nSFXVolume;}

bool COptionsMenuState::GetMute(void) {return this->m_bMute;}
void COptionsMenuState::SetMute(bool bMute) {this->m_bMute = bMute;}