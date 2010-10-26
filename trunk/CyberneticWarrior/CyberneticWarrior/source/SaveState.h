#ifndef	SAVESTATE_H_
#define SAVESTATE_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CEvent;

#define SMENU_SPACE 42

class CSaveState : public IGameState
{
private:

	enum selections { SAVE1 = 0, SAVE2, SAVE3, BACK, 
		SMENU_START =130, SM_CURSOR_POS = 90};

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;

	//const char*	m_nFileName;//[3];

	int	m_nBackgroundID;
	int m_nCursorID;


	int m_nSelectionPos;
	int	m_nSelection;


	bool	m_bNewGame;
	bool	m_bDelete;


	CSaveState(void);
	~CSaveState(void);
	CSaveState(const CSaveState&);
	CSaveState&	operator=(const CSaveState&);

	static CSaveState*	sm_pSaveMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	inline bool		GetNewGame(void) { return this->m_bNewGame;}
	inline void		SetNewGame(bool bNewGame) { this->m_bNewGame = bNewGame;}

	inline bool		GetDelete(void) { return this->m_bDelete;}
	inline void		SetDelete(bool bDelete) { this->m_bDelete = bDelete;}

	static CSaveState*	GetInstance(void);
	static void DeleteInstance(void);

};

#endif