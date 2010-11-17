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
		(*iter)->CheckCulling();

		iter++;
	}
}

void CObjectManager::RenderObjects(void)
{
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		if(!m_vObjectList[i]->GetCulling())
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

	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		if( m_vObjectList[i]->GetType() != OBJ_BLOCK )
		{
			if(m_vObjectList[i]->GetType() == OBJ_ENEMY)
			{
				CBaseEnemy* Enemy = (CBaseEnemy*)m_vObjectList[i];
				Enemy->SetCollision(false);
			}

			for(unsigned int j = 0; j < this->m_vObjectList.size(); j++)
			{
				if(j != i )
				{
					if((!m_vObjectList[i]->GetCulling() && !m_vObjectList[j]->GetCulling()))
						this->m_vObjectList[i]->CheckCollision(this->m_vObjectList[j]);
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