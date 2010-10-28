#ifndef LOADING_H_
#define LOADING_H_

#include "IGameState.h"
#include "CFont.h"

class CSGD_Direct3D;
class CSGD_DirectInput;
class CSGD_TextureManager;
class CSGD_WaveManager;
class CSGD_DirectSound;

class CLoadingState : public IGameState
{
private:

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	CFont						m_nLoadFont;

	float						m_fRotationRate;
	float						m_fRotateTime;
	float						m_fRotation;

	int							m_nBackgroundID;
	int							m_nLoadingID;

	bool						m_bReady;

	CLoadingState(void);
	~CLoadingState(void);
	CLoadingState(const CLoadingState&);
	CLoadingState& operator=(const CLoadingState&);

	static CLoadingState*	sm_pLoadingInstance;

public:

	bool	GetReady(void);
	void	SetReady(bool bReady);

	bool	Input(void);
	void	Enter(void);
	void	Update(float fElapsedTime);
	void	Render(void);
	void	Exit(void);

	static CLoadingState*	GetInstance(void);
	static void	DeleteInstance(void);
};
#endif