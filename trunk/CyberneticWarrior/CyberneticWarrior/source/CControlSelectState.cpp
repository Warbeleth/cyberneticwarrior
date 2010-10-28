#include "CControlSelectState.h"
#include "CStackStateMachine.h"
#include "CSinglePlayerState.h"
#include "CLoadingState.h"

CControlSelectState*		CControlSelectState::sm_pControlSelectInstance = NULL;

CControlSelectState::CControlSelectState(void)
{
	this->m_pD3D = NULL;
	this->m_pDI  = NULL;
	this->m_pTM  = NULL;
	this->m_pWM  = NULL;
	this->m_pDS  = NULL;

	this->m_nBackgroundID = -1;
	this->m_nSelection    = -1;
}

CControlSelectState::~CControlSelectState(void)
{
	this->m_pD3D = NULL;
	this->m_pDI  = NULL;
	this->m_pTM  = NULL;
	this->m_pWM  = NULL;
	this->m_pDS  = NULL;

	this->m_nBackgroundID = -1;
	this->m_nSelection    = -1;
}

bool	CControlSelectState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_RIGHT))
	{
		if(this->m_nSelection == this->GAMEPAD)
		{
			this->m_nSelection = this->KEYBOARD;
		}
		else if(this->m_nSelection == this->KEYBOARD)
		{
			this->m_nSelection = this->GAMEPAD;
		}
	}
	if(this->m_pDI->KeyPressed(DIK_LEFT))
	{
		if(this->m_nSelection == this->GAMEPAD)
		{
			this->m_nSelection = this->KEYBOARD;
		}
		else if(this->m_nSelection == this->KEYBOARD)
		{
			this->m_nSelection = this->GAMEPAD;
		}
	}
	if(this->m_pDI->KeyPressed(DIK_DOWN))
	{
		if(this->m_nSelection != this->EXIT_MENU)
		{
			this->m_nSelection = this->EXIT_MENU;
		}
		else
		{
			this->m_nSelection = this->KEYBOARD;
		}
	}
	if(this->m_pDI->KeyPressed(DIK_UP))
	{
		if(this->m_nSelection != this->EXIT_MENU)
		{
			this->m_nSelection = this->EXIT_MENU;
		}
		else
		{
			this->m_nSelection = this->GAMEPAD;
		}
	}

	if(this->m_pDI->KeyPressed(DIK_RETURN))
	{
		switch(this->m_nSelection)
		{
		case this->GAMEPAD:
			CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
			return 1;
			break;
		case this->KEYBOARD:
			CStackStateMachine::GetInstance()->ChangeState(CSinglePlayerState::GetInstance());
			return 1;
			break;
		case this->EXIT_MENU:
			CStackStateMachine::GetInstance()->Pop_back();
			return 1;
			break;
		}
	}
		

	if(this->m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CStackStateMachine::GetInstance()->Pop_back();
		return 1;
	}
	return 1;
}

void	CControlSelectState::Enter(void)
{
	this->m_pD3D			= CSGD_Direct3D::GetInstance();
	this->m_pDI				= CSGD_DirectInput::GetInstance();
	this->m_pTM				= CSGD_TextureManager::GetInstance();
	this->m_pWM				= CSGD_WaveManager::GetInstance();
	this->m_pDS				= CSGD_DirectSound::GetInstance();
	
	this->m_nBackgroundID	= m_pTM->LoadTexture("resource/graphics/fantasy_17.jpg");
	this->m_nKeys			= m_pTM->LoadTexture("resource/graphics/Keys.png");
	this->m_nGamePad		= m_pTM->LoadTexture("resource/graphics/GamePad2.png");

	this->m_ControlSelFont.InitFont("resource/fonts/example.png", "resource/fonts/Example.fnt");
	
	this->m_nSelection = this->KEYBOARD;	

}

void	CControlSelectState::Update(float fElapsedTime)
{
}

void	CControlSelectState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0, 1.0f, 1.0f, 0, 0.0f, 0.0f, 0.0f, D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f));

	this->m_ControlSelFont.Draw("-Choose Input-", 220, 50, 1.2f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));

	this->m_pTM->Draw(this->m_nKeys, 400, 90, (this->m_nSelection == this->KEYBOARD ? 0.75f : 0.7f), 
		(this->m_nSelection == this->KEYBOARD ? 0.75f : 0.7f), 0, 0.0f, 0.0f, 0.0f,
		(this->m_nSelection == this->KEYBOARD ? D3DXCOLOR(1.0f,1.0f, 0.75f, 1.0f) : D3DXCOLOR(1.0f,1.0f, 1.0f, 0.5f)));

	this->m_pTM->Draw(this->m_nGamePad, 50, 135, (this->m_nSelection == this->GAMEPAD ? 0.55f : 0.5f), 
		(this->m_nSelection == this->GAMEPAD ? 0.55f : 0.5f), 0, 0.0f, 0.0f, 0.0f, 
		(this->m_nSelection == this->GAMEPAD ? D3DXCOLOR(1.0f,1.0f, 0.75f, 1.0f) : D3DXCOLOR(1.0f,1.0f, 1.0f, 0.5f)));

	this->m_ControlSelFont.Draw("Back", 320, 430, (this->m_nSelection == this->EXIT_MENU ? 1.5f : 1.2f),
		(this->m_nSelection == this->EXIT_MENU ? D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)));


}

void	CControlSelectState::Exit(void)
{
	this->m_ControlSelFont.ShutdownFont();

	if(this->m_nGamePad > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nGamePad);
		this->m_nGamePad = -1;
	}
	if(this->m_nKeys > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nKeys);
		this->m_nKeys = -1;
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

CControlSelectState*	CControlSelectState::GetInstance(void)
{
	if(!sm_pControlSelectInstance)
	{
		sm_pControlSelectInstance = new CControlSelectState();
	}
	return sm_pControlSelectInstance;
}

void CControlSelectState::DeleteInstance(void)
{
	if(sm_pControlSelectInstance)
	{
		delete sm_pControlSelectInstance;
		sm_pControlSelectInstance = NULL;
	}
}