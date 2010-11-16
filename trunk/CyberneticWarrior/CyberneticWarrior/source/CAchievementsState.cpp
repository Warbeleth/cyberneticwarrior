#include "PrecompiledHeader.h"

#include "CAchievementsState.h"
#include "CStackStateMachine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////
//				 ******************* CLASS - CAchievementsState *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
CAchievementsState* CAchievementsState::sm_pAchievementsInstance = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CAchievementsState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CAchievementsState::CAchievementsState(void)
{
	this->m_pD3D		= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	// Atract Mode
	m_fAtractMode = 60.0f;
	m_fAtractModeTimer = 0.0f;
	m_bInput = false;

	m_nBackgroundID		= -1;

	this->m_fWaitTime	= 0.0f;

	this->m_nBackgroundID		= -1;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CAchievementsState”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CAchievementsState::~CAchievementsState(void)
{

	m_pD3D		= NULL;
	m_pTM		= NULL;
	m_pDI		= NULL;
	m_pWM		= NULL;
	m_pDS		= NULL;

	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;


	this->m_nBackgroundID		= -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Enter”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAchievementsState::Enter(void)
{
	this->m_pD3D	=	CSGD_Direct3D::GetInstance();
	this->m_pDI	=	CSGD_DirectInput::GetInstance();
	this->m_pTM	=	CSGD_TextureManager::GetInstance();
	this->m_pWM	=	CSGD_WaveManager::GetInstance();
	this->m_pDS	=	CSGD_DirectSound::GetInstance();

	this->m_OptionsFont.InitFont("resource/fonts/CyberneticWarriorChintzy.png", "resource/fonts/CyberneticWarriorChintzy.fnt", 64);
	this->m_nScrollingID = m_OptionsFont.AddScrolling( 0, 0 );

	this->m_nBackgroundID	= m_pTM->LoadTexture("resource/graphics/AchievementsBG.png");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Input”
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAchievementsState::Input(void)
{
	if(this->m_pDI->GetInstance()->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(8, 0) || this->m_pDI->JoystickButtonPressed(2, 0))
	{
		CStackStateMachine::GetInstance()->Pop_back();
		return 1;
	}
	if(this->m_pDI->GetInstance()->KeyDown(DIK_UP) || this->m_pDI->JoystickDPadDown(DIR_UP) || (this->m_pDI->JoystickGetLStickYAmount() < 0.0f && this->m_fWaitTime > 0.3f))
	{
		this->m_OptionsFont.ChangeScrolling( 0, -100, this->m_nScrollingID );
	}
	else if(this->m_pDI->GetInstance()->KeyDown(DIK_DOWN) || this->m_pDI->JoystickDPadDown(DIR_DOWN) || (this->m_pDI->JoystickGetLStickYAmount() > 0.0f && this->m_fWaitTime > 0.3f))
	{
		this->m_OptionsFont.ChangeScrolling( 0, 100, this->m_nScrollingID );
	}
	else
	{
		this->m_OptionsFont.ChangeScrolling( 0, 0, this->m_nScrollingID );
	}

	if(m_pDI->GetInstance()->KeyDown(DIK_UP))
	{
		m_OptionsFont.ChangeScrolling( 0, -100, m_nScrollingID );
		m_bInput = true;
	}
	else if(m_pDI->GetInstance()->KeyDown(DIK_DOWN))
	{
		m_OptionsFont.ChangeScrolling( 0, 100, m_nScrollingID );
		m_bInput = true;
	}
	else
	{
		m_OptionsFont.ChangeScrolling( 0, 0, m_nScrollingID );
		m_bInput = false;
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAchievementsState::Update(float fElapsedTime)
{
	for(;;)
	{
		cout << "LOLOLOLOL";
	}
	m_OptionsFont.Update( fElapsedTime );

	AtractMode( fElapsedTime );

	this->m_fWaitTime += fElapsedTime;
	this->m_OptionsFont.Update( fElapsedTime );	

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAchievementsState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundID, 0, 0);

	this->m_OptionsFont.Draw("ACHIEVEMENTS", 225, 50, 0.6f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	this->m_OptionsFont.DrawScrolling("Achievement  1:  Incomplete\n\nAchievement  2:  Incomplete\n\nAchievement  3:  Incomplete", 
		150, 25+MENU_SPACE, 0.5f, D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f), 24+MENU_SPACE, 520, 149, 630, this->m_nScrollingID );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Exit”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAchievementsState::Exit(void)
{
	if(this->m_nBackgroundID != -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundID);
		this->m_nBackgroundID = -1;
	}

	if(this->m_pDS)
		this->m_pDS = NULL;

	if(this->m_pWM)
		this->m_pWM = NULL;

	if(this->m_pTM)
		this->m_pTM = NULL;

	if(this->m_pDI)
		this->m_pDI = NULL;
	
	if(this->m_pD3D)
		this->m_pD3D = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetInstance”
//////////////////////////////////////////////////////////////////////////////////////////////////////
CAchievementsState* CAchievementsState::GetInstance(void)
{
	if(!sm_pAchievementsInstance)
	{
		sm_pAchievementsInstance = new CAchievementsState();
	}
	return sm_pAchievementsInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “DeleteInstance”
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CAchievementsState::DeleteInstance(void)
{
	if(sm_pAchievementsInstance)
	{
		delete sm_pAchievementsInstance;
		sm_pAchievementsInstance = NULL;
	}
}