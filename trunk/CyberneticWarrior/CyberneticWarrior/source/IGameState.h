//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : IGameState.h
//
//	Author : Patrick Alvarez 
//
//	Purpose : Parent base class declarations for all Game States.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef IGAMESTATE_H_
#define IGAMESTATE_H_
#include "SGD Wrappers/CSGD_Direct3D.h"
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_WaveManager.h"
#include "SGD Wrappers/CSGD_DirectInput.h"
#include "SGD Wrappers/CSGD_DirectSound.h"

enum StateType { MENU = 0, OPTIONS, LOADING, ATTRACT, GAMEPLAY };
class IGameState
{
protected:

	CSGD_Direct3D*				m_pD3D;
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;
	CSGD_WaveManager*			m_pWM;
	CSGD_DirectSound*			m_pDS;

	float m_fAtractModeTimer;
	float m_fAtractMode;

	int	  m_nStateType;
	
	bool m_bInput;
	void AtractMode( float fElapsedTime );
public:
	virtual bool Input(void) = 0;
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;
	virtual void Enter(void) = 0;
	virtual void Exit(void) = 0;
	inline virtual ~IGameState(void) = 0 { };	

	int		GetType(void) const;
	void	SetType(int nStateType);

};
#endif