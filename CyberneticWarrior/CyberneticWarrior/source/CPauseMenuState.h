#ifndef PAUSEMENU_H_
#define PAUSEMENU_H_


#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CEvent;

#define PMENU_SPACE 42

class CPauseMenuState : public IGameState
{
private:

	enum selections { RESUME = 0, SAVE, CONTROLS, ACHIEVEMENTS, OPTIONS, MAIN_MENU,
		PMENU_START =200, PM_CURSOR_POS = 90};

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;

	float m_fWaitTime;

	int	m_nBackgroundID;
	int m_nCursorID;

	/*int m_nMusicID;
	int m_nSFXID;*/

	int m_nSelectionPos;
	int	m_nSelection;


	int		m_nMusicVolume;
	int		m_nSFXVolume;
	bool	m_bMute;


	CPauseMenuState(void);
	~CPauseMenuState(void);
	CPauseMenuState(const CPauseMenuState&);
	CPauseMenuState&	operator=(const CPauseMenuState&);

	static CPauseMenuState*	sm_pPauseMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CPauseMenuState*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif