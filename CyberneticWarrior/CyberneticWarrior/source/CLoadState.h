#ifndef	LOADSTATE_H_
#define LOADSTATE_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CEvent;

#define LMENU_SPACE 42

class CLoadState : public IGameState
{
private:

	enum selections { LOAD1 = 0, LOAD2, LOAD3, BACK, 
		LMENU_START =130, LM_CURSOR_POS = 90};

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;

	int	m_nBackgroundID;
	int m_nCursorID;


	int m_nSelectionPos;
	int	m_nSelection;


	bool	m_bMute;


	CLoadState(void);
	~CLoadState(void);
	CLoadState(const CLoadState&);
	CLoadState&	operator=(const CLoadState&);

	static CLoadState*	sm_pLoadMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CLoadState*	GetInstance(void);
	static void DeleteInstance(void);


};

#endif