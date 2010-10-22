//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : WinMain.cpp
//
//	Author : Patrick Alvarez 
//	Based in part on:
//		-Window code from the book: "Physics for Game Developers" by David M. Bourg.
//		-The previous WinMain.cpp by Jensen Rivera.
//		-Dave Brown and SGD Course.		(Full Sail University)
//
//	Purpose : To Provide a basic Window Framework for the Cybernetic Warrior Game build.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "CGame.h"
#include "..//resource.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include "vld.h"

const char* g_szWINDOW_CLASS_NAME	=	"Holy Melon";

const char*	g_szWINDOW_TITLE		=	"Cybernetic Warrior";
const int	g_nWINDOW_WIDTH			=	640;
const int	g_nWINDOW_HEIGHT		=	480;

//#ifndef	_DEBUG 1
const BOOL	g_bIS_WINDOWED			=	TRUE;
//#else
//const BOOL	g_bIS_WINDOWED			=	FALSE;
//#endif

////////////////////////////////////////////////////////////////////////////////////
//
//	Function : Windowproc
//
//	Purpose : To create basic Windows paint objects 
//			  and to check if basic Windows flags have gone off.
//
////////////////////////////////////////////////////////////////////////////////////
LRESULT	CALLBACK	WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC			hdc;

	switch(msg)
	{
	case WM_SYSKEYUP:
		break;
	case WM_SYSCHAR:
		return(0);
		break;
	case WM_CLOSE:
		{
			if(LOWORD(wParam) != WA_INACTIVE)
			{
			}
			else
			{
			}
		}
		break;
	case WM_CREATE:
		{
			return(0);
		}
		break;
	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);

			EndPaint(hWnd, &ps);
			return(0);
		}
		break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
		break;
	default:
		break;
	}

	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Function : CheckIfAlreadyRunning
//
//	Purpose : To check if a Window with this game is already open and running.
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CheckIfAlreadyRunning(void)
{
	HWND hWnd = FindWindow(g_szWINDOW_CLASS_NAME, g_szWINDOW_TITLE);

	if(hWnd)
	{
		if(IsIconic(hWnd))
		{
			ShowWindow(hWnd, SW_RESTORE);
		}

		SetForegroundWindow(hWnd);

		return TRUE;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Function : RegisterWindowClass
//
//	Purpose : Registers a Window class and sets off especific flags
//			  to meet the games requirements.
//
////////////////////////////////////////////////////////////////////////////////////
BOOL RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX winClassEx;

	winClassEx.cbSize			= sizeof(winClassEx);
	winClassEx.style			= CS_DBLCLKS | CS_OWNDC |CS_HREDRAW |CS_VREDRAW;
	winClassEx.lpfnWndProc		= WindowProc;
	winClassEx.cbClsExtra		= 0;
	winClassEx.cbWndExtra		= 0;
	winClassEx.hInstance		= hInstance;
	winClassEx.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hIconSm			= NULL;
	winClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	winClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClassEx.lpszMenuName		= NULL;
	winClassEx.lpszClassName	= g_szWINDOW_CLASS_NAME;

	return RegisterClassEx(&winClassEx);
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Function : MakeWindow
//
//	Purpose : To create and size a basic Windows window.
//
////////////////////////////////////////////////////////////////////////////////////
HWND MakeWindow(HINSTANCE hInstance)
{
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	if(g_bIS_WINDOWED)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		ShowCursor(FALSE);
	}

	RECT rWindow;
	rWindow.left	= 0;
	rWindow.top		= 0;
	rWindow.right	= g_nWINDOW_WIDTH;
	rWindow.bottom	= g_nWINDOW_HEIGHT;

	AdjustWindowRectEx(&rWindow, dwWindowStyleFlags, FALSE, WS_EX_APPWINDOW);

	int nWindowWidth		= rWindow.right - rWindow.left;
	int nWindowHeight		= rWindow.bottom - rWindow.top;

	return CreateWindowEx(WS_EX_APPWINDOW,
		g_szWINDOW_CLASS_NAME,
		g_szWINDOW_TITLE,
		dwWindowStyleFlags,
		(GetSystemMetrics(SM_CXSCREEN)/2) - (nWindowWidth/2),
		(GetSystemMetrics(SM_CYSCREEN)/2) - (nWindowHeight/2),
		nWindowWidth,
		nWindowHeight,
		NULL,
		NULL,
		hInstance,
		NULL);
}

////////////////////////////////////////////////////////////////////////////////////
//
//	Function : WinMain
//
//	Purpose : Creates and utilizes basic Windows handles and 
//			  messaging systems required to run an application, 
//			  along with running main game loop.
//
////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(LEAK_LINE);
	

	MSG		msg;
	HWND	hWnd;


	if(!hPrevInstance)
	{
		if(CheckIfAlreadyRunning())
		{
			return FALSE;
		}
	}


	if(!RegisterWindowClass(hInstance))
	{
		return 0;
	}


	hWnd = MakeWindow(hInstance);

	if(!hWnd)
	{
		return(0);
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	CGame* pGame = CGame::GetInstance();
	pGame->Initialize(hWnd, hInstance, g_nWINDOW_WIDTH, g_nWINDOW_HEIGHT, g_bIS_WINDOWED);

	while(TRUE)
	{
		if(pGame->Main() == false)
		{
			PostQuitMessage(0);
		}

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
	}



	pGame->ShutDown();
	pGame->DeleteInstance();


	UnregisterClass(g_szWINDOW_CLASS_NAME, hInstance);

	return (int) (msg.wParam);


}