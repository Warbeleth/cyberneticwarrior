#include "CCodeProfiler.h"
#include <fstream>


CCodeProfiler*	CCodeProfiler::sm_pCodeProfilerInstance = NULL;

CCodeProfiler::CCodeProfiler(void)
{
	this->m_nFunctionCount = 0;

	this->m_nProfilerTime = 0;
	this->m_nProgramTime = 0;
}

CCodeProfiler::~CCodeProfiler(void)
{
}

CCodeProfiler*	CCodeProfiler::GetInstance(void)
{
	if(!sm_pCodeProfilerInstance)
	{
		sm_pCodeProfilerInstance = new CCodeProfiler();
	}
	return sm_pCodeProfilerInstance;
}

void CCodeProfiler::DeleteInstance(void)
{
	if(sm_pCodeProfilerInstance)
	{
		delete sm_pCodeProfilerInstance;
		sm_pCodeProfilerInstance = NULL;
	}
}

int CCodeProfiler::CreateFunction(const char* szFunctionName)
{
	//tFunction* tNewFunction = new tFunction();
	
	this->m_nFunctionCount++;

	m_szFuncName.push_back(szFunctionName);
	//tNewFunction->m_nID = this->m_nFunctionCount;
	m_nCallCounter.push_back(0);
	m_liCurrentFreq.push_back(0);
	m_liCurrentCounter.push_back(0);


	//this->m_vFunctions.push_back(tNewFunction);


	return this->m_nFunctionCount-1;
}

void CCodeProfiler::FunctionStart(int nID)
{
	LARGE_INTEGER liCounterStart, liFreq;
	QueryPerformanceCounter(&liCounterStart);
	QueryPerformanceFrequency(&liFreq);

	m_liCurrentCounter[nID] = liCounterStart.LowPart;
	m_liCurrentFreq[nID] = liFreq.LowPart;
	m_nCallCounter[nID]++;

}

void CCodeProfiler::FuntionEnd(int nID)
{
	LARGE_INTEGER liCounterEnd;
	LONG temp;
	QueryPerformanceCounter(&liCounterEnd);
	temp = ((liCounterEnd.LowPart - m_liCurrentCounter[nID]) / (m_liCurrentFreq[nID] / 1000));
	//this->m_vFunctions[nID]->m_vPerformanceTimes.push_back(temp);
	m_vPerformanceTimes.push_back(temp);

}

void CCodeProfiler::SavePerformance(void)
{
	std::fstream save;

	for(int i = 0; i < this->m_nFunctionCount; i++)
	{
		m_szFuncName[i].append(".csv");// = m_szFuncName[i]. + ;
		save.clear();
		save.open(m_szFuncName[i].c_str(), std::ios_base::out | std::ios::app);
		if(save.is_open())
		{
			save << "Function Name: " << m_szFuncName[i].c_str();
			save << "\nAmount of Calls: " << m_nCallCounter[i];
			save << "\nPerformace: " << m_vPerformanceTimes[i] << " milli Seconds";
			save << "\nTimeStamp: " << __TIMESTAMP__ << "\n\n";
			
		}
		save.close();

	}
}
