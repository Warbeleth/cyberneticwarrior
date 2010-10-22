#ifndef SINGLEPLAYER_H_
#define SINGLEPLAYER_H_

#include "IGameState.h"

#include "CPlayer.h"
#include "CPickUp.h"
#include "CGrapplingHook.h"
#include "CBlock.h"

#include "CObjectFactory.h"
#include <string>
using std::string;

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CObjectManager;

class CCamera;


class CSinglePlayerState : public IGameState
{
private:

	//////////////////////////
	//***Temp***
	//////////////////////////
	CPlayer* m_TempPlayer;
	CPickUp* m_PickUp;


	CBlock*	m_TempPlatform1;
	CBlock*	m_TempPlatform2;
	//////////////////////////

	struct tProfile
	{
		bool m_bHaveHook;
	}m_Profile;
	
	bool m_bNewGame;




	tVector2D	m_tBGOffset;
	
	int			m_nBackgroundImageID;
	int			m_nCrossHairID;
	int			m_nBGMusic;
	
	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CObjectFactory<string, CBase>*			m_pOF;
	CObjectManager*							m_pOM;

	CCamera*						m_pCamera;

	CSinglePlayerState(void);
	~CSinglePlayerState(void);
	CSinglePlayerState(const CSinglePlayerState&);
	CSinglePlayerState&		operator=(const CSinglePlayerState&);


	static	CSinglePlayerState*	sm_pGamePlayInstance;

public:

	//////////////////////////
	// TEMP
	//////////////////////////
	CPlayer*	GetPlayerPointer(void);
	//////////////////////////

	tProfile*	GetProfileValues(void);
	void		SetProfileValues(bool bValue);

	inline bool		GetNewGame(void) { return this->m_bNewGame;}
	inline void		SetNewGame(bool bNewGame) {this->m_bNewGame = bNewGame;}


	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CSinglePlayerState*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif