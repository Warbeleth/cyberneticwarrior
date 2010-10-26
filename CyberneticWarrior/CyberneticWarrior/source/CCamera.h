//////////////////////////////////////////////////////////////////////////////////////////////////////
//	File : CCamera.h
//
//	Author : Anthony Muccio / Corey Ringer / Patrick Alvarez 
//
//	Purpose : Camera class declarations.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CCAMERA_H_
#define CCAMERA_H_

class CCamera
{
	int m_nX;
	int m_nY;
	float m_fScale;

	CCamera( void );
	~CCamera( void );
	CCamera(CCamera&);
	CCamera& operator=(CCamera&);

	static CCamera* sm_pCameraInstance;
public:
	int SetCameraOffsetX( int nOffsetX ) { m_nX = nOffsetX; }
	int SetCameraOffsetY( int nOffsetY ) { m_nY = nOffsetY; }

	int GetOffsetX( void ) { return (int)(m_nX * m_fScale); }
	int GetOffsetY( void ) { return (int)(m_nY * m_fScale); }
	float GetScale( void ) { return m_fScale; }

	void ZoomIn( void ) { m_fScale += 0.1f; (m_fScale > 2) ? m_fScale = 2 : m_fScale = m_fScale;}
	void ZoomOut( void ) { m_fScale -= 0.1f; (m_fScale <= 0) ? m_fScale = 0.1f : m_fScale = m_fScale; }

	static CCamera* GetInstance( void );
	static void DeleteInstance( void );

};
#endif