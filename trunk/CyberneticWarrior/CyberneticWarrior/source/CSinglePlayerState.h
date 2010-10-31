#ifndef SINGLEPLAYER_H_
#define SINGLEPLAYER_H_

#include "IGameState.h"
#include "SGD Wrappers/SGD_Math.h"
#include "CAnimationLoad.h"
#include "CObjectFactory.h"
#include <string>
using std::string;

class CPlayer;
class CPickUp;
class CGrapplingHook;
class CBlock;
class CMapLoad;
class CBase;
class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CObjectManager;
class CCamera;
class CAnimations;
class CIdleEnemy;
class CPatrolEnemy;
class CFLCLMech;

class CSinglePlayerState : public IGameState
{
private:

	enum Input {CKEYBOARD = 0, CGAMEPAD = 1};

	//////////////////////////
	//***Temp***
	//////////////////////////
	CPlayer* m_TempPlayer;
	CPickUp* m_PickUp;
	CMapLoad* m_TempMap;

	CIdleEnemy* Enemy_1;
	CPatrolEnemy* Enemy_2;
	CFLCLMech* Enemy_3;

	CBlock*	m_TempPlatform1;
	CBlock*	m_TempPlatform2;
	//////////////////////////

	struct tProfile
	{
		bool m_bHaveHook;
	}m_Profile;
	
	bool m_bNewGame;
	bool m_bMusic;
	bool m_bInput;


	int			m_nMusicVolume;
	int			m_nSFXVolume;

	tVector2D	m_tBGOffset;
	
	int			m_nBackgroundImageID;
	int			m_nCrossHairID;
	int			m_nBGMusic;

	int			m_nRocketID;
	int			m_nFlameID;
	int			m_nPlasmaID;
	int			m_nShockID;
	int			m_nGrenadeID;
	int			m_nIceID;
	int			m_nFireID;
	

	CObjectFactory<string, CBase>*			m_pOF;
	CObjectManager*							m_pOM;

	CCamera*						m_pCamera;

	CAnimations m_nAnimation;

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

	inline int GetRocketID( void ) { return this->m_nRocketID; }
	inline int GetFlameID( void ) { return this->m_nFlameID; }
	inline int GetPlasmaID( void ) { return this->m_nPlasmaID; }
	inline int GetShockID( void ) { return this->m_nShockID; }
	inline int GetGrenadeID( void ) { return this->m_nGrenadeID; }
	inline int GetIceID( void ) { return this->m_nIceID; }
	inline int GetFireID( void ) { return this->m_nFireID; }
	
	
	
	void		SetProfileValues(bool bValue);

	inline bool		GetNewGame(void) { return this->m_bNewGame;}
	inline void		SetNewGame(bool bNewGame) {this->m_bNewGame = bNewGame;}
	inline bool		GetInputType(void) { return this->m_bInput;}
	inline void		SetInputType(bool bInput) {this->m_bInput = bInput;}


	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CSinglePlayerState*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif