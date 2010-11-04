//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CMapLoad.h”
//
// Author: Corey Ringer (CR) / Animation Functionality - Anthony Muccio (AM)
//
// Purpose: This class serves as the in-game tile-set as well as an engine loader.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef MAPLOADER_H_
#define MAPLOADER_H_
#include <fstream>
#include <vector>
#include <string>
#include "CCamera.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"
#include "CObjectFactory.h"
#include "CObjectManager.h"
#include "CSpawner.h"
#include "CBlock.h"
#include "CBase.h"
#include "CAnimationLoad.h"

//Enemies
#include "CBaseEnemy.h"
//Base types
#include "CIdleEnemy.h"
#include "CPatrolEnemy.h"
//Special
#include "CFLCLMech.h"

namespace SGP_Map_Editor
{
	enum Enemies { Turret_Gun = 0, Turret_Frost, Turret_Fire, Turret_Multi,
		Drone_Attack, Drone_Seeker, Drone_Heavy,
		Ground_Mech, Ground_Siege, Ground_FLCL,
		Boss_Apple, Boss_Pimp, Boss_Pirate, Enemies_Total      
	};

	enum Spawners { Turret_Spawner = 0,
		Drone_Spawner,
		Ground_Spawner,
		Boss_Spawner
	};

	enum Collisions { Coll_Solid = 0,
		Coll_Partial,
		Coll_Moving,
		Coll_Unstable,
		Coll_Trap
	};

	struct Scales
	{
		float m_fGridWidth;
		float m_fGridHeight;
		float m_fSelectionWidth;
		float m_fSelectionHeight;
	};

	struct Grid
	{
		int m_nColumns;
		int m_nRows;
		int m_nWidth;
		int m_nHeight;
	};

	struct Selection
	{
		int m_nColumns;
		int m_nRows;
		int m_nWidth;
		int m_nHeight;
		int m_nSelectedIndex;
		int m_nImageID;
	};

	struct Enemy
	{
		int m_nType;
		int m_nPosX;
		int m_nPosY;
	};

	struct Spawner
	{
		int m_nType;
		int m_nPosX;
		int m_nPosY;
	};

	struct Collision
	{
		int m_nType;
		RECT m_rWorldPos;
	};

	struct ListObject
	{
		Enemy m_eEnemy;
		Spawner m_sSpawner;
		Collision m_cCollision;
		bool m_bEnemy;
		bool m_bSpawner;
		bool m_bCollision;
		int m_nDrawImage;
	};
}


using namespace SGP_Map_Editor;
using namespace std;

//#define BOXTYPES

class CMapLoad
{
private:
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Trilogy of Evil
	//
	//	Purpose : Protection to prevent calling of unused functions for this singleton
	////////////////////////////////////////////////////////////////////////////////////
	CMapLoad(){};
	CMapLoad(CMapLoad &copy);
	CMapLoad& operator=(CMapLoad &assign);
	~CMapLoad(void){};

public:
	struct EnemyImageIDs
	{
		int TurretGun, 
			TurretFrost, 
			TurretFire, 
			TurretMulti,
			DroneAttack, 
			DroneSeeker, 
			DroneHeavy,
			GroundMech, 
			GroundSiege,
			GroundFLCL,
			BossApple,
			BossPimp,
			BossPirate;
	}m_sEnemyImage;

	Grid m_gTileMap;
	Selection m_gSelectionMap;
	vector<ListObject> m_lMap;
	string m_szFileName;
	string m_szWorkingDirectoryGRAPHICS;
	string m_szWorkingDirectoryMAPS;
	string m_szMap_01;
	
	bool m_bCollisionCheck;

	float m_fScaleX;
	float m_fScaleY;

	CAnimations m_Animations[12];

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : GetInstance()
	//
	//	Purpose : Gets the instance to the map loader.
	////////////////////////////////////////////////////////////////////////////////////
	static CMapLoad *GetInstance();

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : LoadMap()
	//
	//	Purpose : Loads a map from the designated file.
	////////////////////////////////////////////////////////////////////////////////////
	bool LoadMap(const char* szFilename);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : LoadMapImage()
	//
	//	Purpose : Loads the designated image file that is imported during the LoadMap 
	//			function and cleans up any old image IDs if any.
	////////////////////////////////////////////////////////////////////////////////////
	bool LoadMapImage(const char* szFilename);

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : LoadAnimations()
	//
	//	Purpose : Loads all 12 enemy animations for assignment.
	////////////////////////////////////////////////////////////////////////////////////
	bool LoadAnimations( void );

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : CreateAnimation()
	//
	//	Purpose : Returns a copy of any 12 loaded enemy animations.
	////////////////////////////////////////////////////////////////////////////////////
	CAnimations* CreateAnimation( int nAnimationNumber );

	////////////////////////////////////////////////////////////////////////////////////
	//	Function : Render()
	//
	//	Purpose : Loads the designated image file that is imported during the LoadMap 
	//			function and cleans up any old image IDs if any.
	////////////////////////////////////////////////////////////////////////////////////
	void Render();

};
#endif