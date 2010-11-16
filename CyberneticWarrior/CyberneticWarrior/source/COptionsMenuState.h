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

#define OMENU_SPACE 75

class COptionsMenuState : public IGameState
{
private:

	enum selections { MUSIC_VOLUME = 0, SFX_VOLUME, MUTE, FULL_SCREEN, CONTROL_SELECT, EXIT_OMENU,
		OMENU_START = 150, OM_CURSOR_POS = 130, MENUX = 125};

	CFont	m_OptionsFont;
	
	float m_fWaitTime;
	
	// Texture IDs
	int	m_nBackgroundID;
	int m_nCursorID;
	int m_nMuteOnID;
	int m_nMuteOffID;
	
	// Music IDs
	int m_nMusicID;
	int m_nSFXID;

	// Selection IDs
	int m_nSelectionPos;
	int	m_nSelection;

	// Game Volume Levels
	int		m_nMusicVolume;
	int		m_nSFXVolume;
	bool	m_bMute;

	// Selection On
	bool	m_bSelection;
	bool	m_bPlayMusic;
	bool	m_bPlaySFX;


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