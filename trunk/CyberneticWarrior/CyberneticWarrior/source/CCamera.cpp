//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : CCamera.cpp
//
//	Author : Anthony Muccio / Corey Ringer / Patrick Alvarez 
//
//	Purpose : Camera class used to update world position for player and game objects.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CCamera.h"

CCamera* CCamera::sm_pCameraInstance = 0;

CCamera::CCamera( void )
{
	m_nX = m_nY = 0;
	m_fScale = 0.5f;
}

CCamera::~CCamera( void )
{

}

CCamera* CCamera::GetInstance( void )
{
	if(!sm_pCameraInstance)
	{
		sm_pCameraInstance = new CCamera();
	}
	return sm_pCameraInstance;
}

void CCamera::DeleteInstance( void )
{
	if(sm_pCameraInstance)
	{
		delete sm_pCameraInstance;
		sm_pCameraInstance = 0;
	}
}