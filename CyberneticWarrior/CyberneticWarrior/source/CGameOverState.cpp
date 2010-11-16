#include "CGameOverState.h"
#include "PrecompiledHeader.h"

#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CLoadingState.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//				 ******************* CLASS - CGameOverState *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
CGameOverState* CGameOverState::sm_pGameOverStateInstance = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CGameOverState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CGameOverState::CGameOverState(void)
{
	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;

	// Background
	m_nBackgroundID		= -1;
	m_nSelection = this->CONTINUE;
	this->m_nStinger = -1;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CGameOverState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CGameOverState::~CGameOverState(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Enter”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameOverState::Enter(void)
{
	this->m_pD3D	=	CSGD_Direct3D::GetInstance();
	this->m_pDI	=	CSGD_DirectInput::GetInstance();
	this->m_pTM	=	CSGD_TextureManager::GetInstance();
	this->m_pWM	=	CSGD_WaveManager::GetInstance();
	this->m_pDS	=	CSGD_DirectSound::GetInstance();

	this->m_GameOverFont.InitFont("resource/fonts/CyberneticWarriorChintzy.png", "resource/fonts/CyberneticWarriorChintzy.fnt", 64);
	this->m_nScrollingID = this->m_GameOverFont.AddScrolling( 0, 0 );
	this->m_nStinger = this->m_pWM->LoadWave("resource/sounds/JTM_MainStinger.wav");
	this->m_pWM->Play(this->m_nStinger);

	this->m_nBackgroundID	= this->m_pTM->LoadTexture("resource/graphics/AchievementsBG.png");
	this->m_nSelection = this->CONTINUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGameOverState::Input(void)
{
	/*if(m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(8, 0) || m_pDI->JoystickButtonPressed(2, 0))
	{
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return 1;
	}*/

	if(m_pDI->GetInstance()->KeyPressed(DIK_RIGHT) || m_pDI->JoystickDPadPressed(DIR_RIGHT))
	{
		m_nSelection++;
		if(this->m_nSelection > this->EXIT)
		{
			this->m_nSelection = this->CONTINUE;
		}
	}
	
	if(m_pDI->GetInstance()->KeyPressed(DIK_LEFT) || m_pDI->JoystickDPadPressed(DIR_LEFT))
	{
		m_nSelection--;
		if(this->m_nSelection < this->CONTINUE)
		{
			this->m_nSelection = this->EXIT;
		}
	}

	if(m_pDI->GetInstance()->KeyPressed(DIK_RETURN) ||  m_pDI->JoystickButtonPressed(1, 0))
	{
		switch(this->m_nSelection)
		{
		case this->CONTINUE:
		CSinglePlayerState::GetInstance()->SetCurrentLevel(TUTORIAL);
		CSinglePlayerState::GetInstance()->SetPreviousLevel(-1);
		CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
		return 1;
		break;
		case this->EXIT:
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return 1;
		break;
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameOverState::Update(float fElapsedTime)
{
	m_GameOverFont.Update( fElapsedTime );

	AtractMode( fElapsedTime );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameOverState::Render(void)
{
	m_pTM->Draw(m_nBackgroundID, 0, 0);

	m_GameOverFont.Draw( "GAME OVER", 260, 45, 0.7f, D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) );

	this->m_GameOverFont.Draw("Continue?", 100, 250, 
		(this->m_nSelection == this->CONTINUE? 1.1f : 1.0f) ,
		(this->m_nSelection == this->CONTINUE? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));

	this->m_GameOverFont.Draw("Exit", 550, 250, 
		(this->m_nSelection == this->EXIT? 1.1f : 1.0f) ,
		(this->m_nSelection == this->EXIT? D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f) : D3DXCOLOR(0.7f, 1.0f, 1.0f, 1.0f)));;

//	m_GameOverFont.Draw( "Press Esc to Exit.", 240, 500, 0.5f, D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Exit”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameOverState::Exit(void)
{
	if(this->m_nBackgroundID != -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundID);
		this->m_nBackgroundID = -1;
	}
	if(this->m_nStinger != -1)
	{
		this->m_pWM->UnloadWave(this->m_nStinger);
		this->m_nStinger = -1;
	}

	if(m_pDS)
		m_pDS = NULL;

	if(m_pWM)
		m_pWM = NULL;

	if(m_pTM)
		m_pTM = NULL;

	if(m_pDI)
		m_pDI = NULL;
	
	if(m_pD3D)
		m_pD3D = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetInstance”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CGameOverState* CGameOverState::GetInstance(void)
{
	if(!sm_pGameOverStateInstance)
	{
		sm_pGameOverStateInstance = new CGameOverState();
	}
	return sm_pGameOverStateInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DeleteInstance”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CGameOverState::DeleteInstance(void)
{
	if(sm_pGameOverStateInstance)
	{
		delete sm_pGameOverStateInstance;
		sm_pGameOverStateInstance = NULL;
	}
}