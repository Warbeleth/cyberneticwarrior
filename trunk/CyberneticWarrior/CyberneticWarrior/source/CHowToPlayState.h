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

#define MENU_SPACE 75

class CHowToPlayState : public IGameState
{
private:

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

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