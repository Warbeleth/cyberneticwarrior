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

//class CSGD_Direct3D;
//class CSGD_WaveManager;
//class CSGD_TextureManager;
//class CSGD_DirectSound;
//class CSGD_DirectInput;

class IGameState
{
private:

	/*int m_nSelected;
	int m_nFontID;

	CSGD_Direct3D*				m_pD3D;
	CSGD_WaveManager*			m_pWM;
	CSGD_TextureManager*		m_pTM;
	CSGD_DirectSound*			m_pDS;
	CSGD_DirectInput*			m_pDI;*/

public:

	virtual bool Input(void) = 0;
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;
	virtual void Enter(void) = 0;
	virtual void Exit(void) = 0;
	virtual ~IGameState(void) = 0 { };

};
#endif