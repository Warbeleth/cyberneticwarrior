//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CMapLoad.cpp”
//
// Author: Corey Ringer (CR)
//
// Purpose: This class serves as the in-game tile-set as well as an engine loader.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "CMapLoad.h"

//Enemy includes
#include "CBaseEnemy.h"
#include "CIdleEnemy.h"
#include "CPatrolEnemy.h"

//Ground
#include "CFLCLMech.h"
#include "CMech.h"
#include "CSiegeWalker.h"
//Turrets
#include "CTurretCore.h"
//Air
#include "CAttackDrone.h"
#include "CHeavyAttackDrone.h"
#include "CSeekerDrone.h"
//Bosses
#include "CAppleMech.h"
#include "CDeathPirate.h"
#include "CPimpStriker.h"


bool CMapLoad::LoadMap(const char* szFilename)
{
	//ENEMY IMAGE LOADS
	{
		m_sEnemyImage.TurretGun = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/regularturret.png");
		m_sEnemyImage.TurretFrost = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/iceturret.png");
		m_sEnemyImage.TurretFire = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/fireturret.png");
		m_sEnemyImage.TurretMulti = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/multiturret.png");
		m_sEnemyImage.DroneAttack = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/regularattackdrone.png");
		m_sEnemyImage.DroneSeeker = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/seekerdrone.png");
		m_sEnemyImage.DroneHeavy = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/attackdrone.png");
		m_sEnemyImage.GroundMech = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/regulargroundmech.png");
		m_sEnemyImage.GroundSiege = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/siegemech.png");
		m_sEnemyImage.GroundFLCL = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/specialgroundmech.png");
		m_sEnemyImage.BossApple = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bossone.png");
		m_sEnemyImage.BossPimp = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/bosstwo.png");
		m_sEnemyImage.BossPirate = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/deathpirate2.png");
	}

	string m_szWorkingDirectoryGRAPHICS = "resource/graphics/";
	string m_szWorkingDirectoryMAPS = "resource/maps/";

	string m_szMap_01 = m_szWorkingDirectoryMAPS + szFilename;

	fstream Import(m_szMap_01.c_str(), ios_base::in | ios_base::binary);

	if(Import.good())
	{
		int nEnemyCount, nSpawnerCount, nCollisionCount;
		char cSizeOfString;

		CObjectFactory<string, CBase>* m_pOF = CObjectFactory<string, CBase>::GetInstance();
		CObjectManager*				   m_pOM = CObjectManager::GetInstance();

		m_pOF->RegisterClassType<CBase>("CBase");
		m_pOF->RegisterClassType<CBlock>("CBlock");
		m_pOF->RegisterClassType<CSpawner>("CSpawner");

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

		//Clear the list
		m_lMap.clear();

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

			//Use "If nGlobalType..." to decide what to spawn
			switch(NewNode.m_eEnemy.m_nType)
			{
			case Turret_Gun:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY, Turret_Gun);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Turret_Frost:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY, Turret_Frost);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Turret_Fire:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY, Turret_Fire);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			case Turret_Multi:
				{
					CTurretCore* CEnemy = new CTurretCore(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY, Turret_Multi);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Drone_Attack:
				{
					CAttackDrone* CEnemy = new CAttackDrone(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Drone_Seeker:
				{
					CSeekerDrone* CEnemy = new CSeekerDrone(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				} 
				break;
			case Drone_Heavy:
				{
					CHeavyAttackDrone* CEnemy = new CHeavyAttackDrone(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				} 
				break;
			case Ground_Mech:
				{
					CMech* CEnemy = new CMech(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Ground_Siege:
				{
					CSiegeWalker* CEnemy = new CSiegeWalker(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Ground_FLCL:
				{
					CFLCLMech* CEnemy = new CFLCLMech(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}
				break;
			case Boss_Apple:
				{
					CAppleMech* CEnemy = new CAppleMech(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}  
				break;
			case Boss_Pimp:
				{
					CPimpStriker* CEnemy = new CPimpStriker(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			case Boss_Pirate:
				{
					CDeathPirate* CEnemy = new CDeathPirate(-1, (float)NewNode.m_eEnemy.m_nPosX, (float)NewNode.m_eEnemy.m_nPosY);
					CObjectManager::GetInstance()->AddObject(CEnemy);
					CEnemy->Release();
				}   
				break;
			}
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

			CSpawner* Spawner = new CSpawner(NewNode.m_sSpawner.m_nType, (float)NewNode.m_sSpawner.m_nPosX, (float)NewNode.m_sSpawner.m_nPosY);

			m_pOM->AddObject(Spawner);
			Spawner->Release();
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

			CBlock* newPlatform = (CBlock*)m_pOF->CreateObject("CBlock");
			newPlatform->SetPosX((float)NewNode.m_cCollision.m_rWorldPos.left);
			newPlatform->SetPosY((float)NewNode.m_cCollision.m_rWorldPos.top);

			newPlatform->SetWidth(NewNode.m_cCollision.m_rWorldPos.right - NewNode.m_cCollision.m_rWorldPos.left);
			newPlatform->SetHeight(NewNode.m_cCollision.m_rWorldPos.bottom - NewNode.m_cCollision.m_rWorldPos.top);
			newPlatform->SetBlock(NewNode.m_cCollision.m_nType);
			newPlatform->SetType(OBJ_BLOCK);

			int ImageRow = m_lMap[Index].m_nDrawImage / m_gSelectionMap.m_nColumns;
			int ImageCol = m_lMap[Index].m_nDrawImage % m_gSelectionMap.m_nColumns;

			RECT rDrawRect;
			rDrawRect.left = ImageCol * m_gSelectionMap.m_nWidth;
			rDrawRect.top = ImageRow * m_gSelectionMap.m_nHeight;
			rDrawRect.right = rDrawRect.left + m_gSelectionMap.m_nWidth;
			rDrawRect.bottom = rDrawRect.top + m_gSelectionMap.m_nHeight;

			newPlatform->SetDrawRect(rDrawRect);
			newPlatform->SetImageID(m_nImageID);
			newPlatform->SetScaleX(m_fScaleX);
			newPlatform->SetScaleY(m_fScaleY);


			m_pOM->AddObject(newPlatform);
			newPlatform->Release();
		}
	}
	return true;
}

bool CMapLoad::LoadMapImage(const char* szFilename)
{
	if(m_gSelectionMap.m_nImageID == -1)
		m_nImageID = m_gSelectionMap.m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(szFilename);
	else
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_gSelectionMap.m_nImageID);
		m_nImageID = -1;
		m_nImageID = m_gSelectionMap.m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(szFilename);
	}

	return true;
}

bool CMapLoad::LoadAnimations( void )
{
	m_Animations[Turret_Gun].LoadBinary("resource/binary/Turret_Regular.bae");
	m_Animations[Turret_Frost].LoadBinary("resource/binary/Turret_Ice.bae");
	m_Animations[Turret_Fire].LoadBinary("resource/binary/Turret_Fire.bae");
	m_Animations[Turret_Multi].LoadBinary("resource/binary/Turret_MultiTurret.bae");
	m_Animations[Drone_Attack].LoadBinary("resource/binary/Air_RegularAttackDrone.bae");
	m_Animations[Drone_Seeker].LoadBinary("resource/binary/Air_Seeker.bae");
	m_Animations[Drone_Heavy].LoadBinary("resource/binary/Air_SpecialAttackDrone.bae");
	m_Animations[Ground_Mech].LoadBinary("resource/binary/Ground_Mech.bae");
	m_Animations[Ground_Siege].LoadBinary("resource/binary/Ground_Siege.bae");
	m_Animations[Ground_FLCL].LoadBinary("resource/binary/Ground_FLCL.bae");
	m_Animations[Boss_Apple].LoadBinary("resource/binary/AppleMech.bae");
	m_Animations[Boss_Pimp].LoadBinary("resource/binary/PimpStriker.bae");
	m_Animations[Boss_Pirate].LoadBinary("resource/binary/DeathPirate.bae");
	return true;
}
	
CAnimations* CMapLoad::CreateAnimation( int nAnimationNumber )
{
	CAnimations* Animation;
	if(nAnimationNumber >= 0 && nAnimationNumber <= Enemies_Total )
	{
		Animation = new CAnimations( );
		*Animation = m_Animations[nAnimationNumber];
	
	}
	else
		Animation = NULL;

	return Animation;
}

void CMapLoad::Render()
{
	CSGD_TextureManager *TM = CSGD_TextureManager::GetInstance();
	CSGD_Direct3D *D3D = CSGD_Direct3D::GetInstance();

	int OffsetX = CCamera::GetInstance()->GetOffsetX();
	int OffsetY = CCamera::GetInstance()->GetOffsetY();
	float CamScale = CCamera::GetInstance()->GetScale();

	//for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
	//	for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
	//	{
	//		int Index = Row * m_gTileMap.m_nColumns + Col;

	//		int PosX = (int)(CamScale * (Col * m_gTileMap.m_nWidth - OffsetX));
	//		int PosY = (int)(CamScale * (Row * m_gTileMap.m_nHeight - OffsetY));

	//		//m_lMap[Index].m_nDrawImage;
	//		//y = i/width
	//		//x = i%width

	//		int ImageRow = m_lMap[Index].m_nDrawImage / m_gSelectionMap.m_nColumns;
	//		int ImageCol = m_lMap[Index].m_nDrawImage % m_gSelectionMap.m_nColumns;

	//		if ((ImageRow == (m_gSelectionMap.m_nRows - 1)) && (ImageCol == (m_gSelectionMap.m_nColumns - 1)))
	//			continue;

	//		if (PosX > 800 || PosX + m_gSelectionMap.m_nWidth < 0)
	//			continue;

	//		if (PosY > 600 || PosY + m_gSelectionMap.m_nHeight < 0)
	//			continue;

	//		RECT rDrawRect;
	//		rDrawRect.left = ImageCol * m_gSelectionMap.m_nWidth;
	//		rDrawRect.top = ImageRow * m_gSelectionMap.m_nHeight;
	//		rDrawRect.right = rDrawRect.left + m_gSelectionMap.m_nWidth;
	//		rDrawRect.bottom = rDrawRect.top + m_gSelectionMap.m_nHeight;


	//		TM->Draw(m_gSelectionMap.m_nImageID, PosX, PosY, m_fScaleX*CamScale, m_fScaleY*CamScale, &rDrawRect, 0, 0, 0, -1);
	//	}

	//	D3D->GetSprite()->Flush();	// DRAW ALL SPRITES NOW!!!

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

CMapLoad* CMapLoad::GetInstance()
{ 
	static CMapLoad sm_Instance;
	return &sm_Instance; 
}