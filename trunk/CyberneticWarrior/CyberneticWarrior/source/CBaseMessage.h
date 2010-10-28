#ifndef	BASEMESSAGE_H_
#define BASEMESSAGE_H_

class CGrapplingHook;
class CPlayer;
class CRocket;

typedef  int MSGID;

/*unsigned char*/
enum eMsgTypes	{ MSG_NULL = 0, MSG_CREATE_HOOK, MSG_DESTROY_HOOK, MSG_CREATE_ROCKET, MSG_DESTROY_ROCKET,/*MSG_CREATE_PICKUP,*/ MSG_MAX };

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