#include "IGameState.h"
#include "CStackStateMachine.h"
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

int	IGameState::GetType(void) const	{return this->m_nStateType;}
void IGameState::SetType(int nStateType) {this->m_nStateType = nStateType;}