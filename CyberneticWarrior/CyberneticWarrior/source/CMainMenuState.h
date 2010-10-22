#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "IGameState.h"
#include "IListener.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CEvent;

#define MMENU_SPACE 37

class CMainMenuState : public IGameState//, public IListener
{
private:

	enum selections { SINGLE_PLAYER = 0, MULTI_PLAYER, LEADER_BOARDS,
		  ACHIEVEMENTS, MM_OPTIONS, MM_CONTROLS, CREDITS, EXIT_GAME, 
		  MMENU_START = 130, MMCURSOR_POS = 90};

	enum events { CURSOR_UP, CURSOR_DOWN, ENTER };

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	
	int		m_nBackgroundID;
	int		m_nMenuID;
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

	//void	HandleEvent(CEvent*	pEvent);

	static CMainMenuState*	GetInstance(void);
	static void	DeleteInstance(void);

};
#endif