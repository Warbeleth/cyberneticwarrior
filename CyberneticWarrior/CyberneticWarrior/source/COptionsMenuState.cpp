#include "PrecompiledHeader.h"
#include "COptionsMenuState.h"
#include "CStackStateMachine.h"

COptionsMenuState*	COptionsMenuState::sm_pOptionsMenuInstance = NULL;

COptionsMenuState::COptionsMenuState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_fWaitTime			= 0.0f;

	this->m_nBackgroundID		= -1;
	this->m_nCursorID			= -1;
	this->m_nMuteOnID			= -1;
	this->m_nMuteOffID			= -1;
	
	this->m_nMusicID			= -1;
	this->m_nSFXID				= -1;
	
	this->m_nMusicVolume		= 75;
	this->m_nSFXVolume			= 75;

	this->m_nSelection			= this->MUSIC_VOLUME;
	this->m_nSelectionPos		= this->OMENU_START;

	this->m_bSelection			= 0;
	this->m_bMute				= 0;
		
	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;
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

	this->m_bSelection			= 0;
	this->m_bPlayMusic			= 0;
	this->m_bPlaySFX			= 0;
	this->m_bMute				= 0;
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
	this->m_nMuteOnID			= this->m_pTM->LoadTexture("resource/graphics/mute.png");
	this->m_nMuteOffID			= this->m_pTM->LoadTexture("resource/graphics/SoundOn.png");
	this->m_nCursorID			= this->m_pTM->LoadTexture("resource/graphics/shield.png");

	this->m_nMusicID			= this->m_pWM->LoadWave("resource/sounds/SO3_Victory_Bell.wav");
	//this->m_nSFXID				= this->m_pWM->LoadWave("resource/sounds/multiShots.wav");


	this->m_OptionsFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");

	this->m_pWM->SetVolume(this->m_nMusicID, this->m_nMusicVolume);

	this->m_nSelection			= this->MUSIC_VOLUME;
	this->m_nSelectionPos		= this->OMENU_START;
}

bool	COptionsMenuState::Input(void)
{
	m_bInput = false;

	if(this->m_pDI->KeyPressed(DIK_UP) || this->m_pDI->JoystickDPadPressed(DIR_UP,0))
	if(this->m_pDI->KeyPressed(DIK_UP) || this->m_pDI->JoystickDPadPressed(DIR_UP,0)
		|| (this->m_pDI->JoystickGetLStickYAmount(0) < 0.0f && this->m_fWaitTime > 0.3f))
	{
		m_bInput = true;
		if(this->m_bSelection)
		{
			this->m_bSelection = 0;
		}
		--this->m_nSelection;
		this->m_fWaitTime = 0.0f;
		if(this->m_nSelection < this->MUSIC_VOLUME)// && !this->m_bSelection)
		{
			this->m_nSelection = this->EXIT_OMENU;
		}
	}
	
	if(this->m_pDI->KeyPressed(DIK_DOWN) || this->m_pDI->JoystickDPadPressed(DIR_DOWN,0)
		|| (this->m_pDI->JoystickGetLStickYAmount(0) > 0.0f && this->m_fWaitTime > 0.3f))
	{
		if(this->m_bSelection)
		{
			this->m_bSelection = 0;
		}
		m_bInput = true;
		++this->m_nSelection;
		this->m_fWaitTime = 0.0f;
		if(this->m_nSelection > this->EXIT_OMENU)// && !this->m_bSelection)
		{
			this->m_nSelection = this->MUSIC_VOLUME;
		}
	}

	if(this->m_pDI->KeyDown(DIK_RIGHT) || this->m_pDI->JoystickDPadDown(DIR_RIGHT,0)
		|| this->m_pDI->JoystickGetLStickXAmount(0) > 0.0f)
	{
		m_bInput = true;
		if(this->m_nSelection == this->MUSIC_VOLUME)
		{
			if(this->m_nMusicVolume < 100 && this->m_bSelection && this->m_fWaitTime > 0.07f)
			{	
				this->m_fWaitTime = 0.0f;
				this->m_nMusicVolume++;
				this->m_pWM->SetVolume(m_nMusicID, this->m_nMusicVolume);
			}
			if(!this->m_bSelection)
			{
				this->m_bPlayMusic = 1;
				this->m_bSelection = 1;
			}
		}
		else if(this->m_nSelection == this->SFX_VOLUME)
		{
			if(this->m_nSFXVolume < 100 && this->m_bSelection && this->m_fWaitTime > 0.07f)
			{
				this->m_nSFXVolume++;
				this->m_fWaitTime = 0.0f;
				//this->m_pWM->SetVolume(m_nSFXID, this->m_nSFXVolume);
			}
			if(!this->m_bSelection)
			{
				this->m_bPlaySFX = 1;
				this->m_bSelection = 1;
			}
		}
	}

	if(this->m_pDI->KeyDown(DIK_LEFT) || this->m_pDI->JoystickDPadDown(DIR_LEFT,0)
		|| this->m_pDI->JoystickGetLStickXAmount(0))
	{
		m_bInput = true;
		if(this->m_nSelection == this->MUSIC_VOLUME)
		{
			if(this->m_nMusicVolume > 0 && this->m_bSelection && this->m_fWaitTime > 0.07f)
			{	
				this->m_nMusicVolume--;
				this->m_fWaitTime = 0.0f;
				this->m_pWM->SetVolume(m_nMusicID, this->m_nMusicVolume);
			}
			if(!this->m_bSelection)
			{
				this->m_bPlayMusic = 1;
				this->m_bSelection = 1;
			}
		}
		else if(this->m_nSelection == this->SFX_VOLUME)
		{
			if(this->m_nSFXVolume > 0 && this->m_bSelection && this->m_fWaitTime > 0.07f)
			{
				this->m_nSFXVolume--;
				this->m_fWaitTime = 0.0f;
				//this->m_pWM->SetVolume(m_nSFXID, this->m_nSFXVolume);
			}
			if(!this->m_bSelection)
			{
				this->m_bPlaySFX = 1;
				this->m_bSelection = 1;
			}
		}		
	}

	if(this->m_pDI->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(2,0) || this->m_pDI->JoystickButtonPressed(8,0))
	{
		m_bInput = true;
		if(this->m_bSelection)
		{
			this->m_bSelection = 0;
		}
		else
		{
			CStackStateMachine::GetInstance()->Pop_back();
			return 1;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN)  || this->m_pDI->JoystickButtonPressed(1,0) || this->m_pDI->JoystickButtonPressed(9,0))
	{
		m_bInput = true;

		switch(this->m_nSelection)
		{
		case this->MUSIC_VOLUME:
			//this->m_pWM->Stop(this->m_nBGMusic);
			//CStackStateMachine::GetInstance()->Push_Back(CSinglePlayerState::GetInstance());
			this->m_bSelection = 1;
			this->m_bPlayMusic = 1;
			break;
		case this->SFX_VOLUME:
			this->m_bSelection = 1;
			this->m_bPlaySFX = 1;
			break;
		case this->MUTE:
			this->m_bMute = !this->m_bMute;
			break;
		case this->EXIT_OMENU:
			CStackStateMachine::GetInstance()->Pop_back();
			return 1;
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
	this->m_fWaitTime += fElapsedTime;

	if(this->m_nSelection == this->MUSIC_VOLUME)
	{
		if(this->m_bSelection && this->m_bPlayMusic)
		{
			//this->m_pWM->Stop(m_nSFXID);
			this->m_pWM->Play(m_nMusicID);
			this->m_bPlayMusic = 0;
		}
	}
	else if(this->m_nSelection == this->SFX_VOLUME)
	{
		if(this->m_bSelection && this->m_bPlaySFX)
		{
			this->m_pWM->Stop(m_nMusicID);
			//this->m_pWM->Play(m_nSFXID);
		}
	}
	if(!this->m_bSelection)
	{
		this->m_pWM->Stop(m_nMusicID);
		//this->m_pWM->Stop(m_nSFXID);
	}

	this->m_nSelectionPos = (this->m_nSelection * OMENU_SPACE) + this->OMENU_START;

	AtractMode( fElapsedTime );
}

void	COptionsMenuState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);
	if(this->GetMute())
	{
		this->m_pTM->Draw(this->m_nMuteOnID, MENUX + 325, (this->MUTE * OMENU_SPACE) + this->OMENU_START - 10);
	}
	else
	{
		this->m_pTM->Draw(this->m_nMuteOffID, MENUX + 325, (this->MUTE * OMENU_SPACE) + this->OMENU_START - 10);
	}
	this->m_pTM->Draw(this->m_nCursorID, (int)(this->MENUX + 310 +(250 *(this->m_nMusicVolume * .01f))), (this->MUSIC_VOLUME * OMENU_SPACE) + this->OMENU_START - 9);
	this->m_pTM->Draw(this->m_nCursorID, (int)(this->MENUX + 310 +(250 *(this->m_nSFXVolume * .01f))), (this->SFX_VOLUME * OMENU_SPACE) + this->OMENU_START - 9);
	//this->m_pTM->Draw(this->m_nMenuID,0,0,1.3f,1.0f);//,1.0f,1.0f, 0, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,0,128,128));

	this->m_OptionsFont.Draw("-OPTIONS MENU-", 225, 25, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	
	this->m_OptionsFont.Draw("Music Volume", this->MENUX, (this->MUSIC_VOLUME * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->MUSIC_VOLUME? 1.2f : 1.0f) ,
		(this->m_nSelection == this->MUSIC_VOLUME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("SFX Volume", this->MENUX, (this->SFX_VOLUME * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->SFX_VOLUME? 1.2f : 1.0f), 
		(this->m_nSelection == this->SFX_VOLUME? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Mute", this->MENUX, (this->MUTE * OMENU_SPACE) + this->OMENU_START, 
		(this->m_nSelection == this->MUTE? 1.2f : 1.0f),
		(this->m_nSelection == this->MUTE? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	this->m_OptionsFont.Draw("Exit", this->MENUX + 225, (this->EXIT_OMENU * OMENU_SPACE) + this->OMENU_START,
		(this->m_nSelection == this->EXIT_OMENU? 1.2f : 1.0f),
		(this->m_nSelection == this->EXIT_OMENU? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));
	
	CSGD_Direct3D::GetInstance()->GetSprite()->Flush();
	CSGD_Direct3D::GetInstance()->DrawLine(MENUX + 325, (this->MUSIC_VOLUME * OMENU_SPACE) + this->OMENU_START + 25, MENUX + 325 + 250,  (this->MUSIC_VOLUME * OMENU_SPACE) + this->OMENU_START + 25, ((this->m_nSelection != this->MUSIC_VOLUME || !this->m_bSelection)? 255: 0), 0, ((this->m_nSelection == this->MUSIC_VOLUME && this->m_bSelection)? 255: 0));
	CSGD_Direct3D::GetInstance()->DrawLine(MENUX + 325, (this->SFX_VOLUME * OMENU_SPACE) + this->OMENU_START + 25, MENUX + 325 + 250,  (this->SFX_VOLUME * OMENU_SPACE) + this->OMENU_START + 25, ((this->m_nSelection != this->SFX_VOLUME || !this->m_bSelection)? 255: 0), 0, ((this->m_nSelection == this->SFX_VOLUME && this->m_bSelection)? 255: 0));
//	CSGD_Direct3D::GetInstance()->DrawRect(MENUX + 325, (this->MUTE * OMENU_SPACE) + this->OMENU_START + 25, MENUX + 325 + 250,  (this->MUTE * OMENU_SPACE) + this->OMENU_START + 25, 255, 0, 0);
}

void	COptionsMenuState::Exit(void)
{
	this->m_OptionsFont.ShutdownFont();

	if(this->m_nSFXID > -1)
	{
		this->m_pWM->UnloadWave(this->m_nSFXID);
		this->m_nSFXID = -1;
	}
	if(this->m_nMusicID > -1)
	{
		this->m_pWM->Stop(this->m_nMusicID);
		this->m_pWM->UnloadWave(this->m_nMusicID);
		this->m_nMusicID = -1;
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

int	COptionsMenuState::GetMusicVolume(void)	{return this->m_nMusicVolume;}
void COptionsMenuState::SetMusicVolume(int nMusicVolume) {this->m_nMusicVolume = nMusicVolume;}

int COptionsMenuState::GetSFXVolume(void)	{return this->m_nSFXVolume;}
void COptionsMenuState::SetSFXVolume(int nSFXVolume) {this->m_nSFXVolume = nSFXVolume;}

bool COptionsMenuState::GetMute(void) {return this->m_bMute;}
void COptionsMenuState::SetMute(bool bMute) {this->m_bMute = bMute;}