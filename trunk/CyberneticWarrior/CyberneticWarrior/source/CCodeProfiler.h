#ifndef CODEPROFILER_H_
#define CODEPROFILER_H_

#include "SGD Wrappers/CSGD_Direct3D.h"
#include <vector>

class CCodeProfiler
{
private:
	
	//struct tFunction
	//{
	//	std::vector<int>	m_vPerformanceTimes;
	//	
	//	int					m_nID;
	////	QueryPerformanceCounter()
	//};
	std::vector<LONG>					m_vPerformanceTimes;
	std::vector<int>					m_liCurrentCounter;
	std::vector<int>					m_liCurrentFreq;
	std::vector<std::string>					m_szFuncName;
	std::vector<int>					m_nCallCounter;

	//std::vector<tFunction*> m_vFunctions;

	int m_nFunctionCount;

	int	m_nProfilerTime;
	int	m_nProgramTime;

	CCodeProfiler(void);
	~CCodeProfiler(void);
	CCodeProfiler(const CCodeProfiler&);
	CCodeProfiler&	operator=(const CCodeProfiler&);

	static CCodeProfiler*	sm_pCodeProfilerInstance;

public:

	static CCodeProfiler*	GetInstance(void);
	static void DeleteInstance(void);

	int CreateFunction(const char* szFunctionName);

	void FunctionStart(int nID);
	void FuntionEnd(int nID);

	void SavePerformance(void);	

};
#endif