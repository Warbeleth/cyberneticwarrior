#ifndef	HOWTOMENU_H_
#define HOWTOMENU_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CEvent;

class CHowToPlayState : public IGameState
{
private:
	enum Height { JUMP = 0, LEFT, RIGHT, FIRE_WEAPON, SECONDARY_FIRE, AIM_WEAPON, SWAP_WEAPON, HEAD_SLOT, BOOT_SLOT, CLIMB, REPEL, MENU_SPACE = 25, MMENU_START = 150 };
	CFont	m_OptionsFont;
	
	// Texture IDs
	int	m_nBackgroundID;
	// Selection IDs
	int m_nSelectionPos;
	int	m_nSelection;

	CHowToPlayState(void);
	~CHowToPlayState(void);
	CHowToPlayState(const CHowToPlayState&);
	CHowToPlayState& operator=(const CHowToPlayState&);

	static CHowToPlayState* sm_pHowToPlayInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CHowToPlayState*	GetInstance(void);
	static void DeleteInstance(void);

};

#endif