#include "CBaseMessage.h"
#include "CGrapplingHook.h"
#include "CPlayer.h"

CBaseMessage::CBaseMessage(MSGID msgID)
{
	this->m_nMsgID = msgID;
}



CCreateHookMessage::CCreateHookMessage(CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_HOOK)
{
	this->m_pPlayer = pPlayer;
}

CCreateHookMessage::~CCreateHookMessage(void)
{
}

CPlayer*	CCreateHookMessage::GetPlayerPointer(void) { return this->m_pPlayer; }
void	CCreateHookMessage::SetPlayerPointer(CPlayer* pPlayer)	{ this->m_pPlayer = pPlayer; }


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

/* ---Message Class Example---
CCreateEnemyMessage::CCreateEnemyMessage(CEnemy* pEnemy) : CBaseMessage(MSG_CREATE_ENEMY)
{
	this->m_pEnemy = pEnemy;
}

CCreateEnemyMessage::~CCreateEnemyMessage(void)
{
}
*/