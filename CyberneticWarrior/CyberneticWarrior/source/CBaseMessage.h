#ifndef	BASEMESSAGE_H_
#define BASEMESSAGE_H_

class CGrapplingHook;
class CBase;
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
					MSG_CREATE_ENEMY, MSG_DESTROY_ENEMY,
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
	CBase*	m_pOwner;
public:
	CCreateRocketMessage( CBase* pOwner );
	~CCreateRocketMessage( void );

	CBase* GetOwnerPointer( void );
	void SetOwnerPointer( CBase* pOwner );
};

class CDestroyRocketMessage : public CBaseMessage
{
private:
	CRocket*		m_pRocket;
	CBase*		m_pOwner;
public:
	CDestroyRocketMessage(CRocket* pRocket, CBase* pOwner);
	~CDestroyRocketMessage(void);
	CBase* GetOwnerPointer(void);
	CRocket* GetRocketPointer(void);
};

class CCreateBulletMessage : public CBaseMessage
{
private:
	CBase*		m_pOwner;
public:
	CCreateBulletMessage(CBase* pOwner);
	~CCreateBulletMessage(void);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyBulletMessage : public CBaseMessage
{
private:
	CBullet*		m_pBullet;
	CBase*			m_pOwner;
public:
	CDestroyBulletMessage(CBullet* pBullet, CBase* pOwner);
	~CDestroyBulletMessage(void);
	CBase*	GetOwnerPointer(void);
	CBullet*	GetBulletPointer(void);
};

class CCreateFlameMessage : public CBaseMessage
{
private:
	CBase*		m_pOwner;
public:
	CCreateFlameMessage(CBase* pOwner);
	~CCreateFlameMessage(void);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyFlameMessage : public CBaseMessage
{
private:
	CFlame*			m_pFlame;
	CBase*		m_pOwner;
public:
	CDestroyFlameMessage(CFlame* pFlame, CBase* pOwner);
	~CDestroyFlameMessage(void);
	CBase*	GetOwnerPointer(void);
	CFlame*		GetFlamePointer(void);
};

class CCreatePlasmaMessage : public CBaseMessage
{
private:
	CBase*		m_pOwner;
public:
	CCreatePlasmaMessage(CBase* pOwner);
	~CCreatePlasmaMessage(void);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyPlasmaMessage : public CBaseMessage
{
private:
	CPlasma*			m_pPlasma;
	CBase*				m_pOwner;
public:
	CDestroyPlasmaMessage(CPlasma* pPlasma, CBase* pOwner);
	~CDestroyPlasmaMessage(void);
	CBase*			GetOwnerPointer(void);
	CPlasma*		GetPlasmaPointer(void);
};

class CCreateShockMessage : public CBaseMessage
{
private:
	CBase*		m_pOwner;
public:
	CCreateShockMessage(CBase* pOwner);
	~CCreateShockMessage(void);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyShockMessage : public CBaseMessage
{
private:
	CShock*				m_pShock;
	CBase*				m_pOwner;
public:
	CDestroyShockMessage(CShock* pShock, CBase* pOwner);
	~CDestroyShockMessage(void);
	CBase*			GetOwnerPointer(void);
	CShock*			GetShockPointer(void);
};

class CCreateGrenadeMessage : public CBaseMessage
{
private:
	CBase*		m_pOwner;
public:
	CCreateGrenadeMessage(CBase* pOwner);
	~CCreateGrenadeMessage(void);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyGrenadeMessage : public CBaseMessage
{
private:
	CGrenade*			m_pGrenade;
	CBase*			m_pOwner;
public:
	CDestroyGrenadeMessage(CGrenade* pGrenade, CBase* pOwner);
	~CDestroyGrenadeMessage(void);
	CBase*		GetOwnerPointer(void);
	CGrenade*		GetGrenadePointer(void);
};

class CCreateIceMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
	CBase*			m_pOwner;
public:
	CCreateIceMessage(CPlayer* pPlayer, CBase* pOwner);
	~CCreateIceMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyIceMessage : public CBaseMessage
{
private:
	CIce*				m_pIce;
	CBase*			m_pOwner;
public:
	CDestroyIceMessage(CIce* pIce, CBase* pOwner);
	~CDestroyIceMessage(void);
	CBase*		GetOwnerPointer(void);
	CIce*			GetIcePointer(void);
};

class CCreateFireMessage : public CBaseMessage
{
private:
	CPlayer*		m_pPlayer;
	CBase*			m_pOwner;
public:
	CCreateFireMessage(CPlayer* pPlayer, CBase* pOwner);
	~CCreateFireMessage(void);

	CPlayer* GetPlayerPointer(void);
	void SetPlayerPointer(CPlayer* pPlayer);

	CBase* GetOwnerPointer(void);
	void SetOwnerPointer(CBase* pOwner);
};

class CDestroyFireMessage : public CBaseMessage
{
private:
	CFire*				m_pFire;
	CBase*			m_pOwner;
public:
	CDestroyFireMessage(CFire* pFire, CBase* pOwner);
	~CDestroyFireMessage(void);
	CBase*			GetOwnerPointer(void);
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

class CDestroyEnemyMessage : public CBaseMessage
{
private:
	CBaseEnemy* m_pEnemy;
public:
	CDestroyEnemyMessage(CBaseEnemy* pEnemy);
	~CDestroyEnemyMessage(void);

	CBaseEnemy*	GetEnemyPointer(void) { return m_pEnemy; }
};

class CCreateEnemyMessage : public CBaseMessage
{
private:
	int m_nEnemyType;
	int m_nPosX;
	int m_nPosY;
public:
	CCreateEnemyMessage(int nEnemyType, int nPosX, int nPosY);
	~CCreateEnemyMessage(void);

	int GetEnemyType(void) { return m_nEnemyType; }
	int GetPosX(void) { return m_nPosX; }
	int GetPosY(void) { return m_nPosY; }
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