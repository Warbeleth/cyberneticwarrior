#include "PrecompiledHeader.h"

#include "CSinglePlayerState.h"
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


	this->m_tBGOffset.fX = 0;
	this->m_tBGOffset.fY = 0;

	this->m_TempPlayer = NULL;
	this->m_TempPlatform1 = NULL;
	this->m_TempPlatform2 = NULL;
	this->m_TempMap = NULL;

	this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume(); 
	this->m_nSFXVolume = COptionsMenuState::GetInstance()->GetMusicVolume(); 

	Enemy_1 = NULL;
	Enemy_2 = NULL;
	Enemy_3 = NULL;

	this->SetNewGame(1);

	this->m_Profile.m_bHaveHook = 0;

	this->SetType(GAMEPLAY);
	this->SetInputType(this->CKEYBOARD);
	this->m_bMusic = 1;
}

CSinglePlayerState::~CSinglePlayerState(void)
{
	this->m_nBackgroundImageID		= -1;
	this->m_nCrossHairID			= -1;
	this->m_nBGMusic				= -1;

	this->m_nSelectedWeaponID		= -1;
	this->m_nWeaponID					= -1;

	this->m_TempPlayer = NULL;
	this->m_TempPlatform1 = NULL;
	this->m_TempPlatform2 = NULL;
	this->SetInputType(this->CKEYBOARD);
	this->m_bMusic = 1;
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
	this->m_TempMap = CMapLoad::GetInstance();


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


	this->m_nBackgroundImageID = this->m_pTM->LoadTexture("resource/graphics/bgGame.png");
	this->m_nCrossHairID = this->m_pTM->LoadTexture("resource/graphics/CrossHairs.png");
	this->m_nBGMusic = this->m_pWM->LoadWave("resource/sounds/Jak2_Haven_City.wav");
	this->m_nSelectedWeaponID		= m_pTM->LoadTexture("resource/graphics/EquipmentWeaponIcons.png");
	this->m_nWeaponID = m_pTM->LoadTexture("resource/graphics/Weapons.png");

	this->m_TempPlayer = (CPlayer*)m_pOF->CreateObject("CPlayer");
	this->m_TempPlayer->SetImageID(this->m_pTM->LoadTexture("resource/graphics/Running1.bmp"));
	this->m_TempPlayer->SetPosX((float)0);
	this->m_TempPlayer->SetPosY((float)200);
	this->m_TempPlayer->SetWidth(64);
	this->m_TempPlayer->SetHeight(143);
	this->m_TempPlayer->SetBaseVelX(0);
	this->m_TempPlayer->SetBaseVelY(0);
	this->m_TempPlayer->SetSpeedX(0.0f);
	this->m_TempPlayer->SetSpeedY(0.0f);
	this->m_TempPlayer->SetPlayerNumber(0);

	//if(!this->GetNewGame())	//{	//	//this->m_Profile.m_bHaveHook = 1;	//}
	
	this->Enemy_1 = new CIdleEnemy(Idle, Turret_Gun, -1, 100, 90, 50, 100, .5, 50, 100, 400, 32, 32);
	this->Enemy_2 = new CPatrolEnemy(Patrol, 0, 250, Turret_Gun, -1, 100, 90, 50, 100, .5, 100, 300, 400, 32, 32);
	this->Enemy_3 = new CFLCLMech(-1, 300, 400);


	tVector2D vStartingPos;




	if(!this->m_Profile.m_bHaveHook)	{		
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

	

	this->m_pOM->AddObject(this->m_TempPlayer);
	this->m_TempPlayer->Release();

	m_TempMap->LoadMap("CW-Map_01.CWM");


	CLoadingState::GetInstance()->SetReady(1);

}

bool CSinglePlayerState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_ESCAPE) || this->m_pDI->JoystickButtonPressed(9))
	{
		this->GetPlayerPointer()->SetShutDown(true);
		CStackStateMachine::GetInstance()->UpdateState(0.0f);
		CStackStateMachine::GetInstance()->Push_Back(CPauseMenuState::GetInstance());
		//CStackStateMachine::GetInstance()->Pop_back();
	}

	if(this->m_pDI->KeyPressed(DIK_SPACE))
		m_TempPlayer->DecrementEnergy(40);

	if(this->m_pDI->MouseButtonPressed(MOUSE_RIGHT))
		m_TempPlayer->DecrementHealth(20);

	return 1;
}

void CSinglePlayerState::Update(float fElapsedTime)
{
	if(this->m_bMusic)
	{
		//this->m_pWM->Play(this->m_nBGMusic, DSBPLAY_LOOPING);
		this->m_bMusic = 0;
	}

	this->m_tBGOffset.fX = 0;

	if( this->m_nMusicVolume != COptionsMenuState::GetInstance()->GetMusicVolume())
	{
		this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume();
		this->m_pWM->SetVolume(m_nBGMusic, this->m_nMusicVolume);
	}
	this->m_pOM->CheckCollisions();
	this->m_pOM->UpdateObjects(fElapsedTime);


	Enemy_1->Update(fElapsedTime);
	Enemy_2->Update(fElapsedTime);
	Enemy_3->Update(fElapsedTime);

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


}

void CSinglePlayerState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundImageID,
		(int)((this->m_tBGOffset.fX - CCamera::GetInstance()->GetOffsetX()) * CCamera::GetInstance()->GetScale()),
		(int)((this->m_tBGOffset.fY - CCamera::GetInstance()->GetOffsetY()) * CCamera::GetInstance()->GetScale()), 
		1.0f*CCamera::GetInstance()->GetScale(),
		1.0f*CCamera::GetInstance()->GetScale());
	
	//////////////////////////////
	// TEMP
	//////////////////////////////
	/*this->m_pTM->Draw(this->m_tBasicPlatform.m_nPlatformID,
				(int)this->m_tBasicPlatform.m_tTempPlatformPoint.fX,
				(int)this->m_tBasicPlatform.m_tTempPlatformPoint.fY);*/

	//this->m_TempPlayer.Render();
	//////////////////////////////

	m_pD3D->GetSprite()->Flush();

 	m_TempMap->Render();

	m_pD3D->GetSprite()->Flush();

	Enemy_1->Render();
	Enemy_2->Render();
	Enemy_3->Render();

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
	this->m_bMusic = 1;

	if(Enemy_1)
	{
		delete Enemy_1;
	}
	if(Enemy_1)
	{
		delete Enemy_2;
	}
	if(Enemy_1)
	{
		delete Enemy_3;	
	}
	this->m_Profile.m_bHaveHook = 0;	

	CObjectManager::GetInstance()->RemoveObject(this->m_PickUp);	
	this->m_pOF->UnregisterClassType ("CPickUp");	
	this->m_pOF->UnregisterClassType("CBlock");	
	this->m_pOF->UnregisterClassType("CHook");	
	this->m_pOF->UnregisterClassType("CPlayer");	
	this->m_pOF->UnregisterClassType("CBase");

	if(this->m_pOF)
	{
		//this->m_pOF->
		this->m_pOF = NULL;
	}

	this->m_pOM->RemoveAllObjects();
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
}




CPlayer*	CSinglePlayerState::GetPlayerPointer(void)	{return this->m_TempPlayer;}

CSinglePlayerState::tProfile*	CSinglePlayerState::GetProfileValues(void){	return &this->m_Profile;}void		CSinglePlayerState::SetProfileValues(bool bValue){	this->m_Profile.m_bHaveHook = bValue;}