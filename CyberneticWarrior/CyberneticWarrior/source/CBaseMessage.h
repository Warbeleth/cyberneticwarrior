#ifndef	BASEMESSAGE_H_
#define BASEMESSAGE_H_

class CGrapplingHook;
class CBaseEnemy;
class CPlayer;
class CRocket;
class CBullet;
class CFlame;
class CPlasma;
class CShock;
class CGrenade;
class CIce;
class CFire;
class CBlock;

typedef  int MSGID;

/*unsigned char*/
enum eMsgTypes	{ MSG_NULL = 0, 
					MSG_CREATE_HOOK, MSG_DESTROY_HOOK,
					MSG_CREATE_ROCKET, MSG_DESTROY_ROCKET,
					MSG_CREATE_BULLET, MSG_DESTROY_BULLET, 
					MSG_CREATE_FLAME, MSG_DESTROY_FLAME,
					MSG_CREATE_GRENADE, MSG_DESTROY_GRENADE,
					MSG_CREATE_PLASMA, MSG_DESTROY_PLASMA,
					MSG_CREATE_SHOCK, MSG_DESTROY_SHOCK,
					MSG_CREATE_FIRE, MSG_DESTROY_FIRE,
					MSG_CREATE_ICE, MSG_DESTROY_ICE,
					MSG_DESTROY_BLOCK,
					/*MSG_CREATE_PICKUP,*/ MSG_MAX };

class CBaseMessage
{
private:

	MSGID m_nMsgID;

public:

	CBaseMessage(MSGID msgID);
	inline virtual ~CBaseMessage(void)	{}

	inline MSGID GetMsgID(void)	{ return this->m_nMsgID; }
	
};

class CCreateHookMessage : public CBaseMessage
{
private:

	CPlayer*	m_pPlayer;

public:
	CCreateHookMessage(CPlayer*	pPlayer);
	~CCreateHookMessage(void);

	CPlayer*	GetPlayerPointer(void);
	void	SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyHookMessage : public CBaseMessage
{
private:
	CGrapplingHook*	m_pHook;
	CPlayer*		m_pPlayer;
public:
	CDestroyHookMessage(CGrapplingHook*	pHook, CPlayer* pPlayer);
	~CDestroyHookMessage(void);
	CPlayer*	GetPlayerPointer(void);
	CGrapplingHook*	GetHookPointer(void);
};

class CCreateRocketMessage : public CBaseMessage
{
private:
	CPlayer*	m_pPlayer;
public:
	CCreateRocketMessage( CPlayer* pPlayer );
	~CCreateRocketMessage( void );

	CPlayer* GetPlayerPointer( void );
	void SetPlayerPointer( CPlayer* pPlayer );
};

class CDestroyRocketMessage : public CBaseMessage
{
private:
	CRocket*		m_pRocket;
	CPlayer*		m_pPlayer;
public:
	CDestroyRocketMessage(CRocket* pRocket, CPlayer* pPlayer);
	~CDestroyRocketMessage(void);
	CPlayer* GetPlayerPointer(void);
	CRocket* GetRocketPointer(void);
};

class CCreateBulletMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
public:
	CCreateBulletMessage(CPlayer* pPlayer);
	~CCreateBulletMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyBulletMessage : public CBaseMessage
{
private:
	CBullet*		m_pBullet;
	CPlayer*		m_pPlayer;
public:
	CDestroyBulletMessage(CBullet* pBullet, CPlayer* pPlayer);
	~CDestroyBulletMessage(void);
	CPlayer*	GetPlayerPointer(void);
	CBullet*	GetBulletPointer(void);
};

class CCreateFlameMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
public:
	CCreateFlameMessage(CPlayer* pPlayer);
	~CCreateFlameMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyFlameMessage : public CBaseMessage
{
private:
	CFlame*			m_pFlame;
	CPlayer*		m_pPlayer;
public:
	CDestroyFlameMessage(CFlame* pFlame, CPlayer* pPlayer);
	~CDestroyFlameMessage(void);
	CPlayer*	GetPlayerPointer(void);
	CFlame*		GetFlamePointer(void);
};

class CCreatePlasmaMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
public:
	CCreatePlasmaMessage(CPlayer* pPlayer);
	~CCreatePlasmaMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyPlasmaMessage : public CBaseMessage
{
private:
	CPlasma*			m_pPlasma;
	CPlayer*			m_pPlayer;
public:
	CDestroyPlasmaMessage(CPlasma* pPlasma, CPlayer* pPlayer);
	~CDestroyPlasmaMessage(void);
	CPlayer*		GetPlayerPointer(void);
	CPlasma*		GetPlasmaPointer(void);
};

class CCreateShockMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
public:
	CCreateShockMessage(CPlayer* pPlayer);
	~CCreateShockMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyShockMessage : public CBaseMessage
{
private:
	CShock*				m_pShock;
	CPlayer*			m_pPlayer;
public:
	CDestroyShockMessage(CShock* pShock, CPlayer* pPlayer);
	~CDestroyShockMessage(void);
	CPlayer*		GetPlayerPointer(void);
	CShock*			GetShockPointer(void);
};

class CCreateGrenadeMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
public:
	CCreateGrenadeMessage(CPlayer* pPlayer);
	~CCreateGrenadeMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);
};

class CDestroyGrenadeMessage : public CBaseMessage
{
private:
	CGrenade*			m_pGrenade;
	CPlayer*			m_pPlayer;
public:
	CDestroyGrenadeMessage(CGrenade* pGrenade, CPlayer* pPlayer);
	~CDestroyGrenadeMessage(void);
	CPlayer*		GetPlayerPointer(void);
	CGrenade*		GetGrenadePointer(void);
};

class CCreateIceMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
	CBaseEnemy*			m_pOwner;
public:
	CCreateIceMessage(CPlayer* pPlayer, CBaseEnemy* pOwner);
	~CCreateIceMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);

	CBaseEnemy* GetOwnerPointer(void);
	void SetOwnerPointer(CBaseEnemy* pEnemy);
};

class CDestroyIceMessage : public CBaseMessage
{
private:
	CIce*				m_pIce;
	CBaseEnemy*			m_pOwner;
public:
	CDestroyIceMessage(CIce* pIce, CBaseEnemy* pOwner);
	~CDestroyIceMessage(void);
	CBaseEnemy*		GetOwnerPointer(void);
	CIce*			GetIcePointer(void);
};

class CCreateFireMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
	CBaseEnemy*			m_pOwner;
public:
	CCreateFireMessage(CPlayer* pPlayer, CBaseEnemy* pOwner);
	~CCreateFireMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);

	CBaseEnemy* GetOwnerPointer(void);
	void SetOwnerPointer(CBaseEnemy* pOwner);
};

class CDestroyFireMessage : public CBaseMessage
{
private:
	CFire*				m_pFire;
	CBaseEnemy*			m_pOwner;
public:
	CDestroyFireMessage(CFire* pFire, CBaseEnemy* pOwner);
	~CDestroyFireMessage(void);
	CBaseEnemy*		GetOwnerPointer(void);
	CFire*			GetFirePointer(void);
};


class CDestroyBlockMessage : public CBaseMessage
{
private:
	CBlock* m_pBlock;
public:
	CDestroyBlockMessage(CBlock* pBlock);
	~CDestroyBlockMessage(void);

	CBlock*	GetBlockPointer(void) { return m_pBlock; }
};

/*	---Message Class Example---
class CCreateEnemyMessage : public CBaseMessage
{
private:
	CEnemy* m_pEnemy;
public:
	CCreateEnemyMessage(CEnemy*	pEnemy);
	~CCreateEnemyMessage(void);

	CEnemy*	GetEnemyPointer(void) { return m_pEnemy; }
};
*/
#endif