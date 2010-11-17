//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : CGame.cpp
//
//	Author : Patrick Alvarez 
//
//	Purpose : Main Game class definitions which
//			  will be used to continually handle and run the game build.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CGame.h"
#include "CCodeProfiler.h"
#include "CCamera.h"

#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CSinglePlayerMenuState.h"
#include "CControlSelectState.h"
#include "CPauseMenuState.h"
#include "COptionsMenuState.h"
#include "CCreditsState.h"
#include "CAtractModeState.h"
#include "CLoadingState.h"
#include "CGameOverState.h"
#include "CHowToPlayState.h"

#include "CMapLoad.h"
#include "CGameProfiler.h"
#include "CEventSystem.h"
#include "CObjectFactory.h"
#include "CObjectManager.h"

#include "CBase.h"
#include "CPlayer.h"
#include "CGrapplingHook.h"
#include "CBullet.h"
#include "CRocket.h"
#include "CFlame.h"
#include "CPlasma.h"
#include "CShock.h"
#include "CGrenade.h"
#include "CFire.h"
#include "CIce.h"

//Enemy includes
#include "CBaseEnemy.h"
#include "CIdleEnemy.h"
#include "CPatrolEnemy.h"

//Ground
#include "CFLCLMech.h"
#include "CMech.h"
#include "CSiegeWalker.h"
//Turrets
#include "CTurretCore.h"
//Air
#include "CAttackDrone.h"
#include "CHeavyAttackDrone.h"
#include "CSeekerDrone.h"
//Bosses
#include "CAppleMech.h"
#include "CDeathPirate.h"
#include "CPimpStriker.h"





// Singleton Instantiation
CGame*	CGame::sm_pGameInstance = NULL;

////////////////////////////////////////////////////////////////////////////////////
//	Function : CGame (Constructor)
////////////////////////////////////////////////////////////////////////////////////
CGame::CGame(void)
{
	this->m_pD3D = NULL;
	this->m_pTM  = NULL;
	this->m_pDS  = NULL;
	this->m_pWM  = NULL;
	this->m_pDI  = NULL;

	this->m_pSSM = NULL;

	this->m_pES = NULL;
	this->m_pMS = NULL;


	this->m_nScreenWidth  = 800;
	this->m_nScreenHeight = 600;



	this->m_fScaleX = 1.75f;
	this->m_fScaleY = 1.75f;


	this->m_dwTimeStamp				= GetTickCount();
	this->m_dwPreviousTimeStamp		= 0;
	this->m_fElapsedTime			= 0.0f;
	this->m_fGameTime				= 0.0f;

	m_cPlayerOne[0] = DIK_SPACE;
	m_cPlayerOne[1] = DIK_A;
	m_cPlayerOne[2] = DIK_D;
	m_cPlayerOne[3] = MOUSE_LEFT;
	m_cPlayerOne[4] = MOUSE_RIGHT;
	m_cPlayerOne[5] = -1;
	m_cPlayerOne[6] = DIK_TAB;
	m_cPlayerOne[7] = DIK_LSHIFT;
	m_cPlayerOne[8] = DIK_SPACE;
	m_cPlayerOne[9] = DIK_W;
	m_cPlayerOne[10] = DIK_S;
	// 0 = jump
	// 1 = left
	// 2 = right
	// 3 = fire weapon
	// 4 = secondary fire
	// 5 = aim weapon
	// 6 = swap weapon
	// 7 = head slot ability
	// 8 = boot slot ability
	// 9 = climb rope
	// 10 = repel rope

}

////////////////////////////////////////////////////////////////////////////////////
//	Function : ~CGame (Destructor)
////////////////////////////////////////////////////////////////////////////////////
CGame::~CGame(void)
{
	this->m_pD3D = NULL;
	this->m_pTM  = NULL;
	this->m_pDS  = NULL;
	this->m_pWM  = NULL;
	this->m_pDI  = NULL;

	this->m_pES = NULL;
	this->m_pMS = NULL;

	this->m_pSSM = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
//	Function : GetInstance
////////////////////////////////////////////////////////////////////////////////////
CGame* CGame::GetInstance(void)
{
	if(sm_pGameInstance == NULL)
	{
		sm_pGameInstance = new CGame();
	}
	return sm_pGameInstance;
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : DeleteInstance
////////////////////////////////////////////////////////////////////////////////////
void CGame::DeleteInstance(void)
{
	if(sm_pGameInstance != NULL)
	{
		delete sm_pGameInstance;
		sm_pGameInstance = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Initialize
////////////////////////////////////////////////////////////////////////////////////
void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, 
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	this->m_pD3D		= CSGD_Direct3D::GetInstance();
	this->m_pTM			= CSGD_TextureManager::GetInstance();
	this->m_pDS			= CSGD_DirectSound::GetInstance();
	this->m_pWM			= CSGD_WaveManager::GetInstance();
	this->m_pDI			= CSGD_DirectInput::GetInstance();

	this->m_pSSM		= CStackStateMachine::GetInstance();

	this->m_pES			= CEventSystem::GetInstance();
	this->m_pMS			= CMessageSystem::GetInstance();

	this->m_pCP			= CCodeProfiler::GetInstance();

	this->m_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, false);
	this->m_pTM->InitTextureManager(this->m_pD3D->GetDirect3DDevice(), this->m_pD3D->GetSprite());
	this->m_pDS->InitDirectSound(hWnd);
	this->m_pWM->InitWaveManager(hWnd, this->m_pDS->GetDSObject());
	this->m_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD | DI_MOUSE | DI_JOYSTICKS, DI_MOUSE);

	this->m_pMS->InitMessageSystem( CGame::MessageProc );

	this->m_nScreenWidth = nScreenWidth;
	this->m_nScreenHeight = nScreenHeight;


	this->m_pSSM->ChangeState(CMainMenuState::GetInstance());

}

////////////////////////////////////////////////////////////////////////////////////
//	Function : ShutDown
////////////////////////////////////////////////////////////////////////////////////
void CGame::ShutDown(void)
{

	this->m_pCP->SavePerformance();


	this->m_pMS->ShutdownSystem();
	this->m_pMS->DeleteInstance();
	this->m_pMS = NULL;

	this->m_pES->ShutdownEventSystem();
	this->m_pES->DeleteInstance();
	this->m_pES = NULL;


	this->m_pSSM->ChangeState(NULL);
	this->m_pSSM->DeleteInstance();
	this->m_pSSM = NULL;

	if(CCamera::GetInstance())
	{
		CCamera::GetInstance()->DeleteInstance();
	}

	

	if(CObjectFactory<std::string, CBase>::GetInstance())
	{
		CObjectFactory<std::string, CBase>::GetInstance()->ShutdownObjectFactory();
	}

	if(CObjectManager::GetInstance())
	{
		CObjectManager::GetInstance()->RemoveAllObjects();
		CObjectManager::GetInstance()->DeleteInstance();
	}

	CMainMenuState::GetInstance()->DeleteInstance();
	CSinglePlayerState::GetInstance()->DeleteInstance();
	CGameProfiler::GetInstance()->DeleteInstance();
	CSinglePlayerMenuState::GetInstance()->DeleteInstance();
	CPauseMenuState::GetInstance()->DeleteInstance();
	COptionsMenuState::GetInstance()->DeleteInstance();
	CCodeProfiler::GetInstance()->DeleteInstance();
	CControlSelectState::GetInstance()->DeleteInstance();
	CCreditsState::GetInstance()->DeleteInstance();
	CAtractModeState::GetInstance()->DeleteInstance();
	CLoadingState::GetInstance()->DeleteInstance();
	CGameOverState::GetInstance()->DeleteInstance();
	CHowToPlayState::GetInstance()->DeleteInstance();

	if(this->m_pDI)
	{
		this->m_pDI->ShutdownDirectInput();
		this->m_pDI = NULL;
	}

	if(this->m_pWM)
	{
		this->m_pWM->ShutdownWaveManager();
		this->m_pWM = NULL;
	}

	if(this->m_pDS)
	{
		this->m_pDS->ShutdownDirectSound();
		this->m_pDS = NULL;
	}

	if(this->m_pTM)
	{
		this->m_pTM->ShutdownTextureManager();
		this->m_pTM = NULL;
	}

	if(this->m_pD3D)
	{
		this->m_pD3D->ShutdownDirect3D();
		this->m_pD3D = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Main
////////////////////////////////////////////////////////////////////////////////////
bool CGame::Main(void)
{

	DWORD dwStartTimeStamp = GetTickCount();

	if(this->m_dwPreviousTimeStamp  != 0)
	{
		this->m_fElapsedTime = (float)(dwStartTimeStamp - this->m_dwPreviousTimeStamp) / 1000.0f;
	}
	this->m_dwPreviousTimeStamp = dwStartTimeStamp;

#ifdef _DEBUG
	if(this->m_fElapsedTime > 0.15f)
	{
		this->m_fElapsedTime = 0.15f;
	}
#endif

	if(this->Input() == false)
	{
		return false;
	}

	this->Update(this->m_fElapsedTime);

	this->Draw();

	return true;
}


////////////////////////////////////////////////////////////////////////////////////
//	Function : Input
////////////////////////////////////////////////////////////////////////////////////
bool CGame::Input(void)
{
	this->m_pDI->ReadDevices();

	///////////////////////////////////
	// Temp
	///////////////////////////////////
	/*if(m_pDI->KeyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}*/
	//////////////////////////////////


	return this->m_pSSM->Input();
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Update
////////////////////////////////////////////////////////////////////////////////////
void CGame::Update(float fElapsedTime)
{
	this->m_pWM->Update();
	this->m_pSSM->UpdateState(fElapsedTime);
	//this->m_pES->ProcessEvents();
	this->m_pMS->ProcessMessages();
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Draw
////////////////////////////////////////////////////////////////////////////////////
void CGame::Draw(void)
{
	this->m_pD3D->Clear(96, 175, 120);
	this->m_pD3D->DeviceBegin();
	this->m_pD3D->SpriteBegin();


	this->m_pSSM->RenderState();


	this->m_pD3D->SpriteEnd();
	this->m_pD3D->DeviceEnd();

	this->m_pD3D->Present();
}

void CGame::MessageProc(CBaseMessage*	pMsg)
{
	switch(pMsg->GetMsgID())
	{
	case MSG_CREATE_HOOK:
		{
			CCreateHookMessage* pGH = (CCreateHookMessage*)pMsg;
			CGrapplingHook*	pHook;

			float fHookVelocity = 700.0f;

			pHook = (CGrapplingHook*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CHook");
			pHook->SetWidth(32);
			pHook->SetHeight(32);
			pHook->SetPosX(pGH->GetPlayerPointer()->GetPosX() + (float)pGH->GetPlayerPointer()->GetWidth());
			pHook->SetPosY(pGH->GetPlayerPointer()->GetPosY());// - pGH->GetPlayerPointer()->GetHeight()/2);
			pHook->SetRotation(pGH->GetPlayerPointer()->GetRotation());

			pHook->SetHooked(0);
			/*tVector2D vWorldPos;
			vWorldPos.fX = pHook->GetPosX();
			vWorldPos.fY = pHook->GetPosY();
			pHook->SetWorldPos(vWorldPos);*/

			

			static tVector2D vPlayerPos;
			vPlayerPos.fX = pGH->GetPlayerPointer()->GetPosX() + (float)pGH->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pGH->GetPlayerPointer()->GetPosY();//) - (float)pGH->GetPlayerPointer()->GetHeight()/2;

			
			static tVector2D vMousePos;
			if(CSinglePlayerState::GetInstance()->GetInputType())
			{
				vMousePos.fX = vPlayerPos.fX + (float)pGH->GetPlayerPointer()->GetJoyPos()->fX;
				vMousePos.fY = vPlayerPos.fY +(float)pGH->GetPlayerPointer()->GetJoyPos()->fY;
			}
			else if(!CSinglePlayerState::GetInstance()->GetInputType())
			{
				vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
				vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();
			}


			static tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pHook->SetBaseVelX(vShot.fX * fHookVelocity);
			pHook->SetBaseVelY(vShot.fY * fHookVelocity);

			

			pHook->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/hook.png"));

			CObjectManager::GetInstance()->AddObject(pHook);
			pHook->Release();

			break;
		}
	case MSG_DESTROY_HOOK:
		{
			CDestroyHookMessage* pDH = (CDestroyHookMessage*)pMsg;

   			pDH->GetPlayerPointer()->SetHookPointer(NULL);
		//	CSGD_TextureManager::GetInstance()->UnloadTexture(pDH->GetHookPointer()->GetImageID());
			CObjectManager::GetInstance()->RemoveObject(pDH->GetHookPointer());

			pDH = NULL;
		}
		break;
	case MSG_CREATE_BULLET:
		{
			CCreateBulletMessage* pCR = (CCreateBulletMessage*)pMsg;
			float fRocketVelocity = 300;

			// Create the bullet, set the owner and set the dimensions
			CBullet* pBullet = (CBullet*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CBullet");
			pBullet->SetOwner(pCR->GetOwnerPointer());	
			pBullet->SetWidth(9);
			pBullet->SetHeight(6);
			
			// Set the firing position
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();	
			pBullet->SetPosX((float)ptStartingPos.m_nX);
			pBullet->SetPosY((float)ptStartingPos.m_nY);	

			// Set firing position vector
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}

			// Set the target position vector
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() + CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Get the firing direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;
		
			// Default vector
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// Set the rotation
			if(pBullet->GetOwner()->GetType() == OBJ_PLAYER)
			{
				pBullet->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate( vecRocketRotation, CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() );
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vShot ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pBullet->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pBullet->SetRotation(fAngle);
			}
			
			// Fire the bullet
			if( pBullet->GetOwner()->GetType() == OBJ_ENEMY )
				vShot = Vector2DNormalize(vShot);
			else if( pBullet->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;

			pBullet->SetBaseVelX(vShot.fX * fRocketVelocity);
			pBullet->SetBaseVelY(vShot.fY * fRocketVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pBullet);
			pBullet->Release();

			break;
		}	
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage* pDR = (CDestroyBulletMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetBulletPointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_ROCKET:
		{
			CCreateRocketMessage* pCR = (CCreateRocketMessage*)pMsg;
			float fRocketVelocity = 300;

			// Create the bullet, set the owner and set the dimensions
			CRocket* pRocket = (CRocket*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CRocket");
			pRocket->SetOwner(pCR->GetOwnerPointer());	
			pRocket->SetWidth(100);
			pRocket->SetHeight(30);

			// Set the starting position
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();
			pRocket->SetPosX((float)ptStartingPos.m_nX);
			pRocket->SetPosY((float)ptStartingPos.m_nY);	

			// Set the starting position vector
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}

			// Set the target position vector
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() + CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Get the firing direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;

			// Default vector
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// Set the rotation
			if(pRocket->GetOwner()->GetType() == OBJ_PLAYER)
			{
				pRocket->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate( vecRocketRotation, CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() );
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vShot ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pRocket->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pRocket->SetRotation(fAngle);
			}

			// Fire the bullet
			if( pRocket->GetOwner()->GetType() == OBJ_ENEMY )
				vShot = Vector2DNormalize(vShot);
			else if( pRocket->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;

			pRocket->SetBaseVelX(vShot.fX * fRocketVelocity);
			pRocket->SetBaseVelY(vShot.fY * fRocketVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pRocket);
			pRocket->Release();

			break;
		}	
	case MSG_DESTROY_ROCKET:
		{
			CDestroyRocketMessage* pDR = (CDestroyRocketMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetRocketPointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_FLAME:
		{
			CCreateFlameMessage* pCR = (CCreateFlameMessage*)pMsg;
			float fFlameVelocity = 300;

			// Create the bullet, set the owner and the dimensions
			CFlame* pFlame = (CFlame*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CFlame");
			pFlame->SetOwner(pCR->GetOwnerPointer());		
			pFlame->SetWidth(206);
			pFlame->SetHeight(68);
			
			// Get the firing position
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();
			pFlame->SetPosX((float)ptStartingPos.m_nX);
			pFlame->SetPosY((float)ptStartingPos.m_nY);
			
			// Get the firing position vector
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}

			// Get target position vector
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() + CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Get the firing direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;

			// Default vector
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// Set the rotation
			if(pFlame->GetOwner()->GetType() == OBJ_PLAYER)
			{
				pFlame->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate( vecRocketRotation, CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() );
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vShot ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pFlame->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pFlame->SetRotation(fAngle);
			}

			// Fire the bullet
			if(pFlame->GetOwner()->GetType() == OBJ_ENEMY)
				vShot = Vector2DNormalize(vShot);
			else if(pFlame->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;

			pFlame->SetBaseVelX(vShot.fX * fFlameVelocity);
			pFlame->SetBaseVelY(vShot.fY * fFlameVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pFlame);
			pFlame->Release();

		}
		break;
	case MSG_DESTROY_FLAME:
		{
			CDestroyFlameMessage* pDR = (CDestroyFlameMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetFlamePointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_PLASMA:
		{
			CCreatePlasmaMessage* pCR = (CCreatePlasmaMessage*)pMsg;
			float fFlameVelocity = 300;

			// Create the bullet, set the owner and the dimensions
			CPlasma* pPlasma = (CPlasma*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CPlasma");
			pPlasma->SetOwner(pCR->GetOwnerPointer());
			pPlasma->SetWidth(42);
			pPlasma->SetHeight(48);
			
			// Get the firing position	
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();
			pPlasma->SetPosX((float)ptStartingPos.m_nX);
			pPlasma->SetPosY((float)ptStartingPos.m_nY);

			// Set the firing position vector			
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}
			
			// Set target vector
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() + CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Get firing direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;

			// Default vector
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// Set the rotation
			if(pPlasma->GetOwner()->GetType() == OBJ_PLAYER)
			{
				pPlasma->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate(vecRocketRotation, CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation());
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vShot ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pPlasma->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pPlasma->SetRotation(fAngle);
			}

			// Fire the bullet
			if( pPlasma->GetOwner()->GetType() == OBJ_ENEMY)
				vShot = Vector2DNormalize(vShot);
			else if( pPlasma->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;

			pPlasma->SetBaseVelX(vShot.fX * fFlameVelocity);
			pPlasma->SetBaseVelY(vShot.fY * fFlameVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pPlasma);
			pPlasma->Release();
		}
		break;
	case MSG_DESTROY_PLASMA:
		{
			CDestroyPlasmaMessage* pDR = (CDestroyPlasmaMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetPlasmaPointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_GRENADE:
		{
			CCreateGrenadeMessage* pCR = (CCreateGrenadeMessage*)pMsg;
			float fFlameVelocity = 300;

			// Create the bullet, set the owner and dimensions
			CGrenade* pGrenade = (CGrenade*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CGrenade");
			pGrenade->SetOwner(pCR->GetOwnerPointer());
			pGrenade->SetWidth(25);
			pGrenade->SetHeight(24);

			// Get the firing position			
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();
			pGrenade->SetPosX((float)ptStartingPos.m_nX);
			pGrenade->SetPosY((float)ptStartingPos.m_nY);

			// Firing position vector
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}

			// Firing Target
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Find the direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;

			// Default vector
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// Set Rotation
			if( pGrenade->GetOwner()->GetType() == OBJ_PLAYER )
			{
				pGrenade->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate( vecRocketRotation, CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() );
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				tVector2D vecPosition;
				vecPosition.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() - pGrenade->GetPosX();
				vecPosition.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY() - pGrenade->GetPosY();

				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vecPosition ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pGrenade->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pGrenade->SetRotation(fAngle);
			}

			// Fire the bullet
			if( pGrenade->GetOwner()->GetType() == OBJ_ENEMY )
				vShot = Vector2DNormalize(vShot);
			else if( pGrenade->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;

			pGrenade->SetBaseVelX(vShot.fX * fFlameVelocity);
			pGrenade->SetBaseVelY(vShot.fY * fFlameVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pGrenade);
			pGrenade->Release();
		}
		break;
	case MSG_DESTROY_GRENADE:
		{
			CDestroyGrenadeMessage* pDR = (CDestroyGrenadeMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetGrenadePointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_SHOCK:
		{
			CCreateShockMessage* pCR = (CCreateShockMessage*)pMsg;
			float fFlameVelocity = 300;

			// Create the bullet, set its owner and its dimensions
			CShock* pShock = (CShock*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CShock");
			pShock->SetOwner(pCR->GetOwnerPointer());		
			pShock->SetWidth(45);
			pShock->SetHeight(112);
			
			// Get firing position
			CPoint ptStartingPos = pCR->GetOwnerPointer()->GetBulletStartPos();
			pShock->SetPosX((float)ptStartingPos.m_nX);
			pShock->SetPosY((float)ptStartingPos.m_nY);

			// Set firing position vector
			tVector2D bOwnerPos;
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				CPlayer* pPlayer = (CPlayer*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pPlayer->GetPosX() + pPlayer->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pPlayer->GetPosY() + pPlayer->GetAnimations()->GetPivotPoint().m_nY;
			}
			if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* pEnemy = (CBaseEnemy*)(pCR->GetOwnerPointer());
				bOwnerPos.fX = pEnemy->GetPosX() + pEnemy->GetAnimations()->GetPivotPoint().m_nX; 
				bOwnerPos.fY = pEnemy->GetPosY() + pEnemy->GetAnimations()->GetPivotPoint().m_nY;
			}

			// Set the target position vector
			tVector2D vShotPos;
			vShotPos.fX = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosX() + CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetWidth();
			vShotPos.fY = CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetPosY();

			// Get the firing direction
			tVector2D vShot;
			vShot = vShotPos - bOwnerPos;
			
			// Set the vector to find the angle
			tVector2D vecRocketRotation;
			vecRocketRotation.fX = 0.0f;
			vecRocketRotation.fY = -1.0f;

			// If its the player then don't calculate else calculate
			if(pCR->GetOwnerPointer()->GetType() == OBJ_PLAYER)
			{
				pShock->SetRotation( (float)(CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() - .5*SGD_PI));
				vecRocketRotation = Vector2DRotate( vecRocketRotation,  CSinglePlayerState::GetInstance()->GetPlayerPointer()->GetHandRotation() );
			}
			else if(pCR->GetOwnerPointer()->GetType() == OBJ_ENEMY)
			{

				// Find Initial rotation
				float fAngle = AngleBetweenVectors( vecRocketRotation, vShot ) - SGD_PI/2;

				// Calculate final rotation
				if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pShock->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
					fAngle = SGD_PI - fAngle;

				pShock->SetRotation(fAngle);
			}

			// Fire the bullet
			if( pShock->GetOwner()->GetType() == OBJ_ENEMY )
				vShot = Vector2DNormalize(vShot);
			else if( pShock->GetOwner()->GetType() == OBJ_PLAYER )
				vShot = vecRocketRotation;
				
			pShock->SetBaseVelX(vShot.fX * fFlameVelocity);
			pShock->SetBaseVelY(vShot.fY * fFlameVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pShock);
			pShock->Release();

		}
		break;
	case MSG_DESTROY_SHOCK:
		{
			CDestroyShockMessage* pDR = (CDestroyShockMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetShockPointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_FIRE:
		{
			float fFireVelocity = 400;
			CCreateFireMessage* pCR = (CCreateFireMessage*)pMsg;

			// Create the bullet set its owner and its dimensions
			CFire* pFire = (CFire*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CFire");
			CBaseEnemy* pEnemy = (CBaseEnemy*)pCR->GetOwnerPointer();
			pFire->SetWidth(50);
			pFire->SetHeight(32);

			// Get where it was fired from
			CPoint ptStartingPos = (*pEnemy).GetBulletStartPos();
			pFire->SetPosX((float)ptStartingPos.m_nX);
			pFire->SetPosY((float)ptStartingPos.m_nY);
			
			// Get firing position
			tVector2D vOwnerPos;
			vOwnerPos.fX = (float)ptStartingPos.m_nX;
			vOwnerPos.fY = (float)ptStartingPos.m_nY;

			// Get firing target
			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			// Get firing direction
			tVector2D vShot;
			vShot = vPlayerPos - vOwnerPos;

			// Set the angle to find the vector
			tVector2D vecUpVector;
			vecUpVector.fX = 0.0f;
			vecUpVector.fY = -1.0f;
	
			// Find Initial rotation
			float fAngle = AngleBetweenVectors( vecUpVector, vShot ) - SGD_PI/2;

			// Calculate final rotation
			if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pFire->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
				fAngle = SGD_PI - fAngle;
			pFire->SetRotation( fAngle );

			// Fire the bullet
			vShot = Vector2DNormalize(vShot);
			pFire->SetBaseVelX(vShot.fX * fFireVelocity);
			pFire->SetBaseVelY(vShot.fY * fFireVelocity);

			// Add the bullet
			CObjectManager::GetInstance()->AddObject(pFire);
			pFire->Release();

		}
		break;
	case MSG_DESTROY_FIRE:
		{
			CDestroyFireMessage* pDR = (CDestroyFireMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetFirePointer());

			pDR = NULL;
		}
		break;
	case MSG_CREATE_ICE:
		{
			float fFireVelocity = 400;
			CCreateIceMessage* pCR = (CCreateIceMessage*)pMsg;

			// Create the object set the person who fired it and set its dimensions
			CIce* pIce = (CIce*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CIce");
			CBaseEnemy* pEnemy = (CBaseEnemy*)pCR->GetOwnerPointer();
			pIce->SetWidth(49);
			pIce->SetHeight(27);
			
			// Get where it was fired from
			CPoint ptStartingPos = (*pEnemy).GetBulletStartPos();
			pIce->SetPosX((float)ptStartingPos.m_nX);
			pIce->SetPosY((float)ptStartingPos.m_nY);

			// Set the owner vector
			tVector2D vOwnerPos;
			vOwnerPos.fX = (float)ptStartingPos.m_nX;
			vOwnerPos.fY = (float)ptStartingPos.m_nY;

			// Get the player's vector
			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();
			
			// Get the firing direction
			tVector2D vShot;
			vShot = vPlayerPos - vOwnerPos;

			// Set the vector to find the angle
			tVector2D vecUpVector;
			vecUpVector.fX = 0.0f;
			vecUpVector.fY = -1.0f;;
	
			// Find Initial rotation
			float fAngle = AngleBetweenVectors( vecUpVector, vShot ) - SGD_PI/2;

			// Calculate final rotation
			if( CSGD_DirectInput::GetInstance()->MouseGetPosX() < pIce->GetPosX() - CCamera::GetInstance()->GetOffsetX() )
				fAngle = SGD_PI - fAngle;
			pIce->SetRotation( fAngle );						

			// Set Fire direction
			vShot = Vector2DNormalize(vShot);
			pIce->SetBaseVelX(vShot.fX * fFireVelocity);
			pIce->SetBaseVelY(vShot.fY * fFireVelocity);

			// Fire the bullet
			CObjectManager::GetInstance()->AddObject(pIce);
			pIce->Release();

		}
		break;
	case MSG_DESTROY_ICE:
		{
			CDestroyIceMessage* pDR = (CDestroyIceMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDR->GetIcePointer());

			pDR = NULL;
		}
		break;
	case MSG_DESTROY_BLOCK:
		{
			CDestroyBlockMessage* pDestroyBlock = (CDestroyBlockMessage*)pMsg;
			CObjectManager::GetInstance()->RemoveObject(pDestroyBlock->GetBlockPointer());

			pDestroyBlock = NULL;
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CCreateEnemyMessage* pEnemy = (CCreateEnemyMessage*)pMsg;

			//Use "If type..." to decide what to spawn
			//if(pEnemy->GetEnemyType())
			int EnemyType = pEnemy->GetEnemyType();

			switch(EnemyType)
			{
			case Turret_Gun:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY(), Turret_Gun);
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Turret_Frost:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY(), Turret_Frost);
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Turret_Fire:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY(), Turret_Fire);
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			case Turret_Multi:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY(), Turret_Multi);
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Drone_Attack:
				{
					CAttackDrone* CEnemy = new CAttackDrone(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Drone_Seeker:
				{
					CSeekerDrone* CEnemy = new CSeekerDrone(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				} 
				break;
			case Drone_Heavy:
				{
					CHeavyAttackDrone* CEnemy = new CHeavyAttackDrone(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				} 
				break;
			case Ground_Mech:
				{
					CMech* CEnemy = new CMech(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Ground_Siege:
				{
					CSiegeWalker* CEnemy = new CSiegeWalker(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Ground_FLCL:
				{
					CFLCLMech* CEnemy = new CFLCLMech(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Boss_Apple:
				{
					CAppleMech* CEnemy = new CAppleMech(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Boss_Pimp:
				{
					CPimpStriker* CEnemy = new CPimpStriker(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			case Boss_Pirate:
				{
					CDeathPirate* CEnemy = new CDeathPirate(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());
					if(pEnemy->GetSpawner())
						CEnemy->SetSpawner(pEnemy->GetSpawner());
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			}
			pEnemy = NULL;
		}
		break;
	case MSG_DESTROY_ENEMY:
		{
			CDestroyEnemyMessage* pDestroyEnemy = (CDestroyEnemyMessage*)pMsg;

			int EnemyType = pDestroyEnemy->GetEnemyPointer()->GetEnemyType();

			switch (EnemyType)
					{
					case Turret_Gun:
						{
							CTurretCore* Temp = (CTurretCore*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}
						break;
					case Turret_Frost:
						{
							CTurretCore* Temp = (CTurretCore*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Turret_Fire:
						{
							CTurretCore* Temp = (CTurretCore*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Turret_Multi:
						{
							CTurretCore* Temp = (CTurretCore*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Drone_Attack:
						{
							CAttackDrone* Temp = (CAttackDrone*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Drone_Seeker:
						{
							CSeekerDrone* Temp = (CSeekerDrone*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}								
						break;
					case Drone_Heavy:
						{
							CHeavyAttackDrone* Temp = (CHeavyAttackDrone*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}								
						break;
					case Ground_Mech:
						{
							CMech* Temp = (CMech*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}
						break;
					case Ground_Siege:
						{
							CSiegeWalker* Temp = (CSiegeWalker*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Ground_FLCL:
						{
							CFLCLMech* Temp = (CFLCLMech*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}
						break;
					case Boss_Apple:
						{
							CAppleMech* Temp = (CAppleMech*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					case Boss_Pimp:
						{
							CPimpStriker* Temp = (CPimpStriker*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}							
						break;
					case Boss_Pirate:
						{
							CDeathPirate* Temp = (CDeathPirate*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}						
						break;
					}
			pDestroyEnemy = NULL;
		}
		break;
	};

}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetScreenWidth
////////////////////////////////////////////////////////////////////////////////////
int CGame::GetScreenWidth(void) const
{
	return this->m_nScreenWidth;
}
////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetScreenHeight
////////////////////////////////////////////////////////////////////////////////////
int CGame::GetScreenHeight(void) const
{
	return this->m_nScreenHeight;
}

CMessageSystem*	CGame::GetMessageSystemPointer(void) const
{
	return this->m_pMS;
}
