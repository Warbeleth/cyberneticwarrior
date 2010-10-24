#include "PrecompiledHeader.h"

#include "CPickUp.h"

CPickUp::CPickUp(void)
{
	this->SetPosX(0.0f);
	this->SetPosY(0.0f);
	this->SetWidth(0);
	this->SetHeight(0);
	this->SetType(OBJ_PICKUP);
	this->SetPickUpType(GRAPPLING_HOOK);
	bOn = 1;
}

CPickUp::~CPickUp(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(this->GetImageID());
}

void CPickUp::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
}

void CPickUp::Render(void)
{
	if(bOn)
	CSGD_TextureManager::GetInstance()->Draw(this->GetImageID(), (int)this->GetPosX(), (int)this->GetPosY());
}


bool CPickUp::CheckCollision(CBase* pBase)
{
	RECT rIntersect;
	if(IntersectRect(&rIntersect, &this->GetRect(), &(pBase->GetRect())))
	{
		if(pBase->GetType() == OBJ_PLAYER)
		{
			bOn = 0;
			//delete this;
		}
		return 1;
	}
	else
	{
		return 0;
	}
	return 0;
}

int CPickUp::GetPickUpType(void) {return this->m_nPickUpType;}
void CPickUp::SetPickUpType(int nType) {this->m_nPickUpType = nType;}