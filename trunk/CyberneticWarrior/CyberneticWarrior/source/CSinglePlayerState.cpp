#include "CSinglePlayerState.h"

#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectSound.h"

#include "CObjectManager.h"

#include "CGame.h"
#include "CCamera.h"

#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CPauseMenuState.h"
#include "COptionsMenuState.h"
#include "CIdleEnemy.h"
#include "CPatrolEnemy.h"
#include "CFLCLMech.h"
#include "CRocket.h"

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
}

CSinglePlayerState::~CSinglePlayerState(void)
{
	this->m_TempPlayer = NULL;
	this->m_TempPlatform1 = NULL;
	this->m_TempPlatform2 = NULL;
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
	this->m_TempMap = CMapLoad::GetInstance();

	m_TempMap->LoadMap("CW-Map_01.CWM");

	this->m_pD3D	=		CSGD_Direct3D::GetInstance();
	this->m_pDI		= 		CSGD_DirectInput::GetInstance();
	this->m_pTM		=		CSGD_TextureManager::GetInstance();
	this->m_pWM		=		CSGD_WaveManager::GetInstance();
	this->m_pDS		=		CSGD_DirectSound::GetInstance();
 
	this->m_pOF		=		CObjectFactory<string, CBase>::GetInstance();
	this->m_pOM		=		CObjectManager::GetInstance();

	this->m_pCamera	= CCamera::GetInstance();

	this->m_pCamera->ResetCam();


	m_nAnimation.LoadBinary("resource/binary/akainu.bae");

	
	this->m_pOF->RegisterClassType<CBase>("CBase");
	this->m_pOF->RegisterClassType<CGrapplingHook>("CHook");
	this->m_pOF->RegisterClassType<CPlayer>("CPlayer");
	this->m_pOF->RegisterClassType<CBlock>("CBlock");
	this->m_pOF->RegisterClassType<CPickUp>("CPickUp");
	this->m_pOF->RegisterClassType<CRocket>("CRocket");






	this->m_nBackgroundImageID = this->m_pTM->LoadTexture("resource/graphics/bgGame.png");
	this->m_nCrossHairID = this->m_pTM->LoadTexture("resource/graphics/CrossHairs.png");
	this->m_nBGMusic = this->m_pWM->LoadWave("resource/sounds/Jak2_Haven_City.wav");
	m_nRocketID = m_pTM->LoadTexture("resource/graphics/CrossHairs.png");

	this->m_TempPlayer = (CPlayer*)m_pOF->CreateObject("CPlayer");
	this->m_TempPlayer->SetImageID(this->m_pTM->LoadTexture("resource/graphics/Running1.bmp"));
	this->m_TempPlayer->SetPosX((float)242);
	this->m_TempPlayer->SetPosY((float)CGame::GetInstance()->GetScreenHeight() - 128);
	this->m_TempPlayer->SetCamX(this->m_TempPlayer->GetPosX());
	this->m_TempPlayer->SetCamY(this->m_TempPlayer->GetPosY());
	this->m_TempPlayer->SetWidth(64);
	this->m_TempPlayer->SetHeight(128);
	this->m_TempPlayer->SetBaseVelX(0);
	this->m_TempPlayer->SetBaseVelY(0);
	this->m_TempPlayer->SetSpeedX(0.0f);
	this->m_TempPlayer->SetSpeedY(0.0f);
	this->m_TempPlayer->SetOnGround(1);
	this->m_TempPlayer->SetMouseDown(0);

	//if(!this->GetNewGame())	//{	//	//this->m_Profile.m_bHaveHook = 1;	//}
	
	Enemy_1 = new CIdleEnemy(Idle, Turret_Gun, -1, 100, 90, 50, 100, .5, 50, 100, 400, 32, 32);
	Enemy_2 = new CPatrolEnemy(Patrol, 0, 250, Turret_Gun, -1, 100, 90, 50, 100, .5, 100, 300, 400, 32, 32);
	Enemy_3 = new CFLCLMech(true, 5, 0, Patrol, 0, 250, Ground_FLCL, -1, 100, 60, 50, 100, .5, 100, 50, 400, 32, 32);


	tVector2D vStartingPos;

	/*this->m_TempPlatform1 = (CBlock*)m_pOF->CreateObject("CBlock");
	this->m_TempPlatform1->SetImageID(this->m_pTM->LoadTexture("resource/graphics/tile.png"));
	this->m_TempPlatform1->SetPosX((float)0);
	this->m_TempPlatform1->SetPosY((float)370);
	vStartingPos.fX = this->m_TempPlatform1->GetPosX();
	vStartingPos.fY = this->m_TempPlatform1->GetPosY();
	this->m_TempPlatform1->SetWorldPos(vStartingPos);
	this->m_TempPlatform1->SetWidth(256);
	this->m_TempPlatform1->SetHeight(32);
	this->m_TempPlatform1->SetBlockType(BLOCK_SOLID);
	this->m_TempPlatform1->SetType(OBJ_BLOCK);*/




	this->m_TempPlatform2 = (CBlock*)m_pOF->CreateObject("CBlock");
	this->m_TempPlatform2->SetImageID(this->m_pTM->LoadTexture("resource/graphics/tile.png"));
	this->m_TempPlatform2->SetPosX((float)330);
	this->m_TempPlatform2->SetPosY((float)100);
	vStartingPos.fX = this->m_TempPlatform2->GetPosX();
	vStartingPos.fY = this->m_TempPlatform2->GetPosY();
	this->m_TempPlatform2->SetWorldPos(vStartingPos);
	this->m_TempPlatform2->SetWidth(256);
	this->m_TempPlatform2->SetHeight(32);
	this->m_TempPlatform2->SetBlockType(BLOCK_SOLID);
	this->m_TempPlatform2->SetType(OBJ_BLOCK);



	if(!this->m_Profile.m_bHaveHook)	{		
		this->m_PickUp = (CPickUp*)m_pOF->CreateObject("CPickUp");		
		this->m_PickUp->SetImageID(this->m_pTM->LoadTexture("resource/graphics/Grappling_Hook.png"));		
		this->m_PickUp->SetPosX((float)50);		
		this->m_PickUp->SetPosY((float)CGame::GetInstance()->GetScreenHeight() - 64);		
		//	this->m_PickUp->SetCamX(this->m_PickUp->GetPosX());		
		//	this->m_PickUp->SetCamY(this->m_PickUp->GetPosY());		
		vStartingPos.fX = this->m_PickUp->GetPosX();		
		vStartingPos.fY = this->m_PickUp->GetPosY();		
		this->m_PickUp->SetWorldPos(vStartingPos);		
		this->m_PickUp->SetWidth(64);		
		this->m_PickUp->SetHeight(64);		
		this->m_PickUp->SetBaseVelX(0);		
		this->m_PickUp->SetBaseVelY(0);		
		this->m_PickUp->SetPickUpType(GRAPPLING_HOOK);
		this->m_pOM->AddObject(this->m_PickUp);		
		this->m_PickUp->Release();
	}
	/*this->m_pOM->AddObject(m_TempPlatform1);
	this->m_TempPlatform1->Release();*/

	

	this->m_pOM->AddObject(this->m_TempPlayer);
	this->m_TempPlayer->Release();
	

	this->m_pOM->AddObject(this->m_TempPlatform2);
	this->m_TempPlatform2->Release();

	this->m_pWM->Play(this->m_nBGMusic, DSBPLAY_LOOPING);

}

bool CSinglePlayerState::Input(void)
{
	if(this->m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CStackStateMachine::GetInstance()->Push_Back(CPauseMenuState::GetInstance());
		//CStackStateMachine::GetInstance()->Pop_back();
	}
	return 1;
}

void CSinglePlayerState::Update(float fElapsedTime)
{
	m_nAnimation.Update( fElapsedTime );

	this->m_tBGOffset.fX = 0 - (float)CCamera::GetInstance()->GetCameraRect().left;

	if( this->m_nMusicVolume != COptionsMenuState::GetInstance()->GetMusicVolume())
	{
		this->m_nMusicVolume = COptionsMenuState::GetInstance()->GetMusicVolume();
		this->m_pWM->SetVolume(m_nBGMusic, this->m_nMusicVolume);
	}
	this->m_pOM->UpdateObjects(fElapsedTime);
	this->m_pOM->CheckCollisions();


	Enemy_1->Update(fElapsedTime);
	Enemy_2->Update(fElapsedTime);
	Enemy_3->Update(fElapsedTime);

}

void CSinglePlayerState::Render(void)
{
	this->m_pTM->Draw(this->m_nBackgroundImageID,(int)this->m_tBGOffset.fX,(int)this->m_tBGOffset.fY);

	m_nAnimation.Render( 400 - CCamera::GetInstance()->GetCameraRect().left, 450 - CCamera::GetInstance()->GetCameraRect().top);
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
	this->m_pTM->Draw(this->m_nCrossHairID, (this->m_pDI->MouseGetPosX()+8),
		(this->m_pDI->MouseGetPosY()+8), 1.0f, 1.0f, &rCrossHairs);
}

void CSinglePlayerState::Exit(void)
{
	delete Enemy_1;
	delete Enemy_2;
	delete Enemy_3;
	

	this->m_Profile.m_bHaveHook = 0;	if(this->m_pCamera)
	{
		this->m_pCamera->ResetCam();
		this->m_pCamera = NULL;
	}
	CObjectManager::GetInstance()->RemoveObject(this->m_PickUp);	this->m_pOF->UnregisterClassType("CPickUp");	this->m_pOF->UnregisterClassType("CBlock");	this->m_pOF->UnregisterClassType("CHook");	this->m_pOF->UnregisterClassType("CPlayer");	this->m_pOF->UnregisterClassType("CBase");

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