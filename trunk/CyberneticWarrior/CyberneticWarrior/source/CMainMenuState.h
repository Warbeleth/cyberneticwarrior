#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CEvent;

#define MMENU_SPACE 50

class CMainMenuState : public IGameState
{
private:

	enum selections { SINGLE_PLAYER = 1, MM_OPTIONS, MM_CONTROLS, CREDITS, SPACE, EXIT_GAME, 
		  MMENU_START = 130, MMCURSOR_POS = 90};


	CFont	m_MenuFont;
	
	float	m_fWaitTime;

	int		m_nBackgroundID;
	int		m_nCursorID;
	
	int		m_nSelectionPos;
	int		m_nSelection;

	int		m_nBGMusic;


	CMainMenuState(void);
	~CMainMenuState(void);
	CMainMenuState(const CMainMenuState&);
	CMainMenuState&		operator=(const CMainMenuState&);

	static CMainMenuState*	sm_pMainMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);


	static CMainMenuState*	GetInstance(void);
	static void	DeleteInstance(void);

};
#endif