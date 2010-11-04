#ifndef CSPAWNER_H_
#define CSPAWNER_H_

#include "CBase.h"

enum{SPAWNER_TURRET = 0, SPAWNER_AIR, SPAWNER_GROUND, SPAWNER_BOSS};
class CSpawner : public CBase
{
private:
	
	int	m_nSpawnerType;
	int m_nWidth;
	int m_nHeight;
	float m_fPosX;
	float m_fPosY;
	float m_fTimeWaited;

public:
	CSpawner(int nSpawnerType = 1, float fPosX = 0, float fPosY = 0, int nWidth = 32, int nHeight = 32);
	~CSpawner(void);

	void Update(float fElapsedTime);
	void Render(void);

	RECT GetRect(void);

	float GetPosX() { return m_fPosX; }
	float GetPosY() { return m_fPosY; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }
	int GetSpawner() { return m_nSpawnerType; }

	void SetPosX(float fPosX) { m_fPosX = fPosX; }
	void SetPosY(float fPosY) { m_fPosY = fPosY; }
	void SetWidth(int nWidth) { m_nWidth = nWidth; }
	void SetHeight(int nHeight) { m_nHeight = nHeight; }
	void SetSpawner(int nType) { m_nSpawnerType = nType; }


	bool CheckCollision(CBase* pBase);

};

#endif