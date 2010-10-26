#include "CStackStateMachine.h"

#include "IGameState.h"
#include "CAtractModeState.h"

void IGameState::AtractMode( float fElapsedTime )
{
	m_fAtractModeTimer += fElapsedTime;

	if(m_bInput)
	{
		m_fAtractModeTimer = 0.0f;
		return;
	}

	if( m_fAtractModeTimer >= m_fAtractMode )
	{
		CStackStateMachine::GetInstance()->Push_Back(CAtractModeState::GetInstance());
		m_fAtractModeTimer = 0.0f;
	}
}