#ifndef OBJECTFACTORY_H_
#define OBJECTFACTORY_H_

#include <map>
using std::map;

template<typename ClassIDType, typename BaseClassType>
class CObjectFactory
{
private:

	typedef BaseClassType*	(*ObjectCreator)(void);

	map<ClassIDType, ObjectCreator>	m_ObjectCreators;

	static CObjectFactory<ClassIDType, BaseClassType>	sm_ObjectFactoryInstance;

	
	inline CObjectFactory(void) {};
	CObjectFactory(const CObjectFactory&);
	CObjectFactory& operator=(const CObjectFactory&);
	inline ~CObjectFactory(void) {};
	
	template<typename DerivedClassType>
	static BaseClassType*	ConstructObject(void);

public:

	BaseClassType*	CreateObject(ClassIDType id);
	
	template<typename DerivedClassType>
	bool RegisterClassType(ClassIDType id);
	bool UnregisterClassType(ClassIDType id);

	void ShutdownObjectFactory(void);
	
	static CObjectFactory<ClassIDType, BaseClassType>*	GetInstance(void);
};
template<typename ClassIDType, typename BaseClassType>
CObjectFactory<ClassIDType, BaseClassType>
CObjectFactory<ClassIDType, BaseClassType>::sm_ObjectFactoryInstance;// = NULL;

template<typename ClassIDType, typename BaseClassType>
inline CObjectFactory<ClassIDType, BaseClassType>*	CObjectFactory<ClassIDType, BaseClassType>::GetInstance(void)
{
	return	&sm_ObjectFactoryInstance;
}


template<typename ClassIDType, typename BaseClassType>
template<typename DerivedClassType>
inline BaseClassType* CObjectFactory<ClassIDType, BaseClassType>::ConstructObject(void)
{
	return new DerivedClassType;
}


template<typename ClassIDType, typename BaseClassType>
template<typename DerivedClassType>
inline bool CObjectFactory<ClassIDType, BaseClassType>::RegisterClassType(ClassIDType id)
{
	std::pair<ClassIDType, CObjectFactory<ClassIDType, BaseClassType>::ObjectCreator> objTypeDef;

	objTypeDef.first	=	id;
	objTypeDef.second	=	this->ConstructObject<DerivedClassType>;

	this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.insert(objTypeDef);

	return 1;
}


template<typename ClassIDType, typename BaseClassType>
inline bool CObjectFactory<ClassIDType, BaseClassType>::UnregisterClassType(ClassIDType id)
{
	map<ClassIDType, CObjectFactory<ClassIDType, BaseClassType>::ObjectCreator>::iterator iter = this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.find(id);

	if(iter == this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.end())
	{
		return 0;
	}

	this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.erase(iter);

	return 1;
}

template<typename ClassIDType, typename BaseClassType>
inline BaseClassType*	CObjectFactory<ClassIDType, BaseClassType>::CreateObject(ClassIDType id)
{
	map<ClassIDType, CObjectFactory<ClassIDType, BaseClassType>::ObjectCreator>::iterator iter = this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.find(id);

	if(iter == this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.end())
	{
		return NULL;
	}

	return (*iter).second();
}


template<typename ClassIDType, typename BaseClassType>
inline void CObjectFactory<ClassIDType, BaseClassType>::ShutdownObjectFactory(void)
{
	this->CObjectFactory<ClassIDType, BaseClassType>::m_ObjectCreators.clear();
}

#endif