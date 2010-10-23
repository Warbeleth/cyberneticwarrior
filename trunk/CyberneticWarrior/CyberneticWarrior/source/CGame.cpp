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
#include "CGame.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_DirectSound.h"
#include "SGD Wrappers/CSGD_WaveManager.h"

#include "CCodeProfiler.h"

#include "CCamera.h"

#include "CStackStateMachine.h"
#include "CMainMenuState.h"
#include "CSinglePlayerState.h"
#include "CSinglePlayerMenuState.h"
#include "SaveState.h"
#include "CLoadState.h"
#include "CPauseMenuState.h"
#include "COptionsMenuState.h"

#include "CEventSystem.h"

#include "CObjectFactory.h"
#include "CObjectManager.h"
#include "CBase.h"
#include "CGrapplingHook.h"
#include "CRocket.h"


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


	m_nCodeProfilerID = this->m_pCP->CreateFunction("Update");

	this->m_pSSM->ChangeState(CMainMenuState::GetInstance());
	
	


}

////////////////////////////////////////////////////////////////////////////////////
//	Function : ShutDown
////////////////////////////////////////////////////////////////////////////////////
void CGame::ShutDown(void)
{

	CCodeProfiler::GetInstance()->SavePerformance();


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
	CSaveState::GetInstance()->DeleteInstance();
	CLoadState::GetInstance()->DeleteInstance();
	CSinglePlayerMenuState::GetInstance()->DeleteInstance();
	CPauseMenuState::GetInstance()->DeleteInstance();
	COptionsMenuState::GetInstance()->DeleteInstance();
	CCodeProfiler::GetInstance()->DeleteInstance();


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
	bool turn = this->m_pSSM->Input();

	///////////////////////////////////
	// Temp
	///////////////////////////////////
	/*if(m_pDI->KeyDown(DIK_ESCAPE))
	{
		PostQuitMessage(0);
	}*/
	//////////////////////////////////

	//return 1;

	return turn;
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Update
////////////////////////////////////////////////////////////////////////////////////
void CGame::Update(float fElapsedTime)
{
	this->m_pCP->FunctionStart(this->m_nCodeProfilerID);
	
	this->m_pWM->Update();
	this->m_pSSM->UpdateState(fElapsedTime);
	this->m_pES->ProcessEvents();
	this->m_pMS->ProcessMessages();

	this->m_pCP->FuntionEnd(this->m_nCodeProfilerID);
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

			static tVector2D vMousePos;
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY();

			static tVector2D vPlayerPos;
			vPlayerPos.fX = pGH->GetPlayerPointer()->GetPosX() + (float)pGH->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pGH->GetPlayerPointer()->GetPosY();//) - (float)pGH->GetPlayerPointer()->GetHeight()/2;

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
			CSGD_TextureManager::GetInstance()->UnloadTexture(pDH->GetHookPointer()->GetImageID());
			CObjectManager::GetInstance()->RemoveObject(pDH->GetHookPointer());

			pDH = NULL;
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
			vMousePos.fX = (float)CSGD_DirectInput::GetInstance()->MouseGetPosX();
			vMousePos.fY = (float)CSGD_DirectInput::GetInstance()->MouseGetPosY();

			tVector2D vPlayerPos;
			vPlayerPos.fX = pCR->GetPlayerPointer()->GetPosX() + (float)pCR->GetPlayerPointer()->GetWidth();
			vPlayerPos.fY = pCR->GetPlayerPointer()->GetPosY();

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