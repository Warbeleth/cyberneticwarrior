#ifndef _CHUD_H_
#define _CHUD_H_

#include "CFont.h"
class CPlayer;
class CSGD_TextureManager;

#define ENERGY_DELAY 0.05f

class CHud
{
	// Singleton Pointer
	CSGD_TextureManager* m_pTM;

	// Player for health bars
	CPlayer* m_pPlayer;

	// Hud Image IDs
	int m_nHealthAndEnergyBarId;
	int m_nCharacterPortraitId;
	int m_nWeaponEquipmentId;

	// Hud Image RECTS
	RECT m_rImageRects[4];

	// Font
	CFont m_HudFont;

	// Settings
	int m_nPlayerNumber;

	// Timer
	float m_fElapsedTime;

	//WatermarkID
	int m_nWatermarkId;

	CHud(CHud&);
	CHud& operator=(CHud&);

	friend class CPlayer;

protected:
	enum Type { TYPE_HEALTH = 0, TYPE_ENERGY, TYPE_PORTRAIT, TYPE_EQUIPMENT };
public:
	void SetPlayerNumber( int nPlayerNumber ) { m_nPlayerNumber = nPlayerNumber; }
	void SetPlayerPointer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

	CHud( void );
	~CHud( void );

	RECT GetRect( int nType, int nEquipment = -1 );

	void Update( float fElapsedTime );
	void Render( void );
};

#endif