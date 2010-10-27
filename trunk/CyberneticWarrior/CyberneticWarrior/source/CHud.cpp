#include "PrecompiledHeader.h"
#include "CHud.h"

CHud::CHud( void )
{
	// Singleton Pointer
	m_pTM = CSGD_TextureManager::GetInstance();

	// Health
	m_nRemainingHealth = 100;
	m_nTotalHealth = 100;

	// Energy
	m_nRemainingEnergy = 0;
	m_nTotalEnergy = 100;

	// Score
	m_nScore = 0;

	// Hud Image IDs
	m_nHealthAndEnergyBarId = -1;
	m_nHealthAndEnergyBarId = m_pTM->LoadTexture("resource/graphics/HealthAndEnergy.png");
	m_nCharacterPortraitId = -1;
	m_nCharacterPortraitId = m_pTM->LoadTexture("resource/graphics/Players.png");

	// Hud Image Width / Height
	m_nHealthAndEnergyBarWidth = 162;
	m_nBarHeight = 16;
	m_nPortraitWidth  = 96;
	m_nPortraitHeight = 96;


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

RECT CHud::GetDisplayRect( int nHealthEnergyPortrait )
{			
	RECT rRectangle;

	if( nHealthEnergyPortrait != 2 )
	{
		float fPercentage;
		rRectangle.left = 0;
		rRectangle.top = nHealthEnergyPortrait * m_nBarHeight;
		rRectangle.bottom = rRectangle.top + m_nBarHeight;

		switch(nHealthEnergyPortrait)
		{
		case 0:
			{
				fPercentage = m_nRemainingHealth / m_nTotalHealth;
				break;
			}
		case 1:
			{
				fPercentage = m_nRemainingEnergy / m_nTotalEnergy;
				break;
			}
		};

		rRectangle.right = (LONG)(fPercentage * m_nHealthAndEnergyBarWidth);
	}
	else
	{
		rRectangle.left = m_nPlayerNumber * m_nPortraitWidth;
		rRectangle.top = 0;
		rRectangle.right = rRectangle.left + m_nPortraitWidth;
		rRectangle.bottom = m_nPortraitHeight;
	}

	return rRectangle;
}

void CHud::Update( float fElapsedTime )
{
	m_fElapsedTime += fElapsedTime;

	if( m_fElapsedTime >= ENERGY_DELAY && m_nRemainingEnergy != m_nTotalEnergy )
	{
		m_fElapsedTime = 0.0f;
		IncrementEnergy( (float)( 1 ) );
	}
}

void CHud::Render( void )
{
	char buffer[32];
	sprintf_s(buffer, "Score: %i", m_nScore);

	switch(m_nPlayerNumber)
	{
	case 0:
		{
			m_pTM->Draw(m_nHealthAndEnergyBarId, m_nPortraitWidth, 0, 1, 1, &GetDisplayRect(0) );
			m_pTM->Draw(m_nHealthAndEnergyBarId, m_nPortraitWidth, m_nBarHeight, 1, 1, &GetDisplayRect(1));
			m_pTM->Draw(m_nCharacterPortraitId, 0, 0, 1, 1, &GetDisplayRect(2));
			
			m_HudFont.Draw("Player 1", 5, 64, 0.55f, -1 );
			m_HudFont.Draw(buffer, 110, 70, 0.75f, -1 );
			break;
		}
	case 1:
		{
			m_pTM->Draw(m_nHealthAndEnergyBarId, 800-m_nPortraitWidth, 0, -1, 1, &GetDisplayRect(0));
			m_pTM->Draw(m_nHealthAndEnergyBarId, 800-m_nPortraitWidth, m_nBarHeight, -1, 1, &GetDisplayRect(1));
			m_pTM->Draw(m_nCharacterPortraitId, 800-m_nPortraitWidth, 0, 1, 1, &GetDisplayRect(2));
			m_HudFont.Draw("Player 2", 720, 64, 0.5f, -1 );
			m_HudFont.Draw(buffer, 510, 70, 0.75f, -1 );
			break;
		}
	}
}