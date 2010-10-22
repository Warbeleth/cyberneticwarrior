#ifndef MAPLOADER_H_
#define MAPLOADER_H_
#include <fstream>
#include <vector>
#include <string>
#include "CCamera.h"
#include "SGD Wrappers\CSGD_TextureManager.h"
#include "SGD Wrappers\CSGD_Direct3D.h"

namespace SGP_Map_Editor
{

	//class RECT
	//{
	//    int left;
	//    int top;
	//    int right;
	//    int bottom;
	//};

	enum Enemies { Turret_Gun = 0, Turret_Frost, Turret_Fire, Turret_Multi,
		Drone_Attack, Drone_Seeker, Drone_Heavy,
		Ground_Mech, Ground_Siege, Ground_FLCL,
		Boss_Apple, Boss_Pimp, Boss_Pirate      
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
	CMapLoad(){};
	CMapLoad(CMapLoad &copy);
	CMapLoad& operator=(CMapLoad &assign);
	~CMapLoad(void){};

public:
	Grid m_gTileMap;
	Selection m_gSelectionMap;
	vector<ListObject> m_lMap;
	string m_szFileName;
	string m_szWorkingDirectoryGRAPHICS;
	string m_szWorkingDirectoryMAPS;
	string m_szMap_01;


	float m_fScaleX;
	float m_fScaleY;

	static CMapLoad *GetInstance()
	{ 
		static CMapLoad sm_Instance;
		return &sm_Instance; 
	}

	bool LoadMap(const char* szFilename)
	{
		string m_szWorkingDirectoryGRAPHICS = "resource/graphics/";
		string m_szWorkingDirectoryMAPS = "resource/maps/";

		string m_szMap_01 = m_szWorkingDirectoryMAPS + szFilename;

		fstream Import(m_szMap_01.c_str(), ios_base::in | ios_base::binary);

		if(Import.good())
		{
			int nEnemyCount, nSpawnerCount, nCollisionCount;
			char cSizeOfString;

			Import.read((char*)&m_gTileMap.m_nColumns, sizeof(int));
			Import.read((char*)&m_gTileMap.m_nRows, sizeof(int));
			Import.read((char*)&m_gTileMap.m_nWidth, sizeof(int));
			Import.read((char*)&m_gTileMap.m_nHeight, sizeof(int));
			Import.read((char*)&m_gSelectionMap.m_nColumns, sizeof(int));
			Import.read((char*)&m_gSelectionMap.m_nRows, sizeof(int));
			Import.read((char*)&m_gSelectionMap.m_nWidth, sizeof(int));
			Import.read((char*)&m_gSelectionMap.m_nHeight, sizeof(int));

			//Set Scale
			float tempX, tempY, stempX, stempY;
			tempX = (float)m_gTileMap.m_nWidth;
			tempY = (float)m_gTileMap.m_nHeight;
			stempX = (float)m_gSelectionMap.m_nWidth;
			stempY = (float)m_gSelectionMap.m_nHeight;

			m_fScaleX = tempX/stempX;
			m_fScaleY = tempY/stempY;

			Import.read((char*)&cSizeOfString, sizeof(char));
			int sizeofstring = (int)cSizeOfString;

			char *buffer = new char[sizeofstring+1];
			Import.read(buffer, sizeofstring);
			buffer[sizeofstring] = '\0';
			m_szFileName = buffer;
			delete buffer;

			string m_szImageLoad = m_szWorkingDirectoryGRAPHICS + m_szFileName;
			
			m_gSelectionMap.m_nImageID = -1;
		
			LoadMapImage(m_szImageLoad.c_str());

			//Create Map Based on size
			for(int i = 0; i < m_gTileMap.m_nColumns*m_gTileMap.m_nRows; i++)
			{
				ListObject temp;
				temp.m_bCollision = false;
				temp.m_bSpawner = false;
				temp.m_bEnemy = false;

				Import.read((char*)&temp.m_nDrawImage, sizeof(int));

				m_lMap.push_back(temp);
			}

			//Read in enemy count
			Import.read((char*)&nEnemyCount, sizeof(int));

			for (int i = 0; i < nEnemyCount; i++)
			{
				int Index;

				Import.read((char*)&Index, sizeof(int));

				ListObject NewNode;
				NewNode = m_lMap[Index];

				NewNode.m_bEnemy = true;

				Import.read((char*)&NewNode.m_eEnemy.m_nType, sizeof(int));
				Import.read((char*)&NewNode.m_eEnemy.m_nPosX, sizeof(int));
				Import.read((char*)&NewNode.m_eEnemy.m_nPosY, sizeof(int));

				m_lMap[Index] = NewNode;
			}

			//Read in spawner count
			Import.read((char*)&nSpawnerCount, sizeof(int));

			for (int i = 0; i < nSpawnerCount; i++)
			{
				int Index;

				Import.read((char*)&Index, sizeof(int));

				ListObject NewNode;
				NewNode = m_lMap[Index];

				NewNode.m_bSpawner = true;

				Import.read((char*)&NewNode.m_sSpawner.m_nType, sizeof(int));
				Import.read((char*)&NewNode.m_sSpawner.m_nPosX, sizeof(int));
				Import.read((char*)&NewNode.m_sSpawner.m_nPosY, sizeof(int));

				m_lMap[Index] = NewNode;
			}

			//Read in collision count
			Import.read((char*)&nCollisionCount, sizeof(int));

			for (int i = 0; i < nCollisionCount; i++)
			{
				int Index;

				Import.read((char*)&Index, sizeof(int));

				ListObject NewNode;
				NewNode = m_lMap[Index];

				NewNode.m_bCollision = true;

				Import.read((char*)&NewNode.m_cCollision.m_nType, sizeof(int));
				Import.read((char*)&NewNode.m_cCollision.m_rWorldPos.left, sizeof(int));
				Import.read((char*)&NewNode.m_cCollision.m_rWorldPos.top, sizeof(int));
				Import.read((char*)&NewNode.m_cCollision.m_rWorldPos.right, sizeof(int));
				Import.read((char*)&NewNode.m_cCollision.m_rWorldPos.bottom, sizeof(int));

				m_lMap[Index] = NewNode;
			}
		}
		return true;
	}


	bool LoadMapImage(const char* szFilename)
	{
		string filename = szFilename;

		m_gSelectionMap.m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(filename.c_str());

		return true;
	}

	void Render()
	{
		CSGD_TextureManager *TM = CSGD_TextureManager::GetInstance();
		CSGD_Direct3D *D3D = CSGD_Direct3D::GetInstance();

		int OffsetX = CCamera::GetInstance()->GetCameraRect().left;
		int OffsetY = CCamera::GetInstance()->GetCameraRect().top;

		for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
			for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
			{
				int Index = Row * m_gTileMap.m_nColumns + Col;

				int PosX = Col * m_gTileMap.m_nWidth - OffsetX;
				int PosY = Row * m_gTileMap.m_nHeight - OffsetY;

				//m_lMap[Index].m_nDrawImage;
				//y = i/width
				//x = i%width

				int ImageRow = m_lMap[Index].m_nDrawImage / m_gSelectionMap.m_nColumns;
				int ImageCol = m_lMap[Index].m_nDrawImage % m_gSelectionMap.m_nColumns;

				if ((ImageRow == (m_gSelectionMap.m_nRows - 1)) && (ImageCol == (m_gSelectionMap.m_nColumns - 1)))
					continue;

				if (PosX > 800 || PosX + m_gSelectionMap.m_nWidth < 0)
					continue;

				if (PosY > 600 || PosY + m_gSelectionMap.m_nHeight < 0)
					continue;

				RECT rDrawRect;
				rDrawRect.left = ImageCol * m_gSelectionMap.m_nWidth;
				rDrawRect.top = ImageRow * m_gSelectionMap.m_nHeight;
				rDrawRect.right = rDrawRect.left + m_gSelectionMap.m_nWidth;
				rDrawRect.bottom = rDrawRect.top + m_gSelectionMap.m_nHeight;


				TM->Draw(m_gSelectionMap.m_nImageID, PosX, PosY, m_fScaleX, m_fScaleY, &rDrawRect, 0, 0, 0, -1);
			}

			D3D->GetSprite()->Flush();	// DRAW ALL SPRITES NOW!!!

#ifdef BOXTYPES
			for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
				for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
				{
					int Index = Row * m_gTileMap.m_nColumns + Col;

					int PosX = Col * m_gTileMap.m_nWidth - OffsetX;
					int PosY = Row * m_gTileMap.m_nHeight - OffsetY;

					if (PosX > 640 || PosX + m_gSelectionMap.m_nWidth < 0)
						continue;

					if (PosY > 480 || PosY + m_gSelectionMap.m_nHeight < 0)
						continue;

					if (m_lMap[Index].m_bEnemy)
					{
						switch (m_lMap[Index].m_eEnemy.m_nType)
						{
						case Turret_Gun:
							D3D->DrawText("E-GT", PosX, PosY, 0, 0, 0);
							break;
						case Turret_Frost:
							D3D->DrawText("E-FT", PosX, PosY, 0, 0, 255);
							break;
						case Turret_Fire:
							D3D->DrawText("E-FT", PosX, PosY, 255, 0, 0);
							break;
						case Turret_Multi:
							D3D->DrawText("E-MT", PosX, PosY, 0, 0, 0);
							break;
						case Drone_Attack:
							D3D->DrawText("E-AD", PosX, PosY, 0, 0, 0);
							break;
						case Drone_Seeker:
							D3D->DrawText("E-SD", PosX, PosY, 0, 0, 0);
							break;
						case Drone_Heavy:
							D3D->DrawText("E-HD", PosX, PosY, 0, 0, 0);
							break;
						case Ground_Mech:
							D3D->DrawText("E-M", PosX, PosY, 0, 0, 0);
							break;
						case Ground_Siege:
							D3D->DrawText("E-SW", PosX, PosY, 0, 0, 0);
							break;
						case Ground_FLCL:
							D3D->DrawText("E-FM", PosX, PosY, 0, 0, 0);
							break;
						case Boss_Apple:
							D3D->DrawText("E-AM", PosX, PosY, 155, 0, 255);
							break;
						case Boss_Pimp:
							D3D->DrawText("E-PS", PosX, PosY, 155, 0, 255);
							break;
						case Boss_Pirate:
							D3D->DrawText("E-DP", PosX, PosY, 155, 0, 255);
							break;
						}
					}
					if (m_lMap[Index].m_bCollision)
					{
						switch (m_lMap[Index].m_cCollision.m_nType)
						{
						case Coll_Solid:
							D3D->DrawText("C-S", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
							break;
						case Coll_Partial:
							D3D->DrawText("C-PS", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
							break;
						case Coll_Moving:
							D3D->DrawText("C-M", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
							break;
						case Coll_Unstable:
							D3D->DrawText("C-U", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
							break;
						case Coll_Trap:
							D3D->DrawText("C-T", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
							break;
						}
					}
					if (m_lMap[Index].m_bSpawner)
					{
						switch (m_lMap[Index].m_sSpawner.m_nType)
						{
						case Turret_Spawner:
							D3D->DrawText("SP-T", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
							break;
						case Drone_Spawner:
							D3D->DrawText("SP-A", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
							break;
						case Ground_Spawner:
							D3D->DrawText("SP-G", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
							break;
						case Boss_Spawner:
							D3D->DrawText("SG-B", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
							break;
						}

					}
				}
#endif
	}
};

#endif