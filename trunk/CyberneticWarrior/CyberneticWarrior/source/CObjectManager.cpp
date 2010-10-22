// Liz, pat, steve, jeremy, dave, john
#include "CObjectManager.h"
#include "CBase.h"

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
	for(unsigned int i = 0; i < this->m_vObjectList.size(); i++)
	{
		for(unsigned int j = 0; j < this->m_vObjectList.size(); ++j)
		{
			if(j != i && this->m_vObjectList[i]->GetType() != this->m_vObjectList[j]->GetType())
			{
				if(this->m_vObjectList[i]->CheckCollision(this->m_vObjectList[j]))
				{
					break;
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