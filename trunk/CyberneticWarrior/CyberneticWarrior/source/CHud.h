#ifndef _CHUD_H_
#define _CHUD_H_

#include "CFont.h"
class CSGD_TextureManager;

#define ENERGY_DELAY 0.05f

class CHud
{
	// Singleton Pointer
	CSGD_TextureManager* m_pTM;

	// Health
	float m_nRemainingHealth;
	float m_nTotalHealth;

	// Energy
	float m_nRemainingEnergy;
	float m_nTotalEnergy;

	// Score
	int m_nScore;

	// Hud Image IDs
	int m_nHealthAndEnergyBarId;
	int m_nCharacterPortraitId;

	// Hud Image Width / Height
	int m_nHealthAndEnergyBarWidth;
	int m_nPortraitWidth;
	int m_nPortraitHeight;
	int m_nBarHeight;

	// Font
	CFont m_HudFont;

	// Settings
	int m_nPlayerNumber;

	// Timer
	float m_fElapsedTime;

	CHud(CHud&);
	CHud& operator=(CHud&);

protected:
	enum Health { HP_HEALTHPACK = 30, DMG_SMALL = 15, DMG_MEDIUM = 30, DMG_LARGE = 45 };
	enum Energy { ENERGY_ROCKET = 100, ENERGY_DASH = 100, ENERGY_HOVER = 10 };
	enum Score  { SCORE_SMALL = 100, SCORE_MEDIUM = 250, SCORE_LARGE = 1000 };
public:
	void IncrementScore( int nScore ) { m_nScore += nScore; }
	void DecrementScore( int nScore ) { m_nScore -= nScore; }
	void IncrementHealth( float nRemainingHealth ) { m_nRemainingHealth += nRemainingHealth; (m_nRemainingHealth > 100.0f) ? m_nRemainingHealth = 100.0f : m_nRemainingHealth = m_nRemainingHealth; }
	void DecrementHealth( float nRemainingHealth ) { m_nRemainingHealth -= nRemainingHealth; (m_nRemainingHealth < 0.0f) ? m_nRemainingHealth = 0.0f : m_nRemainingHealth = m_nRemainingHealth; }
	void IncrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy += nRemainingEnergy; (m_nRemainingEnergy > 100.0f) ? m_nRemainingEnergy = 100.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
	void DecrementEnergy( float nRemainingEnergy ) { m_nRemainingEnergy -= nRemainingEnergy; (m_nRemainingEnergy < 0.0f) ? m_nRemainingEnergy = 0.0f : m_nRemainingEnergy = m_nRemainingEnergy; }
	void SetPlayer( int nPlayerNumber ) { m_nPlayerNumber = nPlayerNumber; }

	CHud( void );
	~CHud( void );

	RECT GetDisplayRect( int nHealthEnergyPortrait );

	void Update( float fElapsedTime );
	void Render( void );
};

#endif