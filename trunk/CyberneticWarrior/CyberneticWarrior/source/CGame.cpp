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
#include "CAchievementsState.h"
#include "CCreditsState.h"
#include "CAtractModeState.h"
#include "CLoadingState.h"

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
#include "CFLCLMech.h"
#include "CMech.h"
#include "CTurretCore.h"




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
	CAchievementsState::GetInstance()->DeleteInstance();
	CControlSelectState::GetInstance()->DeleteInstance();
	CCreditsState::GetInstance()->DeleteInstance();
	CAtractModeState::GetInstance()->DeleteInstance();
	CLoadingState::GetInstance()->DeleteInstance();

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
//	bool turn = ;

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
	this->m_pES->ProcessEvents();
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

   			//pDH->GetPlayerPointer()->SetHookPointer(NULL);
			CSGD_TextureManager::GetInstance()->UnloadTexture(pDH->GetHookPointer()->GetImageID());
			CObjectManager::GetInstance()->RemoveObject(pDH->GetHookPointer());

			pDH = NULL;
		}
		break;
	case MSG_CREATE_BULLET:
		{
			CCreateBulletMessage* pCR = (CCreateBulletMessage*)pMsg;
			CBullet* pBullet;

			float fRocketVelocity = 300;

			pBullet = (CBullet*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CBullet");
			pBullet->SetWidth(16);
			pBullet->SetHeight(16);
			pBullet->SetPosX(pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth());
			pBullet->SetPosY(pCR->GetPlayerPointer()->GetPosY());


			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();


			
			pBullet->SetRotation( SGD_PI + (SGD_PI - AngleBetweenVectors(vPlayerPos, vMousePos)));




			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pBullet->SetBaseVelX(vShot.fX * fRocketVelocity);
			pBullet->SetBaseVelY(vShot.fY * fRocketVelocity);

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
			CRocket* pRocket;

			float fRocketVelocity = 300;

			pRocket = (CRocket*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CRocket");
			pRocket->SetWidth(16);
			pRocket->SetHeight(16);
			pRocket->SetPosX(pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth());
			pRocket->SetPosY(pCR->GetPlayerPointer()->GetPosY());

			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();


			pRocket->SetRotation( SGD_PI + (SGD_PI - AngleBetweenVectors(vPlayerPos, vMousePos)));


			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pRocket->SetBaseVelX(vShot.fX * fRocketVelocity);
			pRocket->SetBaseVelY(vShot.fY * fRocketVelocity);

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
			CFlame* pFlame;

			float fFlameVelocity = 300;

			pFlame = (CFlame*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CFlame");
			pFlame->SetWidth(200);
			pFlame->SetHeight(90);
			pFlame->SetPosX(pCR->GetPlayerPointer()->GetPosX());// + (float)pCR->GetPlayerPointer()->GetWidth());
			pFlame->SetPosY(pCR->GetPlayerPointer()->GetPosY());

			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			
			/*tVector2D vFlameRot;
			vFlameRot.fX = 0.0f;
			vFlameRot.fY = -1.0f;
			tVector2D vMouseRot;
			vMouseRot.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX()-pCR->GetPlayerPointer()->GetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMouseRot.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY()-pCR->GetPlayerPointer()->GetPosY() + CCamera::GetInstance()->GetOffsetY();

			pFlame->SetRotation( SGD_PI + (SGD_PI - AngleBetweenVectors(vFlameRot, vMousePos)));
*/

			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			//Vector2DRotate(vShot, pFlame->GetRotation());

			vShot = Vector2DNormalize(vShot);


			pFlame->SetBaseVelX(vShot.fX * fFlameVelocity);
			pFlame->SetBaseVelY(vShot.fY * fFlameVelocity);

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
			CPlasma* pPlasma;

			float fFlameVelocity = 300;

			pPlasma = (CPlasma*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CPlasma");
			pPlasma->SetWidth(45);
			pPlasma->SetHeight(45);
			pPlasma->SetPosX(pCR->GetPlayerPointer()->GetPosX());// +aaaaaa (float)pCR->GetPlayerPointer()->GetWidth());
			pPlasma->SetPosY(pCR->GetPlayerPointer()->GetPosY());

			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pPlasma->SetBaseVelX(vShot.fX * fFlameVelocity);
			pPlasma->SetBaseVelY(vShot.fY * fFlameVelocity);

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
			CGrenade* pGrenade;

			float fFlameVelocity = 300;

			pGrenade = (CGrenade*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CGrenade");
			pGrenade->SetWidth(30);
			pGrenade->SetHeight(30);
			pGrenade->SetPosX(pCR->GetPlayerPointer()->GetPosX());// + (float)pCR->GetPlayerPointer()->GetWidth());
			pGrenade->SetPosY(pCR->GetPlayerPointer()->GetPosY());

			if((CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY())>pCR->GetPlayerPointer()->GetPosY())
			{
				pGrenade->SetYVelocity(350.0f);
			}
			else
			{
				pGrenade->SetYVelocity(-350.0f);
			}

			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pGrenade->SetBaseVelX(vShot.fX * fFlameVelocity);
			pGrenade->SetBaseVelY(vShot.fY * fFlameVelocity);

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
			CShock* pShock;

			float fFlameVelocity = 300;

			pShock = (CShock*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CShock");
			pShock->SetWidth(40);
			pShock->SetHeight(90);
			pShock->SetPosX(pCR->GetPlayerPointer()->GetPosX());// + (float)pCR->GetPlayerPointer()->GetWidth());
			pShock->SetPosY(pCR->GetPlayerPointer()->GetPosY());

			tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX() + CCamera::GetInstance()->GetOffsetX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY() + CCamera::GetInstance()->GetOffsetY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			tVector2D vShot;

			vShot = vMousePos - vPlayerPos;

			vShot = Vector2DNormalize(vShot);


			pShock->SetBaseVelX(vShot.fX * fFlameVelocity);
			pShock->SetBaseVelY(vShot.fY * fFlameVelocity);

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
			CCreateFireMessage* pCR = (CCreateFireMessage*)pMsg;
			CFire* pFire;

			float fFireVelocity = 400;

			pFire = (CFire*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CFire");
			pFire->SetWidth(50);
			pFire->SetHeight(32);
			pFire->SetPosX(pCR->GetOwnerPointer()->GetPosX());
			pFire->SetPosY(pCR->GetOwnerPointer()->GetPosY());

			tVector2D vOwnerPos;
			vOwnerPos.fX = (float)pCR->GetOwnerPointer()->GetPosX();
			vOwnerPos.fY = (float)pCR->GetOwnerPointer()->GetPosY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			tVector2D vShot;

			vShot = vPlayerPos - vOwnerPos;

			vShot = Vector2DNormalize(vShot);


			pFire->SetBaseVelX(vShot.fX * fFireVelocity);
			pFire->SetBaseVelY(vShot.fY * fFireVelocity);

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
			CCreateIceMessage* pCR = (CCreateIceMessage*)pMsg;
			CIce* pIce;

			float fFireVelocity = 400;

			pIce = (CIce*)CObjectFactory<std::string, CBase>::GetInstance()->CreateObject("CIce");
			pIce->SetWidth(50);
			pIce->SetHeight(32);
			pIce->SetPosX(pCR->GetOwnerPointer()->GetPosX());
			pIce->SetPosY(pCR->GetOwnerPointer()->GetPosY());

			tVector2D vOwnerPos;
			vOwnerPos.fX = (float)pCR->GetOwnerPointer()->GetPosX();
			vOwnerPos.fY = (float)pCR->GetOwnerPointer()->GetPosY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

			tVector2D vShot;

			vShot = vPlayerPos - vOwnerPos;

			vShot = Vector2DNormalize(vShot);


			pIce->SetBaseVelX(vShot.fX * fFireVelocity);
			pIce->SetBaseVelY(vShot.fY * fFireVelocity);

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
			{
				CFLCLMech* CEnemy = new CFLCLMech(-1, (float)pEnemy->GetPosX(), (float)pEnemy->GetPosY());


				CObjectManager::GetInstance()->AddObject(CEnemy);
				CEnemy->Release();
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
						
						break;
					case Drone_Seeker:
						
						break;
					case Drone_Heavy:
						
						break;
					case Ground_Mech:
						{
							CMech* Temp = (CMech*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}
						break;
					case Ground_Siege:
						
						break;
					case Ground_FLCL:
						{
							CFLCLMech* Temp = (CFLCLMech*)pDestroyEnemy->GetEnemyPointer();
							CObjectManager::GetInstance()->RemoveObject(Temp);
							Temp = NULL;
						}
						break;
					case Boss_Apple:
						
						break;
					case Boss_Pimp:
						
						break;
					case Boss_Pirate:
						
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
