#ifndef	SINGLEPLAYERMENU_H_
#define SINGLEPLAYERMENU_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

#define SPMENU_SPACE 42

class CSinglePlayerMenuState : public IGameState
{
private:

	enum selections { NEW_GAME = 0, LOAD, DELETE_PROF, BACK,
		SMENU_START =130, SM_CURSOR_POS = 110};

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;

	float m_fWaitTime;

	int	m_nBackgroundID;
	int m_nCursorID;


	int m_nSelectionPos;
	int	m_nSelection;


	bool	m_bMute;


	CSinglePlayerMenuState(void);
	~CSinglePlayerMenuState(void);
	CSinglePlayerMenuState(const CSinglePlayerMenuState&);
	CSinglePlayerMenuState&	operator=(const CSinglePlayerMenuState&);

	static CSinglePlayerMenuState*	sm_pSinglePlayerMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CSinglePlayerMenuState*	GetInstance(void);
	static void DeleteInstance(void);

	int	GetMusicVolume(void);
	void SetMusicVolume(int nMusicVolume);

	int GetSFXVolume(void);
	void SetSFXVolume(int nSFXVolume);

	bool GetMute(void);
	void SetMute(bool bMute);

};

#endif