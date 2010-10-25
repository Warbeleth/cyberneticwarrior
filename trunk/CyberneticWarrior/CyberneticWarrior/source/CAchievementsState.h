#ifndef _CACHIEVEMENTSSTATE_H_
#define _CACHIEVEMENTSSTATE_H_

#include "IGameState.h"
#include "CFont.h"
#define MENU_SPACE 75

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;
class CEvent;

class CAchievementsState : public IGameState
{
private:
	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont	m_OptionsFont;
	int		m_nScrollingID;
	
	// Texture IDs
	int	m_nBackgroundID;
	// Selection IDs
	int m_nSelectionPos;
	int	m_nSelection;

	CAchievementsState(void);
	~CAchievementsState(void);
	CAchievementsState(const CAchievementsState&);
	CAchievementsState& operator=(const CAchievementsState&);

	static CAchievementsState* sm_pAchievementsInstance;
public:
	void	Enter(void);
	bool	Input(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CAchievementsState*	GetInstance(void);
	static void DeleteInstance(void);
};

#endif