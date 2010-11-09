#include "PrecompiledHeader.h"
#include "CHud.h"
#include "CPlayer.h"
#include "CSinglePlayerState.h"

CHud::CHud( void )
{
	// Singleton Pointer
	m_pTM = CSGD_TextureManager::GetInstance();

	// Player for health bars
	m_pPlayer = NULL;

	// Hud Image IDs
	m_nHealthAndEnergyBarId = -1;
	m_nHealthAndEnergyBarId = m_pTM->LoadTexture("resource/graphics/HealthAndEnergy.png");
	m_nCharacterPortraitId = -1;
	m_nCharacterPortraitId = m_pTM->LoadTexture("resource/graphics/Players.png");
	m_nWeaponEquipmentId = -1;
	m_nWeaponEquipmentId = CSinglePlayerState::GetInstance()->GetWeaponSelectionID();
	// Hud Image RECTS
	// health
	m_rImageRects[0].left = 0;
	m_rImageRects[0].top = 0;
	m_rImageRects[0].right = 162;
	m_rImageRects[0].bottom = 16;
	// energy
	m_rImageRects[1].left = 0;
	m_rImageRects[1].top = 16;
	m_rImageRects[1].right = 162;
	m_rImageRects[1].bottom = 32;
	// portriat
	m_rImageRects[2].left = 0;
	m_rImageRects[2].top = 0;
	m_rImageRects[2].right = 94;
	m_rImageRects[2].bottom = 94;
	// equipment
	m_rImageRects[3].left = 0;
	m_rImageRects[3].top = 0;
	m_rImageRects[3].right = 64;
	m_rImageRects[3].bottom = 64;

	// Font
	m_HudFont.InitFont( "resource/fonts/example.png", "resource/fonts/Example.fnt" );

	// Settings
	m_nPlayerNumber = 0;

	// Timer
	m_fElapsedTime = 0.0f;
}

CHud::~CHud( void )
{
	m_pTM->UnloadTexture(m_nHealthAndEnergyBarId);
	m_pTM->UnloadTexture(m_nCharacterPortraitId);
	m_HudFont.ShutdownFont();
}

RECT CHud::GetRect( int nType, int nEquipment )
{
	RECT rRectangle = m_rImageRects[nType];
		
	switch(nType)
	{
	case TYPE_HEALTH:
		{	
			float fPercentage;
			fPercentage = m_pPlayer->m_fRemainingHealth/ m_pPlayer->m_fTotalHealth;
			
			rRectangle.right = (LONG)(fPercentage * rRectangle.right); 
			break;
		}
	case TYPE_ENERGY:
		{
			float fPercentage;
			fPercentage = m_pPlayer->m_fRemainingEnergy / m_pPlayer->m_fTotalEnergy;
			
			rRectangle.right = (LONG)(fPercentage * rRectangle.right); 
			break;
		}
	case TYPE_PORTRAIT:
		{
			rRectangle.left = rRectangle.right * m_nPlayerNumber;
			rRectangle.right = rRectangle.right + rRectangle.left;
			break;
		}
	case TYPE_EQUIPMENT:
		{
			if(nEquipment < 16)
			{
				rRectangle.left = rRectangle.right * nEquipment;
				rRectangle.right = rRectangle.right + rRectangle.left;
			}
			else
			{
				rRectangle.left = rRectangle.right* (nEquipment - 10);
				rRectangle.right = rRectangle.right + rRectangle.left;
				rRectangle.top = rRectangle.bottom;
				rRectangle.bottom = rRectangle.top + rRectangle.bottom;
			}
			break;
		}
	};

	return rRectangle;
}

void CHud::Update( float fElapsedTime )
{
	m_fElapsedTime += fElapsedTime;

	if( m_fElapsedTime >= ENERGY_DELAY && m_pPlayer->m_fRemainingEnergy != m_pPlayer->m_fTotalEnergy && m_pPlayer->m_bOnGround )
	{
		m_fElapsedTime = 0.0f;
		m_pPlayer->IncrementEnergy( 1.0f );
	}
}

void CHud::Render( void )
{
	char buffer[32];
	sprintf_s(buffer, "Score: %i", m_pPlayer->m_nScore);

	switch(m_nPlayerNumber)
	{
	case 0:
		{
			m_pTM->Draw(m_nHealthAndEnergyBarId, m_rImageRects[TYPE_PORTRAIT].right, 0, 1, 1, &GetRect(TYPE_HEALTH) );
			m_pTM->Draw(m_nHealthAndEnergyBarId, m_rImageRects[TYPE_PORTRAIT].right, m_rImageRects[0].bottom, 1, 1, &GetRect(TYPE_ENERGY));
			m_pTM->Draw(m_nCharacterPortraitId, 0, 0, 1, 1, &GetRect(TYPE_PORTRAIT));
			m_pTM->Draw(m_nWeaponEquipmentId, 100, 35, 0.5f, 0.5f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedBootSlot));
			m_pTM->Draw(m_nWeaponEquipmentId, 140, 35, 0.5f, 0.5f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedHeadSlot));
			m_pTM->Draw(m_nWeaponEquipmentId, 0, 600-m_rImageRects[TYPE_EQUIPMENT].bottom, 1.0f, 1.0f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedWeapon));
			m_pTM->Draw(m_nWeaponEquipmentId, m_rImageRects[TYPE_EQUIPMENT].right, 600-m_rImageRects[TYPE_EQUIPMENT].bottom, 1.0f, 1.0f, &GetRect(TYPE_EQUIPMENT, 20));
			
			m_HudFont.Draw("Player 1", 5, 64, 0.55f, -1 );
			m_HudFont.Draw(buffer, 110, 70, 0.75f, -1 );
			break;
		}
	case 1:
		{
			m_pTM->Draw(m_nHealthAndEnergyBarId, 800-m_rImageRects[TYPE_PORTRAIT].right, 0, -1, 1, &GetRect(0));
			m_pTM->Draw(m_nHealthAndEnergyBarId, 800-m_rImageRects[TYPE_PORTRAIT].right, m_rImageRects[0].bottom, -1, 1, &GetRect(1));
			m_pTM->Draw(m_nCharacterPortraitId, 800-m_rImageRects[TYPE_PORTRAIT].right, 0, 1, 1, &GetRect(2));
			m_pTM->Draw(m_nWeaponEquipmentId, 630, 35, 0.5f, 0.5f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedBootSlot));
			m_pTM->Draw(m_nWeaponEquipmentId, 670, 35, 0.5f, 0.5f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedHeadSlot));
			m_pTM->Draw(m_nWeaponEquipmentId, 800, 600-m_rImageRects[TYPE_EQUIPMENT].bottom, -1.0f, 1.0f, &GetRect(TYPE_EQUIPMENT, m_pPlayer->m_nSelectedWeapon));
			m_pTM->Draw(m_nWeaponEquipmentId, 800-m_rImageRects[TYPE_EQUIPMENT].right, 600-m_rImageRects[TYPE_EQUIPMENT].bottom, -1.0f, 1.0f, &GetRect(TYPE_EQUIPMENT, 20));
			
			m_HudFont.Draw("Player 2", 720, 64, 0.5f, -1 );
			m_HudFont.Draw(buffer, 510, 70, 0.75f, -1 );
			break;
		}
	}
}