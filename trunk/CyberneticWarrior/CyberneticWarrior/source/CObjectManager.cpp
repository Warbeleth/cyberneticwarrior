// Liz, pat, steve, jeremy, dave, john
#include "CObjectManager.h"
#include "CBase.h"
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

CObjectManager* CObjectManager::sm_pObjectManagerInstance = NULL;

CObjectManager::CObjectManager(void) {}

CObjectManager::~CObjectManager(void) {}

void CObjectManager::UpdateObjects(float fElapsedTime)
{
	vector<CBase*>::iterator iter = this->m_vObjectList.begin();

	while(iter != this->m_vObjectList.end())
	{
		(*iter)->Update(fElapsedTime);
		iter++;
	}
}

void CObjectManager::RenderObjects(void)
{
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		this->m_vObjectList[i]->Render();
	}
}

void CObjectManager::AddObject(CBase *pObject)
{
	if(pObject == NULL) { return; }

	this->m_vObjectList.push_back(pObject);

	pObject->AddRef();
}

void CObjectManager::RemoveObject(CBase *pObject)
{
	if(pObject == NULL) { return; }

	for(vector<CBase*>::iterator iter = this->m_vObjectList.begin();
		iter != this->m_vObjectList.end(); iter++)
	{
		if((*iter) == pObject)
		{
			(*iter)->Release();

			iter = this->m_vObjectList.erase(iter);
			break;
		}
	}
}

void CObjectManager::RemoveAllObjects(void)
{
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		this->m_vObjectList[i]->Release();
	}
	this->m_vObjectList.clear();
}

bool CObjectManager::CheckCollisions(void)
{
	CMapLoad::GetInstance()->m_bCollisionCheck = false;

	int CameraX = CCamera::GetInstance()->GetOffsetX();
	int CameraY = CCamera::GetInstance()->GetOffsetY();

	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		for(unsigned int j = 0; j < this->m_vObjectList.size(); j++)
		{
			if(j != i)
			{
				//int ObjectType = m_vObjectList[i]->GetType();

				//if(ObjectType != OBJ_PLAYER)
				//	continue;
				/*
				int ObjectType = m_vObjectList[i]->GetType();
				int ObjectTypeOther = m_vObjectList[j]->GetType();

				float xPos = 0;
				float yPos = 0;

				float xPosOther = 0;
				float yPosOther = 0;


				if(ObjectType == OBJ_ENEMY)
				{
				xPos = ((CBaseEnemy*)m_vObjectList[i])->GetPosX();	
				yPos = ((CBaseEnemy*)m_vObjectList[i])->GetPosY();	
				}
				else if(ObjectType == OBJ_BLOCK)
				{
				xPos = ((CBlock*)m_vObjectList[i])->GetPosX();	
				yPos = ((CBlock*)m_vObjectList[i])->GetPosY();
				}
				else if(ObjectType == OBJ_SPAWNER)
				{
				xPos = ((CSpawner*)m_vObjectList[i])->GetPosX();	
				yPos = ((CSpawner*)m_vObjectList[i])->GetPosY();
				}
				else
				{
				xPos = m_vObjectList[i]->GetPosX();
				yPos = m_vObjectList[i]->GetPosY();
				}

				if(ObjectTypeOther == OBJ_ENEMY)
				{
				xPosOther = ((CBaseEnemy*)m_vObjectList[j])->GetPosX();	
				yPosOther = ((CBaseEnemy*)m_vObjectList[j])->GetPosY();	
				}
				else if(ObjectTypeOther == OBJ_BLOCK)
				{
				xPosOther = ((CBlock*)m_vObjectList[j])->GetPosX();	
				yPosOther = ((CBlock*)m_vObjectList[j])->GetPosY();
				}
				else if(ObjectTypeOther == OBJ_SPAWNER)
				{
				xPosOther = ((CSpawner*)m_vObjectList[j])->GetPosX();	
				yPosOther = ((CSpawner*)m_vObjectList[j])->GetPosY();
				}
				else
				{
				xPosOther = m_vObjectList[j]->GetPosX();
				yPosOther = m_vObjectList[j]->GetPosY();
				}

				if(xPosOther < CameraX || xPos < CameraX)
				continue;
				if(xPosOther > CameraX+800 || xPos > CameraX+800)
				continue;

				if(yPosOther < CameraY || yPos < CameraX)
				continue;
				if(yPosOther > CameraY+600 || yPos > CameraY+600)
				continue;*/

				if(this->m_vObjectList[i]->CheckCollision(this->m_vObjectList[j]))
				{
					//break;
				}
			}
		}
	}
	return 0;
}

CObjectManager* CObjectManager::GetInstance(void)
{
	if(!sm_pObjectManagerInstance)
	{
		sm_pObjectManagerInstance = new CObjectManager();
	}
	return sm_pObjectManagerInstance;
}

void CObjectManager::DeleteInstance(void)
{
	if(sm_pObjectManagerInstance)
	{
		delete sm_pObjectManagerInstance;
		sm_pObjectManagerInstance = NULL;
	}
}