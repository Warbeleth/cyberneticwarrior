#include "PrecompiledHeader.h"

#include "CSinglePlayerState.h"
#include "CGameOverState.h"
#include "CObjectManager.h"
#include "CGame.h"
#include "CCamera.h"
#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CPauseMenuState.h"
#include "CLoadingState.h"
#include "COptionsMenuState.h"
#include "CMapLoad.h"
#include "CBlock.h"
#include "CPickup.h"
#include "CIdleEnemy.h"
#include "CPatrolEnemy.h"
#include "CFLCLMech.h"
#include "CRocket.h"
#include "CGrapplingHook.h"
#include "CBullet.h"
#include "CRocket.h"
#include "CFlame.h"
#include "CPlasma.h"
#include "CShock.h"
#include "CGrenade.h"
#include "CFire.h"
#include "CIce.h"

CSinglePlayerState*	CSinglePlayerState::sm_pGamePlayInstance = NULL;

CSinglePlayerState::CSinglePlayerState(void)
{
	this->m_pD3D	= NULL;
	this->m_pTM		= NULL;
	this->m_pDI		= NULL;
	this->m_pWM		= NULL;
	this->m_pDS		= NULL;

	this->m_pCamera	= NULL;

	this->m_nBackgroundImageID		= -1;
	this->m_nCrossHairID			= -1;
	this->m_nBGMusic				= -1;

	this->m_nSelectedWeaponID		= -1;
	this->m_nWeaponID				= -1;

	this->m_bJamming = false;


	this->m_tBGOffset.fX = 0;
	this->m_tBGOffset.fY = 0;

	this->m_TempPlayer = NULL;
	this->m_TempPlatform1 = NULL;
	this->m_TempPlatform2 = NULL;
	this->m_TempMap = NULL;

	this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume(); 
	this->m_nSFXVolume = COptionsMenuState::GetInstance()->GetMusicVolume(); 

	//Enemy_1 = NULL;
	//Enemy_2 = NULL;
	//Enemy_3 = NULL;
	this->m_nCurrentLevel = TUTORIAL;
	this->m_nPreviousLevel = -1;
	this->SetDeath(false);

	this->SetNewGame(1);

	this->m_Profile.m_bHaveHook = true;

	this->SetType(GAMEPLAY);
	this->SetInputType(this->CKEYBOARD);
	if(COptionsMenuState::GetInstance()->GetMute())
	{
		this->m_bMusic = 1;
	}
	else
	{
		this->m_bMusic = 0;
	}
	this->m_bLevelChange = false;
}

CSinglePlayerState::~CSinglePlayerState(void)
{
	this->m_nBackgroundImageID		= -1;
	this->m_nCrossHairID			= -1;
	this->m_nBGMusic				= -1;
	this->m_bLevelChange = false;
	this->SetDeath(false);

	this->m_nSelectedWeaponID		= -1;
	this->m_nWeaponID					= -1;

	this->m_nCurrentLevel = TUTORIAL;
	this->m_nPreviousLevel = -1;

	this->m_TempPlayer = NULL;
	this->m_TempPlatform1 = NULL;
	this->m_TempPlatform2 = NULL;
	this->SetInputType(this->CKEYBOARD);
	if(COptionsMenuState::GetInstance()->GetMute())
	{
		this->m_bMusic = 1;
	}
	else
	{
		this->m_bMusic = 0;
	}
}

CSinglePlayerState* CSinglePlayerState::GetInstance(void)
{
	if(!sm_pGamePlayInstance)
	{
		sm_pGamePlayInstance = new CSinglePlayerState();
	}
	return sm_pGamePlayInstance;
}

void CSinglePlayerState::DeleteInstance(void)
{
	if(sm_pGamePlayInstance)
	{
		delete sm_pGamePlayInstance;
		sm_pGamePlayInstance = NULL;
	}
}

void CSinglePlayerState::Enter(void)
{
	// File Include Needs conversion - Corey
	//CStackStateMachine::GetInstance()->Push_Back(CLoadingState::GetInstance());
	this->m_bLevelChange = false;
	this->m_TempMap = CMapLoad::GetInstance();
	this->m_TempMap->LoadAnimations();	
	//	m_TempMap->LoadMap("test.CWM");
	
	switch(this->m_nCurrentLevel)
	{
	case TUTORIAL:
		//m_TempMap->LoadMap("swingaway.CWM"); // leave here to test grappling hook
		m_TempMap->LoadMap("Tutorial_v1.5.CWM");
		break;
	case LEVEL1:
		m_TempMap->LoadMap("CW-Map_02.CWM");
		break;
	case LEVEL2:
		m_TempMap->LoadMap("Level-2.CWM");
		break;
	};
	
	//if(SetCurrentLevel == TUTORIAL)
	//	m_TempMap->LoadMap("Tutorial_v1.5.CWM");


	

	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		= 		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();
	this->m_pOF		=		CObjectFactory<string, CBase>::GetInstance();
	this->m_pOM		=		CObjectManager::GetInstance();

	this->m_pOF->RegisterClassType<CBase>("CBase");
	this->m_pOF->RegisterClassType<CPlayer>("CPlayer");
	this->m_pOF->RegisterClassType<CBlock>("CBlock");
	this->m_pOF->RegisterClassType<CPickUp>("CPickUp");
	this->m_pOF->RegisterClassType<CGrapplingHook>("CHook");
	this->m_pOF->RegisterClassType<CBullet>("CBullet");
	this->m_pOF->RegisterClassType<CRocket>("CRocket");
	this->m_pOF->RegisterClassType<CFlame>("CFlame");
	this->m_pOF->RegisterClassType<CPlasma>("CPlasma");
	this->m_pOF->RegisterClassType<CShock>("CShock");
	this->m_pOF->RegisterClassType<CGrenade>("CGrenade");
	this->m_pOF->RegisterClassType<CFire>("CFire");
	this->m_pOF->RegisterClassType<CIce>("CIce");


	this->m_nBackgroundImageID = this->m_pTM->LoadTexture("resource/graphics/Level1.png");
	this->m_nCrossHairID = this->m_pTM->LoadTexture("resource/graphics/CrossHairs.png");
	this->m_nBGMusic = this->m_pWM->LoadWave("resource/sounds/JTM_LevelMusic.wav");
	this->m_nSelectedWeaponID		= m_pTM->LoadTexture("resource/graphics/EquipmentWeaponIcons.png");
	this->m_nWeaponID = m_pTM->LoadTexture("resource/graphics/Weapons.png");
	this->m_nHookID = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/hook.png");

	this->m_TempPlayer = (CPlayer*)m_pOF->CreateObject("CPlayer");
	this->m_TempPlayer->SetImageID(this->m_pTM->LoadTexture("resource/graphics/Running1.bmp"));

	if(this->m_nCurrentLevel == TUTORIAL && this->m_nPreviousLevel == -1)
	{
		this->m_TempPlayer->SetPosX((float)200);
		this->m_TempPlayer->SetPosY((float)90);
	}
	else if(this->m_nCurrentLevel == LEVEL1 && this->m_nPreviousLevel == TUTORIAL)
	{
		this->m_TempPlayer->SetPosX((float)42);
		this->m_TempPlayer->SetPosY((float)700);
	}
	else if(this->m_nCurrentLevel == TUTORIAL && this->m_nPreviousLevel == LEVEL1)
	{
		this->m_TempPlayer->SetPosX((float)2300);
		this->m_TempPlayer->SetPosY((float)1024);
	}
	else if(this->m_nCurrentLevel == LEVEL2 && this->m_nPreviousLevel == TUTORIAL)
	{
		this->m_TempPlayer->SetPosX((float)400);
		this->m_TempPlayer->SetPosY((float)0);
	}


	this->m_TempPlayer->SetWidth(64);
	this->m_TempPlayer->SetHeight(128);
	this->m_TempPlayer->SetBaseVelX(0);
	this->m_TempPlayer->SetBaseVelY(0);
	this->m_TempPlayer->SetSpeedX(0.0f);
	this->m_TempPlayer->SetSpeedY(0.0f);
	this->m_TempPlayer->SetPlayerNumber(0);

	//if(!this->GetNewGame())	//{	//	//this->m_Profile.m_bHaveHook = 1;	//}
	
	//this->Enemy_1 = new CIdleEnemy(Idle, Turret_Gun, -1, 100, 90, 50, 100, .5, 50, 100, 400, 32, 32);
	//this->Enemy_2 = new CPatrolEnemy(Patrol, 0, 250, Turret_Gun, -1, 100, 90, 50, 100, .5, 100, 300, 400, 32, 32);
	//this->Enemy_3 = new CFLCLMech(-1, 300, 400);


	this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume();
		this->m_pWM->SetVolume(m_nBGMusic, this->m_nMusicVolume);




	tVector2D vStartingPos;


	if(!this->m_Profile.m_bHaveHook)	
	{		
		this->m_PickUp = (CPickUp*)m_pOF->CreateObject("CPickUp");		
		this->m_PickUp->SetImageID(this->m_pTM->LoadTexture("resource/graphics/Grappling_Hook.png"));		
		this->m_PickUp->SetPosX((float)50);		
		this->m_PickUp->SetPosY((float)CGame::GetInstance()->GetScreenHeight() - 64);		
		//	this->m_PickUp->SetCamX(this->m_PickUp->GetPosX());		
		//	this->m_PickUp->SetCamY(this->m_PickUp->GetPosY());		
		vStartingPos.fX = this->m_PickUp->GetPosX();		
		vStartingPos.fY = this->m_PickUp->GetPosY();	
		this->m_PickUp->SetWidth(64);		
		this->m_PickUp->SetHeight(64);		
		this->m_PickUp->SetBaseVelX(0);		
		this->m_PickUp->SetBaseVelY(0);		
		this->m_PickUp->SetPickUpType(GRAPPLING_HOOK);
		this->m_pOM->AddObject(this->m_PickUp);		
		this->m_PickUp->Release();
	}

	//m_TempMap->LoadMap("Tutorial_v1.5.CWM");
	

	this->m_pOM->AddObject(this->m_TempPlayer);
	//this->m_pOM->AddObject(this->Enemy_1);
	//this->m_pOM->AddObject(this->Enemy_2);
	//this->m_pOM->AddObject(this->Enemy_3);
	
	//this->Enemy_1->Release();
	//this->Enemy_2->Release();
	//this->Enemy_3->Release();

	this->m_TempPlayer->Release();






	CLoadingState::GetInstance()->SetReady(1);

}

bool CSinglePlayerState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(9))
	{
		if(!COptionsMenuState::GetInstance()->GetMute())
		{
			this->m_pWM->Stop(this->m_nBGMusic);
		}
		this->GetPlayerPointer()->SetShutDown(true);
		CStackStateMachine::GetInstance()->UpdateState(0.0f);
		CStackStateMachine::GetInstance()->Push_Back(CPauseMenuState::GetInstance());
		//CStackStateMachine::GetInstance()->Pop_back();
	}
	


	return 1;
}

void CSinglePlayerState::Update(float fElapsedTime)
{	
	if(this->m_bMusic && !COptionsMenuState::GetInstance()->GetMute())
	{
		this->m_pWM->Play(this->m_nBGMusic, DSBPLAY_LOOPING);
		this->m_bMusic = false;
	}

	if(this->m_bJamming && !COptionsMenuState::GetInstance()->GetMute())
	{
		this->m_pWM->Play(this->m_nBGMusic, DSBPLAY_LOOPING);
		this->m_bJamming = false;
	}

	this->m_tBGOffset.fX = 0;

	if( this->m_nMusicVolume != COptionsMenuState::GetInstance()->GetMusicVolume())
	{
		this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume();
		this->m_pWM->SetVolume(m_nBGMusic, this->m_nMusicVolume);
	}
	this->m_pOM->UpdateObjects(fElapsedTime);
	this->m_pOM->CheckCollisions();


	//Enemy_1->Update(fElapsedTime);	
	//Enemy_2->Update(fElapsedTime);	
	//Enemy_3->Update(fElapsedTime);
	if(this->m_pDI->MouseGetPosX() < 0)
	{
		this->m_pDI->MouseSetPosX(0);
	}
	if(this->m_pDI->MouseGetPosX() > CGame::GetInstance()->GetScreenWidth())
	{
		this->m_pDI->MouseSetPosX(CGame::GetInstance()->GetScreenWidth()-32);
	}
	if(this->m_pDI->MouseGetPosY() < 0)
	{
		this->m_pDI->MouseSetPosY(0);
	}
	if(this->m_pDI->MouseGetPosY() > CGame::GetInstance()->GetScreenHeight())
	{
		this->m_pDI->MouseSetPosY(CGame::GetInstance()->GetScreenHeight() - 32);
	}

	if(this->m_bLevelChange)
	{
		CLoadingState::GetInstance()->SetLoad(false);
		CStackStateMachine::GetInstance()->ChangeState(CLoadingState::GetInstance());
		//CStackStateMachine::GetInstance()->RemoveState(GAMEPLAY);
	}

	if(this->GetDeath())
	{
		/*this->GetPlayerPointer()->SetShutDown(true);
		if(this->m_nBGMusic)

		{
			this->m_pWM->Stop(this->m_nBGMusic);
		}
		CStackStateMachine::GetInstance()->UpdateState(0.0f);*/
		CStackStateMachine::GetInstance()->ChangeState(CGameOverState::GetInstance());
		this->SetDeath(false);
	}

}

void CSinglePlayerState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundImageID,
		(int)((this->m_tBGOffset.fX - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()),
		(int)((this->m_tBGOffset.fY - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f*CCamera::GetInstance()->GetScale(),
		1.0f*CCamera::GetInstance()->GetScale());
	
	



	m_pD3D->GetSprite()->Flush();

 	m_TempMap->Render();

	m_pD3D->GetSprite()->Flush();

	//Enemy_1->Render();	
	//Enemy_2->Render();	
	//Enemy_3->Render();
	m_pOM->RenderObjects();




	RECT	rCrossHairs;
	rCrossHairs.top		= 0;
	rCrossHairs.left	= 0;
	rCrossHairs.right	= 16;
	rCrossHairs.bottom	= 16;

	//this->m_pTM->Draw(this->m_nCrossHairID, ((int)(this->m_TempPlayer->GetJoyPos()->fX - (int)this->m_TempPlayer->GetPosX())+8),
		//(((int)this->m_TempPlayer->GetJoyPos()->fY - (int)this->m_TempPlayer->GetPosX())+8), 1.0f, 1.0f, &rCrossHairs);
	if(!this->GetInputType())
	{
		this->m_pTM->Draw(this->m_nCrossHairID, this->m_pDI->MouseGetPosX() + 8, this->m_pDI->MouseGetPosY() + 8, 1.0f * CCamera::GetInstance()->GetScale(), 1.0f * CCamera::GetInstance()->GetScale(), &rCrossHairs);
	}
	else if(this->GetInputType())
	{
		this->m_pTM->Draw(this->m_nCrossHairID, 
			int((((int)(this->m_TempPlayer->GetJoyPos()->fX - (int)this->m_TempPlayer->GetPosX())+8)) * CCamera::GetInstance()->GetScale()),
			int((((int)this->m_TempPlayer->GetJoyPos()->fY - (int)this->m_TempPlayer->GetPosX())+8) * CCamera::GetInstance()->GetScale()), 
			1.0f * CCamera::GetInstance()->GetScale(), 1.0f * CCamera::GetInstance()->GetScale(), &rCrossHairs);
	}
}

void CSinglePlayerState::Exit(void)
{
	this->SetDeath(false);
	
	this->m_bMusic = 1;

	//if(Enemy_1)	
	//{	//	delete Enemy_1;	//}	
	//if(Enemy_1)	
	//{	//	delete Enemy_2;	//}
	//if(Enemy_1)	//{
	//	delete Enemy_3;		//}
	
	this->m_Profile.m_bHaveHook = 0;	





	CObjectManager::GetInstance()->RemoveObject(this->m_PickUp);	
	this->m_pOF->UnregisterClassType ("CIce");	
	this->m_pOF->UnregisterClassType("CFire");	
	this->m_pOF->UnregisterClassType("CGrenade");	
	this->m_pOF->UnregisterClassType("CShock");	
	this->m_pOF->UnregisterClassType("CPlasma");
	this->m_pOF->UnregisterClassType("CFlame");
	this->m_pOF->UnregisterClassType("CRocket");
	this->m_pOF->UnregisterClassType("CBullet");
	this->m_pOF->UnregisterClassType("CHook");
	this->m_pOF->UnregisterClassType("CPickUp");
	this->m_pOF->UnregisterClassType("CBlock");
	this->m_pOF->UnregisterClassType("CBase");
	this->m_pOF->UnregisterClassType("CPlayer");

	if(this->m_pOF)
	{
		//this->m_pOF->
		this->m_pOF = NULL;
	}

	this->m_pOM->RemoveAllObjects();
	this->m_TempMap = NULL;
	if(this->m_pOM)
	{
		this->m_pOM = NULL;
	}

	if(this->m_nBackgroundImageID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nBackgroundImageID);
		this->m_pTM->UnloadTexture(this->m_nCrossHairID);
	//	this->m_pTM->UnloadTexture(this->m_TempPlayer->GetImageID());
	//	this->m_pTM->UnloadTexture(this->m_TempPlatform1->GetImageID());
	//	this->m_pTM->UnloadTexture(this->m_TempPlatform2->GetImageID());
	}

	
	if(this->m_nWeaponID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nWeaponID);
		this->m_nWeaponID = -1;
	}

	if(this->m_nSelectedWeaponID > -1)
	{
		this->m_pTM->UnloadTexture(this->m_nSelectedWeaponID);
		this->m_nSelectedWeaponID = -1;
	}

	if(this->m_nBGMusic > -1)
	{
		this->m_pWM->Stop(this->m_nBGMusic);
		this->m_pWM->UnloadWave(this->m_nBGMusic);
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

		//this->m_nCurrentLevel = TUTORIAL;
	//this->m_nPreviousLevel = -1;
}




CPlayer*	CSinglePlayerState::GetPlayerPointer(void)	{return this->m_TempPlayer;}

CSinglePlayerState::tProfile*	CSinglePlayerState::GetProfileValues(void){	return &this->m_Profile;}void		CSinglePlayerState::SetProfileValues(bool bValue){	this->m_Profile.m_bHaveHook = bValue;}