#ifndef CONTROLSELECT_H_
#define CONTROLSELECT_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CEvent;

class CControlSelectState : public IGameState
{
private:

	enum selection{ KEYBOARD = 0, GAMEPAD, EXIT_MENU };

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

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