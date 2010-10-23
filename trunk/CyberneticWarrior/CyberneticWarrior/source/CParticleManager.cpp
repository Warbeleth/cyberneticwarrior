//////////////////////////////////////////////////////////////////////////////////////////////////////
// File: “CParticleManager.cpp”
//
// Author: Greg Stefanakis (GS)
//
// Purpose: This file hold the function bodies for CParticleManager, its Emitters, and the Emitter's Particles
//////////////////////////////////////////////////////////////////////////////////////////////////////
// SGD Wrappers
#include "SGD Wrappers/CSGD_TextureManager.h"
#include "SGD Wrappers/CSGD_Direct3D.h"

// My Includes
#include <fstream>
#include "CParticleManager.h"

//Instance in global space
CParticleManager CParticleManager::m_Instance;

///////////////////////
//CParticle Functions//
//////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CParticle”
//
// Purpose: Contstructor will default the initial values.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CParticle::CParticle()
{
	m_fAlpha = 0.0f;
	m_fAlphaModifier = 0.0f;
	m_fScale = 0.0f;
	m_fScaleModifier = 0.0f;
	m_fRotation = 0.0f;
	m_fRotationModifier = 0.0f;
	m_fLifetime = 0.0f;
	m_fAge = 0.0f;
	m_tPos.fX = 0;
	m_tPos.fY = 0;
	m_tOrigin.fX = 0;
	m_tOrigin.fY = 0;
	m_tVel.fX = 0;
	m_tVel.fY = 0;
	m_dwColor = -1;
	m_fRedModifier = 0;
	m_fGreenModifier = 0;
	m_fBlueModifier = 0;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;

	TM = CSGD_TextureManager::GetInstance();
	D3D = CSGD_Direct3D::GetInstance();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CParticle”
//
// Purpose: Destructor will take care of any shutdown functionality.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CParticle::~CParticle()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: This function will update the particles data based on the elapsed time.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticle::Update( float fElapsedTime )
{
	m_fAge += fElapsedTime;

	//Alpha Change
	m_fAlpha += (m_fAlphaModifier * fElapsedTime);

	//Scale Change
	m_fScale += (m_fScaleModifier * fElapsedTime);

	//Rotate Change
	m_fRotation += (m_fRotationModifier * fElapsedTime);

	//Need to calculate color change here

	//Velocity changing position
	m_tPos.fX += (m_tVel.fX * fElapsedTime);
	m_tPos.fY += (m_tVel.fY * fElapsedTime);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: This function will render the particle to the screen.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticle::Render( int nImageID )
{
    //Info divided up to clean up the function call
    RECT rDrawRect;
	rDrawRect.left = 0;
	rDrawRect.top = 0;
	rDrawRect.right = (int)m_fWidth;
	rDrawRect.bottom = (int)m_fHeight;

	//Get the center of the rectangle
    float rotCenterX = (m_fWidth * m_fScale) / 2.0f;
    float rotCenterY = (m_fHeight * m_fScale) / 2.0f;

	//Offset the drawing so it draws the center of the rectangle on the position, not the top-left corner
    int nOffsetPosX = (int)m_tPos.fX - (int)rotCenterX;
    int nOffsetPosY = (int)m_tPos.fY - (int)rotCenterY;

	//Draw Call
	TM->Draw( nImageID, nOffsetPosX, nOffsetPosY, m_fScale, m_fScale, &rDrawRect, rotCenterX, rotCenterY, m_fRotation, m_dwColor );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “SetParticleInfo”
//
// Purpose: This function will act as a big modifier and set all the particle info
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticle::SetParticleInfo( float fAlpha, float fAlphaMod, float fScale, float fScaleMod, float fRotation, 
						float fRotationMod, float fLifetime, float fAge, float fPosX, float fPosY, 
						float fOriginX, float fOriginY, float fVelX, float fVelY, char cRed, 
						float fRedMod, char cGreen, float fGreenMod, char cBlue, float fBlueMod, 
						float fWidth, float fHeight)
{
	m_fAlpha = fAlpha;
	m_fAlphaModifier = fAlphaMod;
	m_fScale = fScale;
	m_fScaleModifier = fScaleMod;
	m_fRotation = fRotation;
	m_fRotationModifier = fRotationMod;
	m_fLifetime = fLifetime;
	m_fAge = fAge;
	m_tPos.fX = fPosX;
	m_tPos.fY = fPosY;
	m_tOrigin.fX = fOriginX;
	m_tOrigin.fY = fOriginY;
	m_tVel.fX = fVelX;
	m_tVel.fY = fVelY;
	m_dwColor = D3DCOLOR_ARGB(255,cRed,cGreen,cBlue);
	m_fRedModifier = fRedMod;
	m_fGreenModifier = fGreenMod;
	m_fBlueModifier = fBlueMod;
	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

/////////////////////
//Emitter Functions//
////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CEmitter”
//
// Purpose: The constructor will initialize default values.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CEmitter::CEmitter()
{
	m_vLivingParticles.clear();
	m_vDeadParticles.clear();
	
	m_nNumParticles = 0;
	m_tEmitterPos.fX = 128;
	m_tEmitterPos.fY = 128;
	m_nParticleTextureID = -1;
	m_fEmitterLifetime = 0.0f;
	m_fEmitterAge = 0.0f;
	m_fEmitterAge = 0.0f;
	m_nSourceBlend = 0;
	m_nDestinationBlend = 0;

	TM = CSGD_TextureManager::GetInstance();
	D3D = CSGD_Direct3D::GetInstance();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CEmitter”
//
// Purpose: The destructor will clear up any memory that was created in the class's lifetime.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CEmitter::~CEmitter()
{
	m_vLivingParticles.clear();
	m_vDeadParticles.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: This function will call update on all of it's particles.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::Update( float fElapsedTime )
{
	//Update both vectors of particles
	for( unsigned int i = 0; i < m_vLivingParticles.size(); i++ )
	{
		m_vLivingParticles[i].Update(fElapsedTime);
	}

	for( unsigned int i = 0; i < m_vDeadParticles.size(); i++ )
	{
		m_vDeadParticles[i].Update(fElapsedTime);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: This function will call render on all of it's particles.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::Render()
{
	for( unsigned int i = 0; i < m_vLivingParticles.size(); i++ )
	{
		m_vLivingParticles[i].Render( m_nParticleTextureID );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “SetEmitterValues”
//
// Purpose: This function will send all the information gathered from the binary load to the emitter.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CEmitter::SetEmitterValues( string szParticleFilename, int nNumParticles, float fEmitterLifetime, int nSourceBlend, int nDestBlend)
{
	//Load the texture and allign the appropriate texture ID

	//Hardcoding the file path for testing
	m_nParticleTextureID = TM->LoadTexture( "resource/graphics/smoke2.jpg", -1 );

	//Rest of the info
	m_nNumParticles = nNumParticles;
	m_fEmitterLifetime = fEmitterLifetime;
	m_nSourceBlend = nSourceBlend;
	m_nDestinationBlend = nDestBlend;
}


//////////////////////////////
//CParticleManager Functions//
/////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “CParticleManager”
//
// Purpose: Constructor for CParticleManager.  This will initialize any values to default
//////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::CParticleManager()
{
	m_vEmitters.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “~CParticleManager”
//
// Purpose: Destructor for CParticleManager.  This will clear memory in the vector.
//////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager::~CParticleManager()
{
	m_vEmitters.clear();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “GetInstance”
//
// Purpose: This function will return the singleton instance of this class
//////////////////////////////////////////////////////////////////////////////////////////////////////
CParticleManager* CParticleManager::GetInstance()
{
	return &m_Instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Update”
//
// Purpose: This function will call update on all of it's emitters.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::Update( float fElapsedTime )
{
	for( unsigned int i = 0; i < m_vEmitters.size(); i++ )
	{
		m_vEmitters[i].Update(fElapsedTime);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “Render”
//
// Purpose: This function will call render on all of it's emitters.
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CParticleManager::Render()
{
	for( unsigned int i = 0; i < m_vEmitters.size(); i++ )
	{
		m_vEmitters[i].Render();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Function: “LoadEffect”
//
// Purpose: This function will create a new emitter, load in binary information, and put all important
//			values into the emitter and particles.  It will return a boolean: true for successful load.
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CParticleManager::LoadEffect( char* szEffectFileName )
{
	std::ifstream in;
	in.clear();

	in.open( szEffectFileName, std::ios_base::in | std::ios_base::binary);

	if(in.is_open())
	{
		//Get that input
		//I have a giant binary file of information that is mostly unneeded
		//but there are some important things in it that i have to get, so ill just input everything
		//in order so i dont get lost in the file

//    //Particle Effect Name
		char cStringLength;
		in.read( (char*)&cStringLength, sizeof(char));
		int nStringLength = (int)cStringLength;

		string szParticleEffectName;
		char* buffer = new char[nStringLength+1];
		in.read( (char*)buffer, nStringLength);
		buffer[nStringLength] = '\0';
		szParticleEffectName = buffer;
		delete buffer;

		//Emitter Lifetime
		float fEmitterLifetime = 0.0f;
		in.read( (char*)&fEmitterLifetime, sizeof(float));

		//Emitter Looping Boolean
		bool bLooping = false;
		in.read( (char*)&bLooping, sizeof(bool));

//    //Emitter Spawn Range
		bool bSingleOrigin;
		in.read( (char*)&bSingleOrigin, sizeof(bool));

//    //Circle
		bool bCircle;
		in.read( (char*)&bCircle, sizeof(bool));

		float fCircleRadius;
		in.read( (char*)&fCircleRadius, sizeof(float));

//    //Rectangle
		bool bRectangle;
		in.read( (char*)&bRectangle, sizeof(bool));

		float fRectangleWidth;
		in.read( (char*)&fRectangleWidth, sizeof(float));

		float fRectangleHeight;
		in.read( (char*)&fRectangleHeight, sizeof(float));

//    //Line
		bool bLine;
		in.read( (char*)&bLine, sizeof(bool));

		float fLineLength;
		in.read( (char*)&fLineLength, sizeof(float));

//    //Texture Size
		float fTextureWidth;
		in.read( (char*)&fTextureWidth, sizeof(float));

		float fTextureHeight;
		in.read( (char*)&fTextureHeight, sizeof(float));

//    //Spread Angle
		float fSpreadAngle;
		in.read( (char*)&fSpreadAngle, sizeof(float));

		float fDirection;
		in.read( (char*)&fDirection, sizeof(float));

//    //Number of Particles
		int nNumParticles;
		in.read( (char*)&nNumParticles, sizeof(int));

//    //Source Blend
		int nSourceBlend;
		in.read( (char*)&nSourceBlend, sizeof(int));

//    //Destination Blend
		int nDestBlend;
		in.read( (char*)&nDestBlend, sizeof(int));

//    //Now Particle Stuff
//    //Particle Texture name
		char cStringLength1;
		in.read( (char*)&cStringLength1, sizeof(char));
		int nStringLength1 = (int)cStringLength1;

		string szParticleTextureName;
		char* buffer1 = new char[nStringLength1+1];
		in.read( (char*)buffer1, nStringLength1);
		buffer1[nStringLength1] = '\0';
		szParticleTextureName = buffer1;
		delete buffer1;

//    //Lifetime
		float fMinLifetime;
		in.read( (char*)&fMinLifetime, sizeof(float));

		float fMaxLifetime;
		in.read( (char*)&fMaxLifetime, sizeof(float));

		float fTime;
		in.read( (char*)&fTime, sizeof(float));

//    //Lifetime checkbox
		bool bLifeTimeRandom;
		in.read( (char*)&bLifeTimeRandom, sizeof(bool));

//    //Colors
		char cStartRed = 0;
		in.read( (char*)&cStartRed, sizeof(char));

		char cStartGreen = 0;
		in.read( (char*)&cStartGreen, sizeof(char));

		char cStartBlue = 0;
		in.read( (char*)&cStartBlue, sizeof(char));

		char cEndRed = 0;
		in.read( (char*)&cEndRed, sizeof(char));

		char cEndGreen= 0;
		in.read( (char*)&cEndGreen, sizeof(char));

		char cEndBlue = 0;
		in.read( (char*)&cEndBlue, sizeof(char));

//    //Interp Color Checkbox
		bool bInterpColor = false;
		in.read( (char*)&bInterpColor, sizeof(bool));

//    //Randomize Color checkbox 
		bool bRandomizeColor = false;
		in.read( (char*)&bRandomizeColor, sizeof(bool));

//    //Alpha
		float fAlphaStart;
		in.read( (char*)&fAlphaStart, sizeof(float));

		float fAlphaEnd;
		in.read( (char*)&fAlphaEnd, sizeof(float));

//    //Interp Alpha Checkbox
		bool bInterpAlpha = false;
		in.read( (char*)&bInterpAlpha, sizeof(bool));

//    //Randomize Alpha Checkbox
		bool bRandomizeAlpha = false;
		in.read( (char*)&bRandomizeAlpha, sizeof(bool));

//    //Scale 
		float fMinScale;
		in.read( (char*)&fMinScale, sizeof(float));

		float fMaxScale;
		in.read( (char*)&fMaxScale, sizeof(float));

//    //Interp Scale Checkbox
		bool bInterpScale = false;
		in.read( (char*)&bInterpScale, sizeof(bool));

//    //Randomize Scale Checkbox
		bool bRandomizeScale = false;
		in.read( (char*)&bRandomizeScale, sizeof(bool));

//    //Rotation
		float fMinRotation;
		in.read( (char*)&fMinRotation, sizeof(float));

		float fMaxRotation;
		in.read( (char*)&fMaxRotation, sizeof(float));

		float fRotation;
		in.read( (char*)&fRotation, sizeof(float));

//    //Rotation Randomize Checkbox
		bool bRandomizeRotation = false;
		in.read( (char*)&bRandomizeRotation, sizeof(bool));

//    //Particle Speed
		float fMinSpeed;
		in.read( (char*)&fMinSpeed, sizeof(float));

		float fMaxSpeed;
		in.read( (char*)&fMaxSpeed, sizeof(float));

		float fSpeed;
		in.read( (char*)&fSpeed, sizeof(float));

//    //Randomize Speed Checkbox
		bool bRandomizeSpeed = false;
		in.read( (char*)&bRandomizeSpeed, sizeof(bool));

		//Now that we got the emitter information and all the other crap out of the way, i can just input
		//particle information.  Set up the emitter and start loading particle info into it.
		CEmitter* pEmitter = new CEmitter;
		pEmitter->SetEmitterValues( szParticleTextureName, nNumParticles, fEmitterLifetime, nSourceBlend, nDestBlend );
		for( int i = 0; i < nNumParticles; i++ )
		{
			float fAlpha = 0;
			in.read( (char*)&fAlpha, sizeof(float));

			float fAlphaModifier = 0;
			in.read( (char*)&fAlphaModifier, sizeof(float));

			float fScale = 0;
			in.read( (char*)&fScale, sizeof(float));

			float fScaleModifier = 0;
			in.read( (char*)&fScaleModifier, sizeof(float));

			float fRotation = 0;
			in.read( (char*)&fRotation, sizeof(float));

			float fRotationModifier = 0;
			in.read( (char*)&fRotationModifier, sizeof(float));

			float fLifetime = 0;
			in.read( (char*)&fLifetime, sizeof(float));
			
			float fAge = 0;
			in.read( (char*)&fAge, sizeof(float));

			float fPosX = 0;
			in.read( (char*)&fPosX, sizeof(float));

			float fPosY = 0;
			in.read( (char*)&fPosY, sizeof(float));

			float fOriginX = 0;
			in.read( (char*)&fOriginX, sizeof(float));

			float fOriginY = 0;
			in.read( (char*)&fOriginY, sizeof(float));

			float fVelX = 0;
			in.read( (char*)&fVelX, sizeof(float));

			float fVelY = 0;
			in.read( (char*)&fVelY, sizeof(float));

			char cRed = 0;
			in.read( (char*)&cRed, sizeof(char));
			float fRedModifier = 0;
			in.read( (char*)&fRedModifier, sizeof(float));

			char cGreen = 0;
			in.read( (char*)&cGreen, sizeof(char));
			float fGreenModifier = 0;
			in.read( (char*)&fGreenModifier, sizeof(float));

			char cBlue = 0;
			in.read( (char*)&cBlue, sizeof(char));
			float fBlueModifier = 0;
			in.read( (char*)&fBlueModifier, sizeof(float));

			float fWidth = 0;
			in.read( (char*)&fWidth, sizeof(float));
			float fHeight = 0;
			in.read( (char*)&fHeight, sizeof(float));

			//Create the new particle
			CParticle* pParticle = new CParticle;
			//Set all the info
			pParticle->SetParticleInfo( fAlpha, fAlphaModifier, fScale, fScaleModifier, fRotation, fRotationModifier,
						fLifetime, fAge, fPosX, fPosY, fOriginX, fOriginY, fVelX, fVelY, cRed, fRedModifier, cGreen, 
						fGreenModifier, cBlue, fBlueModifier, fWidth, fHeight);
			//Add this new particle to the list ( start them all dead )
			pEmitter->GetDeadParticleVector().push_back( *pParticle );

		}
		//Then put the entire emitter on the CParticleManager vector
		CParticleManager::GetInstance()->m_vEmitters.push_back( *pEmitter );

		//Close the file
		in.close();


		return true;
	}
	//If the file isnt opened, it will return false
	return false;
}


