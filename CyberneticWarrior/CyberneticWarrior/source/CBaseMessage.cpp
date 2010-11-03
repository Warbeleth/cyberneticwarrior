#include "CBaseMessage.h"
#include "CGrapplingHook.h"
#include "CPlayer.h"
#include "CRocket.h"
#include "CFlame.h"
#include "CPlasma.h"
#include "CShock.h"
#include "CGrenade.h"
#include "CFire.h"
#include "CIce.h"
#include "CBlock.h"

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
	this->m_pPlayer = pPlayer;
}

CCreateRocketMessage::~CCreateRocketMessage( void )
{
}

CPlayer* CCreateRocketMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateRocketMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDetroyRocketMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyRocketMessage::CDestroyRocketMessage(CRocket* pRocket, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_ROCKET)
{
	this->m_pRocket = pRocket;
	this->m_pPlayer = pPlayer;
}

CDestroyRocketMessage::~CDestroyRocketMessage(void)
{
}

CPlayer* CDestroyRocketMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CRocket* CDestroyRocketMessage::GetRocketPointer(void)
{
	return this->m_pRocket;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateBulletMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateBulletMessage::CCreateBulletMessage( CPlayer* pPlayer ) : CBaseMessage(MSG_CREATE_BULLET)
{	
	this->m_pPlayer = pPlayer;
}

CCreateBulletMessage::~CCreateBulletMessage( void )
{
}

CPlayer* CCreateBulletMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateBulletMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyBulletMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_BULLET)
{
	this->m_pBullet = pBullet;
	this->m_pPlayer = pPlayer;
}

CDestroyBulletMessage::~CDestroyBulletMessage(void)
{
}

CPlayer* CDestroyBulletMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CBullet* CDestroyBulletMessage::GetBulletPointer(void)
{
	return this->m_pBullet;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CCreateFlameMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateFlameMessage::CCreateFlameMessage( CPlayer* pPlayer) : CBaseMessage(MSG_CREATE_FLAME)
{	
	this->m_pPlayer = pPlayer;
}

CCreateFlameMessage::~CCreateFlameMessage( void )
{
}

CPlayer* CCreateFlameMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateFlameMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyFlameMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyFlameMessage::CDestroyFlameMessage(CFlame* pFlame, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_FLAME)
{
	this->m_pFlame = pFlame;
	this->m_pPlayer = pPlayer;
}

CDestroyFlameMessage::~CDestroyFlameMessage(void)
{
}

CPlayer* CDestroyFlameMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CFlame* CDestroyFlameMessage::GetFlamePointer(void)
{
	return this->m_pFlame;
}



///////////////////////////////////////////////////////////////////////////////
//	class "CCreatePlasmaMessage"
///////////////////////////////////////////////////////////////////////////////
CCreatePlasmaMessage::CCreatePlasmaMessage( CPlayer* pPlayer ) : CBaseMessage(MSG_CREATE_PLASMA)
{	
	this->m_pPlayer = pPlayer;
}

CCreatePlasmaMessage::~CCreatePlasmaMessage( void )
{
}

CPlayer* CCreatePlasmaMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreatePlasmaMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyPlasmaMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyPlasmaMessage::CDestroyPlasmaMessage(CPlasma* pPlasma, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_PLASMA)
{
	this->m_pPlasma = pPlasma;
	this->m_pPlayer = pPlayer;
}

CDestroyPlasmaMessage::~CDestroyPlasmaMessage(void)
{
}

CPlayer* CDestroyPlasmaMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CPlasma* CDestroyPlasmaMessage::GetPlasmaPointer(void)
{
	return this->m_pPlasma;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateShockMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateShockMessage::CCreateShockMessage( CPlayer* pPlayer ) : CBaseMessage(MSG_CREATE_SHOCK)
{	
	this->m_pPlayer = pPlayer;
}

CCreateShockMessage::~CCreateShockMessage( void )
{
}

CPlayer* CCreateShockMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateShockMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyShockMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyShockMessage::CDestroyShockMessage(CShock* pShock, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_SHOCK)
{
	this->m_pShock = pShock;
	this->m_pPlayer = pPlayer;
}

CDestroyShockMessage::~CDestroyShockMessage(void)
{
}

CPlayer* CDestroyShockMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CShock* CDestroyShockMessage::GetShockPointer(void)
{
	return this->m_pShock;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateGrenadeMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateGrenadeMessage::CCreateGrenadeMessage( CPlayer* pPlayer ) : CBaseMessage(MSG_CREATE_GRENADE)
{	
	this->m_pPlayer = pPlayer;
}

CCreateGrenadeMessage::~CCreateGrenadeMessage( void )
{
}

CPlayer* CCreateGrenadeMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateGrenadeMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyGrenadeMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyGrenadeMessage::CDestroyGrenadeMessage(CGrenade* pGrenade, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_GRENADE)
{
	this->m_pGrenade = pGrenade;
	this->m_pPlayer = pPlayer;
}

CDestroyGrenadeMessage::~CDestroyGrenadeMessage(void)
{
}

CPlayer* CDestroyGrenadeMessage::GetPlayerPointer(void)
{
	return this->m_pPlayer;
}

CGrenade* CDestroyGrenadeMessage::GetGrenadePointer(void)
{
	return this->m_pGrenade;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateIceMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateIceMessage::CCreateIceMessage( CPlayer* pPlayer, CBaseEnemy* pOwner ) : CBaseMessage(MSG_CREATE_ICE)
{	
	this->m_pPlayer = pPlayer;
	this->m_pOwner = pOwner;
}

CCreateIceMessage::~CCreateIceMessage( void )
{
}

CPlayer* CCreateIceMessage::GetPlayerPointer( void )
{
	return this->m_pPlayer;
}

void CCreateIceMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}

CBaseEnemy*	CCreateIceMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

void CCreateIceMessage::SetOwnerPointer(CBaseEnemy* pOwner)
{
	this->m_pOwner = pOwner;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyIceMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyIceMessage::CDestroyIceMessage(CIce* pIce, CBaseEnemy* pOwner) : CBaseMessage(MSG_DESTROY_ICE)
{
	this->m_pIce = pIce;
	this->m_pOwner = pOwner;
}

CDestroyIceMessage::~CDestroyIceMessage(void)
{
}

CBaseEnemy* CDestroyIceMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CIce* CDestroyIceMessage::GetIcePointer(void)
{
	return this->m_pIce;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateFireMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateFireMessage::CCreateFireMessage( CPlayer* pPlayer, CBaseEnemy* pOwner ) : CBaseMessage(MSG_CREATE_FIRE)
{	
	this->m_pPlayer = pPlayer;
	this->m_pOwner = pOwner;
}

CCreateFireMessage::~CCreateFireMessage( void )
{
}

CPlayer* CCreateFireMessage::GetPlayerPointer( void ) 
{
	return this->m_pPlayer;
}

void CCreateFireMessage::SetPlayerPointer( CPlayer* pPlayer )
{
	this->m_pPlayer = pPlayer;
}

CBaseEnemy* CCreateFireMessage::GetOwnerPointer(void) 
{
	return this->m_pOwner;
}

void CCreateFireMessage::SetOwnerPointer(CBaseEnemy* pOwner)
{
	this->m_pOwner = pOwner;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyFireMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyFireMessage::CDestroyFireMessage(CFire* pFire, CBaseEnemy* pOwner) : CBaseMessage(MSG_DESTROY_FIRE)
{
	this->m_pFire = pFire;
	this->m_pOwner = pOwner;
}

CDestroyFireMessage::~CDestroyFireMessage(void)
{
}

CBaseEnemy* CDestroyFireMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CFire* CDestroyFireMessage::GetFirePointer(void)
{
	return this->m_pFire;
}

CDestroyBlockMessage::CDestroyBlockMessage(CBlock* pBlock) : CBaseMessage(MSG_DESTROY_BLOCK)
{
	this->m_pBlock = pBlock;
}

CDestroyBlockMessage::~CDestroyBlockMessage(void)
{
}

CDestroyEnemyMessage::CDestroyEnemyMessage(CBaseEnemy* pEnemy) : CBaseMessage(MSG_DESTROY_ENEMY)
{
	this->m_pEnemy = pEnemy;
}

CDestroyEnemyMessage::~CDestroyEnemyMessage(void)
{
}

CCreateEnemyMessage::CCreateEnemyMessage(int nEnemyType, int nPosX, int nPosY) : CBaseMessage(MSG_CREATE_ENEMY)
{
	m_nEnemyType = nEnemyType;
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

CCreateEnemyMessage::~CCreateEnemyMessage(void)
{
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