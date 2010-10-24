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

class IGameState
{
public:
	virtual bool Input(void) = 0;
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render(void) = 0;
	virtual void Enter(void) = 0;
	virtual void Exit(void) = 0;
	virtual ~IGameState(void) = 0 { };

};
#endif