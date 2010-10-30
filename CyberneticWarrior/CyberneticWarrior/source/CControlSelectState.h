#ifndef CONTROLSELECT_H_
#define CONTROLSELECT_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CControlSelectState : public IGameState
{
private:

	enum selection{ KEYBOARD = 0, GAMEPAD, EXIT_MENU };


	CFont		m_ControlSelFont;

	int			m_nBackgroundID;
	int			m_nKeys;
	int			m_nGamePad;
	
	int			m_nSelection;
	
	
	CControlSelectState(void);
	~CControlSelectState(void);
	CControlSelectState(const CControlSelectState&);
	CControlSelectState&	operator=(const CControlSelectState&);

	static CControlSelectState*	sm_pControlSelectInstance;	

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CControlSelectState*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif