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
CDestroyHookMessage::CDestroyHookMessage(CGrapplingHook* pHook, CPlayer* pPlayer) : CBaseMessage(MSG_DESTROY_HOOK)
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
CCreateRocketMessage::CCreateRocketMessage( CBase* pOwner ) : CBaseMessage(MSG_CREATE_ROCKET)
{	
	this->m_pOwner = pOwner;
}

CCreateRocketMessage::~CCreateRocketMessage( void )
{
}

CBase* CCreateRocketMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreateRocketMessage::SetOwnerPointer( CBase* pOwner )
{
	this->m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDetroyRocketMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyRocketMessage::CDestroyRocketMessage(CRocket* pRocket, CBase* pOwner) : CBaseMessage(MSG_DESTROY_ROCKET)
{
	this->m_pRocket = pRocket;
	this->m_pOwner = pOwner;
}

CDestroyRocketMessage::~CDestroyRocketMessage(void)
{
}

CBase* CDestroyRocketMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CRocket* CDestroyRocketMessage::GetRocketPointer(void)
{
	return this->m_pRocket;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateBulletMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateBulletMessage::CCreateBulletMessage( CBase* pOwner ) : CBaseMessage(MSG_CREATE_BULLET)
{	
	this->m_pOwner = pOwner;
}

CCreateBulletMessage::~CCreateBulletMessage( void )
{
}

CBase* CCreateBulletMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreateBulletMessage::SetOwnerPointer( CBase* pOwner )
{
	this->m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyBulletMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet, CBase* pOwner) : CBaseMessage(MSG_DESTROY_BULLET)
{
	this->m_pBullet = pBullet;
	this->m_pOwner = pOwner;
}

CDestroyBulletMessage::~CDestroyBulletMessage(void)
{
}

CBase* CDestroyBulletMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CBullet* CDestroyBulletMessage::GetBulletPointer(void)
{
	return this->m_pBullet;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CCreateFlameMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateFlameMessage::CCreateFlameMessage( CBase* pOwner) : CBaseMessage(MSG_CREATE_FLAME)
{	
	this->m_pOwner = pOwner;
}

CCreateFlameMessage::~CCreateFlameMessage( void )
{
}

CBase* CCreateFlameMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreateFlameMessage::SetOwnerPointer( CBase* pOwner )
{
	m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyFlameMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyFlameMessage::CDestroyFlameMessage(CFlame* pFlame, CBase* pOwner) : CBaseMessage(MSG_DESTROY_FLAME)
{
	this->m_pFlame = pFlame;
	this->m_pOwner = pOwner;
}

CDestroyFlameMessage::~CDestroyFlameMessage(void)
{
}

CBase* CDestroyFlameMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CFlame* CDestroyFlameMessage::GetFlamePointer(void)
{
	return this->m_pFlame;
}



///////////////////////////////////////////////////////////////////////////////
//	class "CCreatePlasmaMessage"
///////////////////////////////////////////////////////////////////////////////
CCreatePlasmaMessage::CCreatePlasmaMessage( CBase* pOwner ) : CBaseMessage(MSG_CREATE_PLASMA)
{	
	this->m_pOwner = pOwner;
}

CCreatePlasmaMessage::~CCreatePlasmaMessage( void )
{
}

CBase* CCreatePlasmaMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreatePlasmaMessage::SetOwnerPointer( CBase* pOwner )
{
	this->m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyPlasmaMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyPlasmaMessage::CDestroyPlasmaMessage(CPlasma* pPlasma, CBase* m_pOwner) : CBaseMessage(MSG_DESTROY_PLASMA)
{
	this->m_pPlasma = pPlasma;
	this->m_pOwner = m_pOwner;
}

CDestroyPlasmaMessage::~CDestroyPlasmaMessage(void)
{
}

CBase* CDestroyPlasmaMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CPlasma* CDestroyPlasmaMessage::GetPlasmaPointer(void)
{
	return this->m_pPlasma;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateShockMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateShockMessage::CCreateShockMessage( CBase* pOwner ) : CBaseMessage(MSG_CREATE_SHOCK)
{	
	this->m_pOwner = pOwner;
}

CCreateShockMessage::~CCreateShockMessage( void )
{
}

CBase* CCreateShockMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreateShockMessage::SetOwnerPointer( CBase* pOwner )
{
	this->m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyShockMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyShockMessage::CDestroyShockMessage(CShock* pShock, CBase* pOwner) : CBaseMessage(MSG_DESTROY_SHOCK)
{
	this->m_pShock = pShock;
	this->m_pOwner = pOwner;
}

CDestroyShockMessage::~CDestroyShockMessage(void)
{
}

CBase* CDestroyShockMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CShock* CDestroyShockMessage::GetShockPointer(void)
{
	return this->m_pShock;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateGrenadeMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateGrenadeMessage::CCreateGrenadeMessage( CBase* pOwner ) : CBaseMessage(MSG_CREATE_GRENADE)
{	
	this->m_pOwner = pOwner;
}

CCreateGrenadeMessage::~CCreateGrenadeMessage( void )
{
}

CBase* CCreateGrenadeMessage::GetOwnerPointer( void )
{
	return this->m_pOwner;
}

void CCreateGrenadeMessage::SetOwnerPointer( CBase* pOwner )
{
	this->m_pOwner = pOwner;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyGrenadeMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyGrenadeMessage::CDestroyGrenadeMessage(CGrenade* pGrenade, CBase* pOwner) : CBaseMessage(MSG_DESTROY_GRENADE)
{
	this->m_pGrenade = pGrenade;
	this->m_pOwner = pOwner;
}

CDestroyGrenadeMessage::~CDestroyGrenadeMessage(void)
{
}

CBase* CDestroyGrenadeMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

CGrenade* CDestroyGrenadeMessage::GetGrenadePointer(void)
{
	return this->m_pGrenade;
}


///////////////////////////////////////////////////////////////////////////////
//	class "CCreateIceMessage"
///////////////////////////////////////////////////////////////////////////////
CCreateIceMessage::CCreateIceMessage( CPlayer* pPlayer, CBase* pOwner ) : CBaseMessage(MSG_CREATE_ICE)
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

CBase*	CCreateIceMessage::GetOwnerPointer(void)
{
	return this->m_pOwner;
}

void CCreateIceMessage::SetOwnerPointer(CBase* pOwner)
{
	this->m_pOwner = pOwner;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyIceMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyIceMessage::CDestroyIceMessage(CIce* pIce, CBase* pOwner) : CBaseMessage(MSG_DESTROY_ICE)
{
	this->m_pIce = pIce;
	this->m_pOwner = pOwner;
}

CDestroyIceMessage::~CDestroyIceMessage(void)
{
}

CBase* CDestroyIceMessage::GetOwnerPointer(void)
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
CCreateFireMessage::CCreateFireMessage( CPlayer* pPlayer, CBase* pOwner ) : CBaseMessage(MSG_CREATE_FIRE)
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

CBase* CCreateFireMessage::GetOwnerPointer(void) 
{
	return this->m_pOwner;
}

void CCreateFireMessage::SetOwnerPointer(CBase* pOwner)
{
	this->m_pOwner = pOwner;
}

///////////////////////////////////////////////////////////////////////////////
//	class "CDestroyFireMessage"
///////////////////////////////////////////////////////////////////////////////
CDestroyFireMessage::CDestroyFireMessage(CFire* pFire, CBase* pOwner) : CBaseMessage(MSG_DESTROY_FIRE)
{
	this->m_pFire = pFire;
	this->m_pOwner = pOwner;
}

CDestroyFireMessage::~CDestroyFireMessage(void)
{
}

CBase* CDestroyFireMessage::GetOwnerPointer(void)
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

CCreateEnemyMessage::CCreateEnemyMessage(int nEnemyType, int nPosX, int nPosY, CSpawner* pHostSpawner ) : CBaseMessage(MSG_CREATE_ENEMY)
{
	m_nEnemyType = nEnemyType;
	m_nPosX = nPosX;
	m_nPosY = nPosY;
	m_pHostSpawner = pHostSpawner;
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