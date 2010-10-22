#include "CBaseMessage.h"
#include "CGrapplingHook.h"
#include "CPlayer.h"
#include "CRocket.h"

///////////////////////////////////////////////////////////////////////////////
//	class "CBaseMessage"
///////////////////////////////////////////////////////////////////////////////
CBaseMessage::CBaseMessage(MSGID msgID)
{
	this->m_nMsgID = msgID;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateHookMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateHookMessage::CCreateHookMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_HOOK)
{
	this->m_pPlayer = pPlayer;
}

CCreateHookMessage::~CCreateHookMessage(void)
{
}

CPlayer*	CCreateHookMessage::GetPlayerPointer(void) { return this->m_pPlayer; }
void	CCreateHookMessage::SetPlayerPointer(CPlayer* pPlayer)	{ this->m_pPlayer = pPlayer; }

///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyHookMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyHookMessage::CDestroyHookMessage(CGrapplingHook*	pHook, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_HOOK)
{
	this->m_pHook	= pHook;
	this->m_pPlayer = pPlayer;
}
CDestroyHookMessage::~CDestroyHookMessage(void)
{
}
CGrapplingHook*	CDestroyHookMessage::GetHookPointer(void)
{
	return this->m_pHook;
}
CPlayer*	CDestroyHookMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CCreateRocketMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateRocketMessage::CCreateRocketMessage( CPlayer* pPlayer ) : CBaseMessage(MSG_CREATE_ROCKET)
{	
	m_pPlayer = pPlayer;
}

CCreateRocketMessage::~CCreateRocketMessage( void )
{
}

CPlayer* CCreateRocketMessage::GetPlayerPointer( void )
{
	return m_pPlayer;
}

void CCreateRocketMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDetroyRocketMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyRocketMessage::CDestroyRocketMessage(CRocket* pRocket, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_ROCKET)
{
	m_pRocket = pRocket;
	m_pPlayer = pPlayer;
}

CDestroyRocketMessage::~CDestroyRocketMessage(void)
{
}

CPlayer* CDestroyRocketMessage::GetPlayerPointer(void)
{
	return m_pPlayer;
}

CRocket* CDestroyRocketMessage::GetRocketPointer(void)
{
	return m_pRocket;
}

/* ---Message Class Example---
CCreateEnemyMessage::CCreateEnemyMessage(CEnemy* pEnemy) : CBaseMessage(MSG_CREATE_ENEMY)
{
	this->m_pEnemy = pEnemy;
}

CCreateEnemyMessage::~CCreateEnemyMessage(void)
{
}
*/