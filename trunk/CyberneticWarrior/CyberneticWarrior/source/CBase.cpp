//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	File : CBase.h
//
//	Author : Patrick Alvarez 
//
//	Purpose : Parent base class definitions for all In-Game objects.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
#include "PrecompiledHeader.h"

#include "CBase.h"
#include "CCamera.h"


////////////////////////////////////////////////////////////////////////////////////
//	Function : CBase (Constructor)
////////////////////////////////////////////////////////////////////////////////////
CBase::CBase(void)
{
	this->m_uiRefCount = 1;

	this->SetImageID(-1);
	this->SetType(OBJ_BASE);

	this->m_rBaseRect.fPosX = this->m_rBaseRect.fPosY = 0.0f;
	this->m_rBaseRect.nWidth = this->m_rBaseRect.nHeight = 0;

	this->SetBaseVelX(0.0f);
	this->SetBaseVelY(0.0f);

	this->SetPosX(0.0f);
	this->SetPosY(0.0f);

	m_pAnimations = NULL;

	this->m_vSpeed.fX = this->m_vSpeed.fY = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : CBase (Default Constructor)
////////////////////////////////////////////////////////////////////////////////////
CBase::CBase(int nType, float fPosX, float fPosY, int nWidth, int nHeight, float fVelX, float fVelY)
{
	this->m_uiRefCount = 1;
	
	this->SetImageID(-1);

	this->m_nType = nType;

	this->SetPosX(fPosX);
	this->SetPosY(fPosY);
	this->SetWidth(nWidth);
	this->SetHeight(nHeight);

	this->SetBaseVelX(fVelX);
	this->SetBaseVelY(fVelY);


}

////////////////////////////////////////////////////////////////////////////////////
//	Function : ~CBase (Destructor)
////////////////////////////////////////////////////////////////////////////////////
CBase::~CBase(void)
{
	if( m_pAnimations )
		delete m_pAnimations;
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Update
////////////////////////////////////////////////////////////////////////////////////
void CBase::Update(float fElapsedTime)
{
	this->SetPosX(this->GetPosX() + this->GetBaseVelX() * fElapsedTime);
	this->SetPosY(this->GetPosY() + this->GetBaseVelY() * fElapsedTime);
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Render
////////////////////////////////////////////////////////////////////////////////////
void CBase::Render(void)
{	
	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), (int)this->GetPosX(), (int)this->GetPosY());
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : AddRef
////////////////////////////////////////////////////////////////////////////////////
void CBase::AddRef(void)
{
	this->m_uiRefCount++;
}

////////////////////////////////////////////////////////////////////////////////////
//	Function : Release
////////////////////////////////////////////////////////////////////////////////////
void CBase::Release(void)
{
	this->m_uiRefCount--;

	if(this->m_uiRefCount == 0)
	{
		delete this;
	}
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetCollisionRect
////////////////////////////////////////////////////////////////////////////////////
RECT CBase::GetRect(void)
{
	RECT rCollision;
	rCollision.top = (LONG)(this->GetPosY() );//- (GetHeight()>>1));
	rCollision.left = (LONG)(this->GetPosX() );//- (GetWidth()>>1));
	rCollision.bottom = rCollision.top + this->GetHeight();
	rCollision.right = rCollision.left + this->GetWidth();

	return rCollision;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : CheckCollision
////////////////////////////////////////////////////////////////////////////////////
bool CBase::CheckCollision(CBase *pBase)
{
	RECT rIntersect;
	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{ 
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetImageID
////////////////////////////////////////////////////////////////////////////////////
int CBase::GetImageID(void) const
{
	return this->m_nImageID;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetType
////////////////////////////////////////////////////////////////////////////////////
int CBase::GetType(void) const
{
	return this->m_nType;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetPosX
////////////////////////////////////////////////////////////////////////////////////
float CBase::GetPosX(void) const
{
	return this->m_rBaseRect.fPosX;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetPosY
////////////////////////////////////////////////////////////////////////////////////
float CBase::GetPosY(void) const
{
	return this->m_rBaseRect.fPosY;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetWidth
////////////////////////////////////////////////////////////////////////////////////
int CBase::GetWidth(void) const
{
	return this->m_rBaseRect.nWidth;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetHeight
////////////////////////////////////////////////////////////////////////////////////
int CBase::GetHeight(void) const
{
	return this->m_rBaseRect.nHeight;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetBaseVelX
////////////////////////////////////////////////////////////////////////////////////
float CBase::GetBaseVelX(void) const
{
	return this->m_vBaseVelocity.fX;
}

////////////////////////////////////////////////////////////////////////////////////
//	Accessor : GetBaseVelY
////////////////////////////////////////////////////////////////////////////////////
float CBase::GetBaseVelY(void) const
{
	return this->m_vBaseVelocity.fY;
}


float CBase::GetRotation(void) const
{
	return this->m_fRotation;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetImageID
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetImageID(int nImageID)
{
	this->m_nImageID = nImageID;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetType
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetType(int nType)
{
	this->m_nType = nType;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetPosX
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetPosX(float fPosX)
{
	this->m_rBaseRect.fPosX = fPosX;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetPosY
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetPosY(float fPosY)
{
	this->m_rBaseRect.fPosY = fPosY;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetBaseVelX
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetBaseVelX(float nVelX)
{
	this->m_vBaseVelocity.fX = nVelX;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetBaseVelY
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetBaseVelY(float nVelY)
{
	this->m_vBaseVelocity.fY = nVelY;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetWidth
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetWidth(int nWidth)
{
	this->m_rBaseRect.nWidth = nWidth;
}

////////////////////////////////////////////////////////////////////////////////////
//	Mutator : SetHeight
////////////////////////////////////////////////////////////////////////////////////
void CBase::SetHeight(int nHeight)
{
	this->m_rBaseRect.nHeight = nHeight;
}


void CBase::SetRotation(float fRotation)
{
	this->m_fRotation = fRotation;
}