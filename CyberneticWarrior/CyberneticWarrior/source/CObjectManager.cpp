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
	for( unsigned int i = 0; i < m_vObjectList.size(); ++i )
	{
			m_vObjectList[i]->Update(fElapsedTime);
			m_vObjectList[i]->CheckCulling();
						
			if(m_vObjectList[i]->GetCulling())
			{
				m_vCulledList.push_back(m_vObjectList[i]);
				m_vObjectList.erase(m_vObjectList.begin() + i);
				i--;
			}
	}

	for( unsigned int i = 0; i < m_vCulledList.size(); ++i )
	{
		if(m_vCulledList[i]->GetType() != OBJ_ENEMY)
			m_vCulledList[i]->Update(fElapsedTime);
		m_vCulledList[i]->CheckCulling();
		if(!m_vCulledList[i]->GetCulling())
		{
			m_vObjectList.push_back(m_vCulledList[i]);
			m_vCulledList.erase(m_vCulledList.begin() + i);
			i--;
		}
	}
}

void CObjectManager::RenderObjects(void)
{
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		if(!m_vObjectList[i]->GetCulling() && m_vObjectList[i]->GetType() == OBJ_BLOCK )
			this->m_vObjectList[i]->Render();
	}
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		if(!m_vObjectList[i]->GetCulling() && m_vObjectList[i]->GetType() != OBJ_BLOCK )
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
	
	for(vector<CBase*>::iterator iter2 = this->m_vCulledList.begin();
		iter2 != this->m_vCulledList.end(); iter2++)
	{
		if((*iter2) == pObject)
		{
			(*iter2)->Release();

			iter2 = this->m_vCulledList.erase(iter2);
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
	
	for(unsigned int i = 0; i < this->m_vCulledList.size(); i++)
	{
		this->m_vCulledList[i]->Release();
	}
	this->m_vObjectList.clear();
	this->m_vCulledList.clear();
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
					if((!m_vObjectList[i]->GetCulling() && !m_vObjectList[j]->GetCulling()) || m_vObjectList[i]->GetType() == OBJ_ENEMY)
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