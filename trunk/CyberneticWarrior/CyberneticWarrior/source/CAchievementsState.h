//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CAchievementState.h”
//
// Author: Anthony Muccio(AM)
//
// Purpose: This header holds the CAchievementsState class. This class' purpose is to render an in
//			game achievement list that scrolls to the screen in a menu format.
//////////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//				 ******************* CLASS - CAchievementsState *******************
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CAchievementsState : public IGameState
{
private:


	// Font
	CFont	m_OptionsFont;

	float	m_fWaitTime;
		
	int		m_nScrollingID;
	// Texture IDs
	int	m_nBackgroundID;
	// Selection IDs
	int m_nSelectionPos;
	int	m_nSelection;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CAchievementsState”
	//
	// Purpose: The default constructor for CAchievementsState. Will initialize values.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CAchievementsState(void);
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “~CAchievementsState”
	//
	// Purpose: The destructor for CAchievementsState. Will cleanup.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	~CAchievementsState(void);
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CAchievementsState”
	//
	// Purpose: The copy constructor for CAchievementsState.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CAchievementsState(const CAchievementsState&);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “CAchievementsState& operator=”
	//
	// Purpose: The assignment operator for CAchievementsState.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	CAchievementsState& operator=(const CAchievementsState&);

	static CAchievementsState* sm_pAchievementsInstance;
public:
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Enter”
	//
	// Purpose: Enter essentially does what the default constructor will do, however it doesn't "need" to
	//			initialize all values. Enter only has to initialize the values that reset everytime the
	//			user enters this state.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void	Enter(void);
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Input”
	//
	// Purpose: Reads the current input of the user and performs actions based on whether what was input
	//			was being looked for or not.
	//			RETURN - true for this state, at all times
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool	Input(void);
		
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Update”
	//
	// Purpose: Update takes care of any increment over time variables as well as anything that constantly
	//			needs to be recalculated.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void	Update(float fElapsedTime);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Render”
	//
	// Purpose: Draws to the screen.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void	Render(void);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “Exit”
	//
	// Purpose: Cleans up any pointers or memory as the state exits.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	void	Exit(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “GetInstance”
	//
	// Purpose: Will return a pointer to the instance of this state and will also initialize the state if
	//			it has not already been initialized.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static CAchievementsState*	GetInstance(void);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: “DeleteInstance”
	//
	// Purpose: Will remove the instance of the state from memory.
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static void DeleteInstance(void);
};

#endif