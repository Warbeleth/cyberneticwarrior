#ifndef CODEPROFILER_H_
#define CODEPROFILER_H_

class CSGD_Direct3D;
#include <vector>

class CCodeProfiler
{
private:
	
	struct tFunction
	{
		std::vector<LONG>	m_vPerformanceTimes;
		std::string				m_nFunctionName;
		int					m_nCallCount;
		int					m_nID;
	};

	std::vector<tFunction>				m_vFunctions;
	std::vector<int>					m_liCurrentCounter;
	std::vector<int>					m_liCurrentFreq;

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