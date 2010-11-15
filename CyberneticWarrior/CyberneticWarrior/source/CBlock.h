#ifndef	BLOCK_H_
#define BLOCK_H_

#include "CBase.h"

enum{BLOCK_SOLID = 0, BLOCK_PARTIAL, BLOCK_MOVING, BLOCK_UNSTABLE, BLOCK_TRAP};
class CBlock : public CBase
{
private:
	
	int	m_nBlockType;
	float m_fPosX;
	float m_fPosY;
	float m_fBasePosX;
	float m_fBasePosY;
	float m_fTimeWaited;
	float m_fSpeed;
	float m_fScaleX;
	float m_fScaleY;
	int m_nWidth;
	int m_nHeight;
	bool m_bStable;
	RECT m_rDrawRect;

public:

	CBlock(int nBlockType = 0, float fPosX = 0, float fPosY = 0, int nWidth = 32, int nHeight = 32);
	~CBlock(void);

	void Update(float fElapsedTime);
	void Render(void);

	RECT GetRect(void);

	float GetPosX() { return m_fPosX; }
	float GetPosY() { return m_fPosY; }
	int GetWidth() { return m_nWidth; }
	int GetHeight() { return m_nHeight; }
	int GetBlock() { return m_nBlockType; }

	void SetPosX(float fPosX) { m_fPosX = fPosX; }
	void SetPosY(float fPosY) { m_fPosY = fPosY; }
	void SetWidth(int nWidth) { m_nWidth = nWidth; }
	void SetHeight(int nHeight) { m_nHeight = nHeight; }
	void SetBlock(int nType) { m_nBlockType = nType; }
	void SetStable(bool Value) { m_bStable = Value; }
	void SetDrawRect(RECT rDrawRect) { m_rDrawRect = rDrawRect; }
	void SetScaleX(float Value) { m_fScaleX = Value; }
	void SetScaleY(float Value) { m_fScaleY = Value; }



	bool CheckCollision(CBase* pBase);
	////////////////////////////////////////////////////////////////////////////////////
	//	Function : CheckCulling
	//
	//	Purpose : To check for culling of objects to improve performance
	////////////////////////////////////////////////////////////////////////////////////
	virtual void CheckCulling();
};
#endif