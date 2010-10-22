#ifndef	OPTIONSMENU_H_
#define OPTIONSMENU_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CEvent;

#define OMENU_SPACE 42

class COptionsMenuState : public IGameState
{
private:

	enum selections { MUSIC_VOLUME = 0, SFX_VOLUME, MUTE, EXIT_OMENU,
		OMENU_START =130, OM_CURSOR_POS = 90};

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;

	int	m_nBackgroundID;
	int m_nCursorID;

	int m_nMusicID;
	int m_nSFXID;

	int m_nSelectionPos;
	int	m_nSelection;


	int		m_nMusicVolume;
	int		m_nSFXVolume;
	bool	m_bMute;


	COptionsMenuState(void);
	~COptionsMenuState(void);
	COptionsMenuState(const COptionsMenuState&);
	COptionsMenuState&	operator=(const COptionsMenuState&);

	static COptionsMenuState*	sm_pOptionsMenuInstance;

public:

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static COptionsMenuState*	GetInstance(void);
	static void DeleteInstance(void);

	int	GetMusicVolume(void);
	void SetMusicVolume(int nMusicVolume);

	int GetSFXVolume(void);
	void SetSFXVolume(int nSFXVolume);

	bool GetMute(void);
	void SetMute(bool bMute);

};

#endif