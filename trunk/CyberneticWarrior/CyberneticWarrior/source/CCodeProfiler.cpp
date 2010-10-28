#include "PrecompiledHeader.h"

#include "CCodeProfiler.h"

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
	
	tFunction tNewFunction;
	//m_szFuncName.push_back(szFunctionName);
	//tNewFunction->m_nID = this->m_nFunctionCount;
	//m_nCallCounter.push_back(0);
	m_liCurrentFreq.push_back(0);
	m_liCurrentCounter.push_back(0);

	tNewFunction.m_nFunctionName = szFunctionName;
	tNewFunction.m_nID = this->m_nFunctionCount;
	tNewFunction.m_nCallCount = 0;
	this->m_vFunctions.push_back(tNewFunction);

	//this->m_vFunctions.push_back(tNewFunction);


	return this->m_nFunctionCount-1;
}

void CCodeProfiler::FunctionStart(int nID)
{
	LARGE_INTEGER liCounterStart, liFreq;
	QueryPerformanceCounter(&liCounterStart);
	QueryPerformanceFrequency(&liFreq);

	this->m_liCurrentCounter[nID] = liCounterStart.LowPart;
	this->m_liCurrentFreq[nID] = liFreq.LowPart;
	this->m_vFunctions[nID].m_nCallCount++;
	//m_nCallCounter[nID]++;

}

void CCodeProfiler::FuntionEnd(int nID)
{
	LARGE_INTEGER liCounterEnd;
	LONG temp;

	QueryPerformanceCounter(&liCounterEnd);

	temp = ((liCounterEnd.LowPart - m_liCurrentCounter[nID]) / (m_liCurrentFreq[nID] / 1000));
	this->m_vFunctions[nID].m_vPerformanceTimes.push_back(temp);
	//this->m_vFunctions[nID]->m_vPerformanceTimes.push_back(temp);
	//m_vPerformanceTimes.push_back(temp);

}

void CCodeProfiler::SavePerformance(void)
{
	std::fstream save;

	for(int i = 0; i < this->m_nFunctionCount; i++)
	{
		static LONG nAverage = 0;
		static std::string sFileName = this->m_vFunctions[i].m_nFunctionName;
		sFileName.append(".csv");
		save.clear();
		save.open(sFileName.c_str(), std::ios_base::out | std::ios::app);
		if(save.is_open())
		{
			save << "\n\nFunction Name: " << this->m_vFunctions[i].m_nFunctionName.c_str();
			save << "\nAmount of Calls: " << this->m_vFunctions[i].m_nCallCount;
			save << "\nPerformace Times: ";
			for(int index = 0; index < this->m_vFunctions[i].m_nCallCount; index++)
			{
				save << "\n\tCall #" << index+1 << ": " << this->m_vFunctions[i].m_vPerformanceTimes[index] << " milli Seconds";
				nAverage += this->m_vFunctions[i].m_vPerformanceTimes[index];
			}
			save << "\n\nAverage: " << nAverage/this->m_vFunctions[i].m_nCallCount << " milli Seconds";
			save << "\nTimeStamp: " << __TIMESTAMP__;
		}
		save.close();

	}
}
