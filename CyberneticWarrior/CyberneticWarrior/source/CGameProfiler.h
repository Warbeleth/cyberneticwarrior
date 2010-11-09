#ifndef	GAMEPROFILER_H_
#define GAMEPROFILER_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CEvent;

#define GMENU_SPACE 125
	enum FileOptions {SAVE_GAME = 0, LOAD_GAME, DELETE_PROFILE};

class CGameProfiler : public IGameState
{
private:

	enum selections {OP1 = 0, OP2, OP3, BACK, 
		MENU_START = 110, CURSOR_POS = 90};


	CFont	m_OptionsFont;

	const char*	m_nFileName[3];

	int	m_nBackgroundID;
	int m_nProfileItemID;


	int m_nSelectionPos;
	int	m_nSelection;

	short	m_sManagement;
	
	bool	m_bNewGame;

	CGameProfiler(void);
	~CGameProfiler(void);
	CGameProfiler(const CGameProfiler&);
	CGameProfiler&	operator=(const CGameProfiler&);

	static CGameProfiler*	sm_pGameProfilerInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	inline bool		GetNewGame(void) { return this->m_bNewGame;}
	inline void		SetNewGame(bool bNewGame) { this->m_bNewGame = bNewGame;}

	inline short		GetManagement(void) { return this->m_sManagement;}
	inline void		SetManagement(short sManagement) { this->m_sManagement = sManagement;}

	static CGameProfiler*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif