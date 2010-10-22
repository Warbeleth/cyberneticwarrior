#ifndef COBJECTMANAGER_H_
#define COBJECTMANAGER_H_

#include <vector>
using std::vector;

class CBase;

class CObjectManager
{
private:

	vector<CBase*> m_vObjectList;

	CObjectManager(void);
	~CObjectManager(void);

	CObjectManager& operator=(const CObjectManager&);
	CObjectManager(const CObjectManager&);

	static CObjectManager*	sm_pObjectManagerInstance;

public:

	bool CheckCollisions(void);
	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);

	void AddObject(CBase* pObject);	
	void RemoveObject(CBase* pObject);

	void RemoveAllObjects(void);

	static CObjectManager*	GetInstance(void);
	static void DeleteInstance(void);

};
#endif