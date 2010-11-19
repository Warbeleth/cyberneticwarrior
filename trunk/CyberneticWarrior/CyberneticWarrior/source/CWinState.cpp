#include "PrecompiledHeader.h"
#include "CWinState.h"
#include "CCreditsState.h"
#include "CStackStateMachine.h"
#include "CObjectManager.h"
#include "CSinglePlayerState.h"
#include "CGame.h"
#include "CExplosion.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//				 ******************* CLASS - CGameOverState *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
CWinState* CWinState::sm_pWinStateInstance = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CGameOverState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CWinState::CWinState(void)
{
	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;
	m_fElapsedTime = 0.0f;

	// Background
	m_nBackgroundID		= -1;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CGameOverState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CWinState::~CWinState(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Enter”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CWinState::Enter(void)
{
	this->m_pD3D	=	CSGD_Direct3D::GetInstance();
	this->m_pDI	=	CSGD_DirectInput::GetInstance();
	this->m_pTM	=	CSGD_TextureManager::GetInstance();
	this->m_pWM	=	CSGD_WaveManager::GetInstance();
	this->m_pDS	=	CSGD_DirectSound::GetInstance();

	this->m_WinStateFont.InitFont("resource/fonts/CyberneticWarriorChintzy.png", "resource/fonts/CyberneticWarriorChintzy.fnt", 64);
	this->m_nScrollingID = this->m_WinStateFont.AddScrolling( 0, 0 );

	CObjectManager::GetInstance()->RemoveAllObjects();

	this->m_nBackgroundID	= this->m_pTM->LoadTexture("resource/graphics/AchievementsBG.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CWinState::Input(void)
{
	/*if(m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(8, 0) || m_pDI->JoystickButtonPressed(2, 0))
	{
		CStackStateMachine::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return 1;
	}*/

	if(m_pDI->GetInstance()->KeyPressed(DIK_RETURN) ||  m_pDI->JoystickButtonPressed(1, 0))
	{
		CStackStateMachine::GetInstance()->ChangeState(CCreditsState::GetInstance());
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CWinState::Update(float fElapsedTime)
{	
	m_fElapsedTime += fElapsedTime;
	if( m_fElapsedTime >= 0.5f )
	{
		m_fElapsedTime = 0.0f;

		if(CObjectManager::GetInstance()->GetSize() <= 50)
			for( int i = 0; i < 25; ++i )
			{
				CExplosion* pExplosion = new CExplosion();
				(*pExplosion).SetPosX((float)(rand()%800));
				(*pExplosion).SetPosY((float)(rand()%600));
				(*pExplosion).SetColor( D3DXCOLOR((float)(rand()%255)/255.0f, (float)(rand()%255)/255.0f, (float)(rand()%255)/255.0f, 1.0f ) );

				CObjectManager::GetInstance()->AddObject(pExplosion);
				pExplosion->Release();
			}
		else
			CObjectManager::GetInstance()->RemoveAllObjects();
	}

	m_WinStateFont.Update( fElapsedTime );
	CObjectManager::GetInstance()->UpdateObjects(fElapsedTime);
	AtractMode( fElapsedTime );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CWinState::Render(void)
{
	m_pTM->Draw(m_nBackgroundID, 0, 0);
	CObjectManager::GetInstance()->RenderObjects();

	m_WinStateFont.Draw( "WINNER!", 200, 150, 1.5f, D3DXCOLOR(0.7f, 0.7f, 1.0f, 1.0f) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Exit”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CWinState::Exit(void)
{
	if(this->m_nBackgroundID != -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundID);
		this->m_nBackgroundID = -1;
	}

	CObjectManager::GetInstance()->RemoveAllObjects();

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
CWinState* CWinState::GetInstance(void)
{
	if(!sm_pWinStateInstance)
	{
		sm_pWinStateInstance = new CWinState();
	}
	return sm_pWinStateInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DeleteInstance”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CWinState::DeleteInstance(void)
{
	if(sm_pWinStateInstance)
	{
		delete sm_pWinStateInstance;
		sm_pWinStateInstance = NULL;
	}
}