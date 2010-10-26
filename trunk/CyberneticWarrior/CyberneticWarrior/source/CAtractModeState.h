#ifndef _CATRACTMODESTATE_H_
#define _CATRACTMODESTATE_H_

#include "IGameState.h"

class CAtractModeState : public IGameState
{
	float m_fTotalTime;
	int m_nCurrentImage;
	// Image Ids
	int m_nImageIds[5];
	// Sound ID
	int m_nBGMusicId;

	CAtractModeState( void );
	~CAtractModeState( void );
	CAtractModeState(CAtractModeState&);
	CAtractModeState& operator=(CAtractModeState&);

	static CAtractModeState* sm_pAtractModeInstance;
public:
	bool Input( void );
	void Update( float fElapsedTime );
	void Render( void );
	void Enter( void );
	void Exit( void );

	static CAtractModeState* GetInstance( void );
	static void DeleteInstance( void );
};

#endif