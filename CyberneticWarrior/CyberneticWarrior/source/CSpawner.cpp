#include "PrecompiledHeader.h"

#include "CSpawner.h"
#include "CGame.h"

CSpawner::CSpawner(int nSpawnerType, float fPosX, float fPosY, int nWidth, int nHeight)
{
	SetType(OBJ_SPAWNER);
	m_nSpawnerType = nSpawnerType;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_fPosX = fPosX;
	m_fPosY = fPosY;
	m_fTimeWaited = 0.0f;

	if(nSpawnerType == SPAWNER_TURRET)
		m_nTypeSpawner = 0;
	else if(nSpawnerType == SPAWNER_AIR)
		m_nTypeSpawner = 4;
	else if(nSpawnerType == SPAWNER_GROUND)
		m_nTypeSpawner = 7;
	else if(nSpawnerType == SPAWNER_BOSS)
		m_nTypeSpawner = 10;
}

CSpawner::~CSpawner()
{

}

void CSpawner::Update(float fElapsedTime)
{
	m_fTimeWaited += fElapsedTime;

	if(m_fTimeWaited >= 15.0f)
	{
		m_fTimeWaited = 0.0f;
		CGame::GetInstance()->GetMessageSystemPointer()->SendMsg(new CCreateEnemyMessage(m_nTypeSpawner, (int)GetPosX(), (int)GetPosY()));
		m_nTypeSpawner++;

		if(m_nSpawnerType == SPAWNER_TURRET)
			if(m_nTypeSpawner > 3)
				m_nTypeSpawner = 0;
		else if(m_nSpawnerType == SPAWNER_AIR)
			if(m_nTypeSpawner > 6)
				m_nTypeSpawner = 4;
		else if(m_nSpawnerType == SPAWNER_GROUND)
			if(m_nTypeSpawner > 9)
				m_nTypeSpawner = 7;
		else if(m_nSpawnerType == SPAWNER_BOSS)
			if(m_nTypeSpawner > 12)
				m_nTypeSpawner = 10;
	}
}

void CSpawner::Render(void)
{
	int left = (int)GetPosX() - CCamera::GetInstance()->GetOffsetX();
	int top = (int)GetPosY() - CCamera::GetInstance()->GetOffsetY();
	int right = left + GetWidth();
	int bottom = top + GetHeight();

	CSGD_Direct3D::GetInstance()->DrawLine(left, top, right, top, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, bottom, right, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(left, top, left, bottom, 255, 0, 0);
	CSGD_Direct3D::GetInstance()->DrawLine(right, top, right, bottom, 255, 0, 0);
}

RECT CSpawner::GetRect(void)
{
	RECT tempRect;

	tempRect.left = (LONG)GetPosX();
	tempRect.top = (LONG)GetPosY();
	tempRect.right = tempRect.left + GetWidth();
	tempRect.bottom = tempRect.top + GetHeight();

	return tempRect;
}

bool CSpawner::CheckCollision(CBase* pBase)
{
	RECT rIntersect;

	if(pBase == this)
		return false;

	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CSpawner::CheckCulling()
{
	SetCulling(false);

	float CameraX = CCamera::GetInstance()->GetOffsetX();
	float CameraY = CCamera::GetInstance()->GetOffsetY();
	float xPos = GetPosX();
	float yPos = GetPosY();
	float wPos = xPos+GetWidth();
	float hPos = yPos+GetHeight();

	if(wPos < CameraX)
		SetCulling(true);
	if(xPos > CameraX+800)
		SetCulling(true);

	if(hPos < CameraY)
		SetCulling(true);
	if(yPos > CameraY+600)
		SetCulling(true);
}