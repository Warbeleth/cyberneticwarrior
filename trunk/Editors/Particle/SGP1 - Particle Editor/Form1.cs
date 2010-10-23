using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Text;
using System.Windows.Forms;
using SGD;

namespace SGP1___Particle_Editor
{

    public partial class Form1 : Form
    {
        enum eSpawnRange { ORIGIN = 0, CIRCLE, LINE, RECT };
        string SavePath = @"C:\Users\Greg\Desktop\ParticleSave.txt";

        //Particle Emitter
        class CEmitter
        {
            //Blenders
            Microsoft.DirectX.Direct3D.Blend eSourceBlend;
            public Microsoft.DirectX.Direct3D.Blend SourceBlend
            {
                get { return eSourceBlend; }
                set { eSourceBlend = value; }
            }

            Microsoft.DirectX.Direct3D.Blend eDestBlend;
            public Microsoft.DirectX.Direct3D.Blend DestBlend
            {
                get { return eDestBlend; }
                set { eDestBlend = value; }
            }

            //String for the particle texture name
            string m_szParticleName;
            public string szParticleName
            {
                get { return m_szParticleName; }
                set { m_szParticleName = value; }
            }

            //String for the effect name
            string m_szSystemName;
            public string szSystemName
            {
                get { return m_szSystemName; }
                set { m_szSystemName = value; }
            }

            //String for the particle texture path
            string m_szParticleTexturePath;
            public string szParticleTexturePath
            {
                get { return m_szParticleTexturePath; }
                set { m_szParticleTexturePath = value; }
            }

            //Array of particles
            CParticle[] m_pParticleArray;

            //wtf is internal
            internal CParticle[] pParticleArray
            {
                get { return m_pParticleArray; }
                set { m_pParticleArray = value; }
            }

            //Origin of emitter
            float m_fEmitterPosX;
            public float fEmitterPosX
            {
                get { return m_fEmitterPosX; }
                set { m_fEmitterPosX = value; }
            }

            float m_fEmitterPosY;
            public float fEmitterPosY
            {
                get { return m_fEmitterPosY; }
                set { m_fEmitterPosY = value; }
            }

            //Control the direction and spread
            int m_nSpreadAngle;
            public int nSpreadAngle
            {
                get { return m_nSpreadAngle; }
                set { m_nSpreadAngle = value; }
            }

            int m_nDirection;
            public int nDirection
            {
                get { return m_nDirection; }
                set { m_nDirection = value; }
            }

            //Number of Particles
            int m_nNumParticles;
            public int nNumParticles
            {
                get { return m_nNumParticles; }
                set { m_nNumParticles = value; }
            }

            //Textures
            int m_nParticleTexture = -1;
            public int nParticleTexture
            {
                get { return m_nParticleTexture; }
                set { m_nParticleTexture = value; }
            }

            //Particle Speed
            float m_fMinSpeed;
            public float fMinSpeed
            {
                get { return m_fMinSpeed; }
                set { m_fMinSpeed = value; }
            }

            float m_fMaxSpeed;
            public float fMaxSpeed
            {
                get { return m_fMaxSpeed; }
                set { m_fMaxSpeed = value; }
            }

            //Enum for spawn range
            int m_nSpawnRange;
            public int nSpawnRange
            {
                get { return m_nSpawnRange; }
                set { m_nSpawnRange = value; }
            }

            ////Booleans
            bool m_bLooping;
            public bool bLooping
            {
                get { return m_bLooping; }
                set { m_bLooping = value; }
            }

            //Emitter Lifetime
            float m_fEmitterLifetime;
            public float fEmitterLifetime
            {
                get { return m_fEmitterLifetime; }
                set { m_fEmitterLifetime = value; }
            }

            float m_fAge;
            public float fAge
            {
                get { return m_fAge; }
                set { m_fAge = value; }
            }

            //Functions
            public CEmitter()
            {
                this.m_nParticleTexture = -1;
                this.m_nNumParticles = 0;

                //Defaulting emitter to start at this location
                this.m_fEmitterPosX = 256.0f;
                this.m_fEmitterPosY = 256.0f;

                this.m_nSpawnRange = (int)eSpawnRange.RECT;
            }

            public void Update(float fElapsedTime)
            {
                //Calculate the age each frame
                m_fAge += fElapsedTime;

                //Check if the emitter is looping and reset the life so its always alive
                if (m_fAge > m_fEmitterLifetime)
                {
                    if (m_bLooping == true)
                        m_fAge = 0.0f;
                }

                if (nParticleTexture != -1)
                {
                    for (int i = 0; i < this.nNumParticles; i++)
                    {
                        //Reset the particle 
                        if (m_pParticleArray[i].fAge > m_pParticleArray[i].fLifetime)
                        {
                            //Reset the particle and keep rendering
                            if (m_bLooping == true)
                            {
                                m_pParticleArray[i].fAge = 0.0f;
                                m_pParticleArray[i].bAlive = true;
                                m_pParticleArray[i].fAlpha = m_pParticleArray[i].nStartAlpha;
                                m_pParticleArray[i].fScale = m_pParticleArray[i].fStartScale;
                                m_pParticleArray[i].tColor = Color.FromArgb((int)m_pParticleArray[i].fAlpha, m_pParticleArray[i].tStartColor);
                                m_pParticleArray[i].fPosX = m_pParticleArray[i].fOriginX;
                                m_pParticleArray[i].fPosY = m_pParticleArray[i].fOriginY;
                            }
                            //Reset the particle and do a check 
                            else if (m_bLooping == false)
                            {
                                //If emitter is still alive ( within the emitter lifetime )
                                //keep the particle alive until the emitter dies.
                                if (m_fAge < m_fEmitterLifetime)
                                {
                                    m_pParticleArray[i].bAlive = true;
                                }
                                else
                                {
                                    m_pParticleArray[i].bAlive = false;
                                }

                                //Reset everything else normally
                                m_pParticleArray[i].fAge = 0.0f;
                                m_pParticleArray[i].fAlpha = m_pParticleArray[i].nStartAlpha;
                                m_pParticleArray[i].fScale = m_pParticleArray[i].fStartScale;
                                m_pParticleArray[i].tColor = Color.FromArgb((int)m_pParticleArray[i].fAlpha, m_pParticleArray[i].tStartColor);
                                m_pParticleArray[i].fPosX = m_pParticleArray[i].fOriginX;
                                m_pParticleArray[i].fPosY = m_pParticleArray[i].fOriginY;
                            }
                        }

                        m_pParticleArray[i].Update(fElapsedTime);
                    }
                }
            }

            public void Render()
            {
                if (nParticleTexture != -1)
                {
                    //Reset and set up the Blend stuff
                    ManagedDirect3D.Instance.SpriteEnd();

                    //Source and Dest blendmodes
                    ManagedDirect3D.Instance.Device.RenderState.AlphaSourceBlend = eSourceBlend;
                    ManagedDirect3D.Instance.Device.RenderState.AlphaDestinationBlend = eDestBlend;
                    
                    ManagedDirect3D.Instance.SpriteBegin();
                    
                    //Call Render on each particle
                    for (int i = 0; i < this.nNumParticles; i++)
                    {
                        this.m_pParticleArray[i].Render();
                    }
                }
            }

            public void SetParticleTexture(int nTexture)
            {
                this.nParticleTexture = nTexture;
                for (int i = 0; i < this.nNumParticles; i++)
                {
                    this.m_pParticleArray[i].nTexture = nTexture;
                }
            }
        };

        //Particle
        class CParticle
        {

            //int for texture
            int m_nTexture;

            public int nTexture
            {
                get { return m_nTexture; }
                set { m_nTexture = value; }
            }

            //Alpha
            float m_fAlpha;
            public float fAlpha
            {
                get { return m_fAlpha; }
                set { m_fAlpha = value; }
            }

            float m_fAlphaModifier;
            public float fAlphaModifier
            {
                get { return m_fAlphaModifier; }
                set { m_fAlphaModifier = value; }
            }

            int m_nStartAlpha;
            public int nStartAlpha
            {
                get { return m_nStartAlpha; }
                set { m_nStartAlpha = value; }
            }

            int m_nEndAlpha;
            public int nEndAlpha
            {
                get { return m_nEndAlpha; }
                set { m_nEndAlpha = value; }
            }

            //Scales
            float m_fScale;
            public float fScale
            {
                get { return m_fScale; }
                set { m_fScale = value; }
            }

            float m_fStartScale;
            public float fStartScale
            {
                get { return m_fStartScale; }
                set { m_fStartScale = value; }
            }

            float m_fEndScale;
            public float fEndScale
            {
                get { return m_fEndScale; }
                set { m_fEndScale = value; }
            }

            float m_fScaleModifier;
            public float fScaleModifier
            {
                get { return m_fScaleModifier; }
                set { m_fScaleModifier = value; }
            }

            //Rotations
            float m_fRotation;
            public float fRotation
            {
                get { return m_fRotation; }
                set { m_fRotation = value; }
            }

            float m_fStartRotation;
            public float fStartRotation
            {
                get { return m_fStartRotation; }
                set { m_fStartRotation = value; }
            }

            float m_fEndRotation;
            public float fEndRotation
            {
                get { return m_fEndRotation; }
                set { m_fEndRotation = value; }
            }

            float m_fRotationModifier;
            public float fRotationModifier
            {
                get { return m_fRotationModifier; }
                set { m_fRotationModifier = value; }
            }

            //Lifetimes
            float m_fMinLifetime;
            public float fMinLifetime
            {
                get { return m_fMinLifetime; }
                set { m_fMinLifetime = value; }
            }

            float m_fMaxLifetime;
            public float fMaxLifetime
            {
                get { return m_fMaxLifetime; }
                set { m_fMaxLifetime = value; }
            }

            float m_fLifetime;
            public float fLifetime
            {
                get { return m_fLifetime; }
                set { m_fLifetime = value; }
            }

            //Age
            float m_fAge;
            public float fAge
            {
                get { return m_fAge; }
                set { m_fAge = value; }
            }

            //Particle Position
            float m_fPosX;

            public float fPosX
            {
                get { return m_fPosX; }
                set { m_fPosX = value; }
            }
            float m_fPosY;

            public float fPosY
            {
                get { return m_fPosY; }
                set { m_fPosY = value; }
            }

            //Particle Velocity
            float m_fVelX;
            public float fVelX
            {
                get { return m_fVelX; }
                set { m_fVelX = value; }
            }

            float m_fVelY;
            public float fVelY
            {
                get { return m_fVelY; }
                set { m_fVelY = value; }
            }

            //Particle Color
            Color m_tColor;
            public Color tColor
            {
                get { return m_tColor; }
                set { m_tColor = value; }
            }

            Color m_tStartColor;
            public Color tStartColor
            {
                get { return m_tStartColor; }
                set { m_tStartColor = value; }
            }

            Color m_tEndColor;
            public Color tEndColor
            {
                get { return m_tEndColor; }
                set { m_tEndColor = value; }
            }

            float m_fRedModifier;
            public float fRedModifier
            {
                get { return m_fRedModifier; }
                set { m_fRedModifier = value; }
            }

            float m_fGreenModifier;
            public float fGreenModifier
            {
                get { return m_fGreenModifier; }
                set { m_fGreenModifier = value; }
            }

            float m_fBlueModifier;
            public float fBlueModifier
            {
                get { return m_fBlueModifier; }
                set { m_fBlueModifier = value; }
            }

            //Positions
            float m_fOriginX;
            public float fOriginX
            {
                get { return m_fOriginX; }
                set { m_fOriginX = value; }
            }

            float m_fOriginY;
            public float fOriginY
            {
                get { return m_fOriginY; }
                set { m_fOriginY = value; }
            }

            //Texture rectangle
            float m_fWidth;
            public float fWidth
            {
                get { return m_fWidth; }
                set { m_fWidth = value; }
            }

            float m_fHeight;
            public float fHeight
            {
                get { return m_fHeight; }
                set { m_fHeight = value; }
            }

            //Booleans
            bool m_bAlive;
            public bool bAlive
            {
                get { return m_bAlive; }
                set { m_bAlive = value; }
            }

            //Blending
            //source color * source blend + dest color * dest blend

            //Functions
            public CParticle(float fEmitterPosX, float fEmitterPosY)
            {

                m_fPosX = fEmitterPosX;
                m_fPosY = fEmitterPosY;

                m_fOriginX = fEmitterPosX;
                m_fOriginY = fEmitterPosY;

                //Lifetime defaults
                m_fMinLifetime = 0.0f;
                m_fMaxLifetime = 0.0f;

                //Alpha defaults
                m_fAlpha = 255.0f;
                m_nStartAlpha = 255;
                m_nEndAlpha = 255;
                m_fAlphaModifier = 0.0f;

                //Scale Defaults
                m_fScale = 1.0f;
                m_fScaleModifier = 0.0f;
                m_fStartScale = 1.0f;
                m_fEndScale = 1.0f;

                //Rotation Defaults
                m_fRotation = 0.0f;
                m_fStartRotation = 0.0f;
                m_fEndRotation = 0.0f;
                m_fRotationModifier = 0.0f;

                //Color Defaults
                m_tColor = Color.White;

                //Default Rectangle
                m_fWidth = 0.0f;
                m_fHeight = 0.0f;

                //Age
                m_fAge = 0.0f;
                m_bAlive = false;

                //Texture
                m_nTexture = -1;
            }

            public void Update(float fElapsedTime)
            {
                m_fAge += fElapsedTime;

                //If the particle is alive
                if (m_bAlive == true)
                {
                    //Alpha subtraction amount
                    m_fAlpha += (m_fAlphaModifier * fElapsedTime);

                    //Scale amount
                    m_fScale += (m_fScaleModifier * fElapsedTime);

                    //Rotate amount
                    m_fRotation += (m_fRotationModifier * fElapsedTime);

                    //Velocity changing position
                    m_fPosX += (this.m_fVelX * fElapsedTime);
                    m_fPosY += (this.m_fVelY * fElapsedTime);

                    //Color change
                    float R = ((float)m_tColor.R + (m_fRedModifier * fElapsedTime));
                    float G = ((float)m_tColor.G + (m_fGreenModifier * fElapsedTime));
                    float B = ((float)m_tColor.B + (m_fBlueModifier * fElapsedTime));

                    //Checks to make sure the colors dont go to invalid numbers
                    if (R > 255) { R = 255; }
                    if (R < 0) { R = 0; }
                    if (G > 255) { G = 255; }
                    if (G < 0) { G = 0; }
                    if (B > 255) { B = 255; }
                    if (B < 0) { B = 0; }
                    if (m_fAlpha > 255) { m_fAlpha = 255; }
                    if (m_fAlpha < 0) { m_fAlpha = 0; }
                    if (m_fScale < 0) { m_fScale = 0.0f; }

                    m_tColor = Color.FromArgb((int)m_fAlpha, (int)R, (int)G, (int)B);
                }
            }

            public void Render()
            {
                if (m_bAlive == true && m_fAlpha > 0)
                {
                    //Info divided up to clean up the function call
                    Rectangle tRect = new Rectangle(0, 0, (int)m_fWidth, (int)m_fHeight);
                    int rotCenterX = (int)(((float)tRect.Width / 2.0f) * m_fScale);
                    int rotCenterY = (int)(((float)tRect.Height / 2.0f) * m_fScale);
                    Color col = Color.FromArgb((int)m_fAlpha, m_tColor.R, m_tColor.G, m_tColor.B);
                    int nOffsetPosX = (int)m_fPosX - rotCenterX;
                    int nOffsetPosY = (int)m_fPosY - rotCenterY;
                    ManagedTextureManager.Instance.Draw(m_nTexture, nOffsetPosX, nOffsetPosY, m_fScale, m_fScale, tRect, rotCenterX, rotCenterY, m_fRotation, col.ToArgb());
                }
            }
        };

        //timing
        long m_dwNewTimeStamp;
        long m_dwPreviousTimeStamp;
        float m_fElapsedTime;

        //Data Members
        bool m_bLooping;
        public bool bLooping
        {
            get { return m_bLooping; }
            set { m_bLooping = value; }
        }

        //DirectX Objects
        ManagedDirect3D m_D3D;
        ManagedTextureManager m_TM;

        //Emitter for effect
        CEmitter m_pEmitter;

        //Workspace for relative file path
        string m_szWorkSpace;

        public Form1()
        {
            InitializeComponent();

            //Initialize Timing
            m_dwNewTimeStamp = Environment.TickCount;
            m_dwPreviousTimeStamp = Environment.TickCount;
            m_fElapsedTime = 0.0f;

            this.bLooping = true;

            this.m_D3D = ManagedDirect3D.Instance;
            this.m_D3D.InitManagedDirect3D(this.panel1, true);

            this.m_TM = ManagedTextureManager.Instance;
            this.m_TM.InitManagedTextureManager(this.m_D3D.Device, this.m_D3D.Sprite);

            //initialize particle effect
            m_pEmitter = new CEmitter();

            //Set up relative file path
            FolderBrowserDialog dlg = new FolderBrowserDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                m_szWorkSpace = dlg.SelectedPath + "\\";
            }
        }

        public void TimeUpdate()
        {
            ///////////////////////////////////////////
            //	Calculate the elapsed time
            ///////////////////////////////////////////
            m_dwNewTimeStamp = Environment.TickCount;
            m_fElapsedTime = (float)(m_dwNewTimeStamp - m_dwPreviousTimeStamp) / 1000.0f;
            m_dwPreviousTimeStamp = m_dwNewTimeStamp;

            m_pEmitter.Update(m_fElapsedTime);
        }

        public void Render()
        {
            this.m_D3D.Clear(this.panel1, 0, 0, 0);
            this.m_D3D.DeviceBegin();

            this.m_D3D.SpriteBegin();

            //Draw texture stuff within here
            this.m_pEmitter.Render();

            this.m_D3D.SpriteEnd();

            //Draw any D3D drawings here
            //this.m_D3D.DrawRect(new Rectangle(0, 0, 64, 64), 255, 0, 0);
            //this.m_D3D.DrawLine(0, 0, 128, 128, 0, 0, 0);

            this.m_D3D.DeviceEnd();
            this.m_D3D.Present(this.panel1);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.bLooping = false;
            //Have to manually exit the program this way?
            this.Close();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.bLooping = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files(*.*)|*.*";
            dlg.FilterIndex = 1;
            dlg.InitialDirectory = m_szWorkSpace;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                //Add the safe file name to the already established work space
                string FilePath = m_szWorkSpace + dlg.SafeFileName;
                
                //Non-relative path way
                //m_pEmitter.SetParticleTexture(m_TM.LoadTexture(dlg.FileName, Color.Black.ToArgb()));
                //m_pEmitter.szParticleTexturePath = dlg.FileName;

                //Set all particle textures
                m_pEmitter.SetParticleTexture( m_TM.LoadTexture(FilePath, Color.Black.ToArgb()));

                //Set the emitter to aim at the filepath of the texture( this is for Loading saved files )
                m_pEmitter.szParticleTexturePath = FilePath;
            }

            textBox2.Text = dlg.SafeFileName;
        }

        //Emitter Number of Particles via numericUpDown box
        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            //Check if i accidentally manually set it to an invalid number
            if ((int)numericUpDown1.Value > 256)
                numericUpDown1.Value = 256;
            if ((int)numericUpDown1.Value < 0)
                numericUpDown1.Value = 0;

            this.m_pEmitter.nNumParticles = (int)numericUpDown1.Value;
            trackBar3.Value = (int)numericUpDown1.Value;

            //Create the new particle array
            this.m_pEmitter.pParticleArray = new CParticle[m_pEmitter.nNumParticles];

            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                //Create each new instance of a particle
                m_pEmitter.pParticleArray[i] = new CParticle(m_pEmitter.fEmitterPosX, m_pEmitter.fEmitterPosY);

                //Set their textures
                m_pEmitter.pParticleArray[i].nTexture = m_pEmitter.nParticleTexture;

                //set up the random # generator
                Random rand = new Random(Environment.TickCount + i);

                //Get speed values
                m_pEmitter.fMinSpeed = (float)numericUpDown11.Value;
                m_pEmitter.fMaxSpeed = (float)numericUpDown13.Value;

                //Set up speed
                bool bRandomSpeed = checkBox11.Checked;
                int nSpeed = 0;

                //Randomize 2 elements
                if (bRandomSpeed == true)
                {
                    //Randomize between the two min and max speeds
                    nSpeed = rand.Next((int)m_pEmitter.fMinSpeed, (int)m_pEmitter.fMaxSpeed);
                }
                //Set static speed
                else if (bRandomSpeed == false)
                {
                    //Single speed value
                    nSpeed = (int)numericUpDown25.Value;
                }

                //Set angle and spread via velocity
                int nLowAngle = m_pEmitter.nDirection;
                int nHighAngle = m_pEmitter.nDirection + m_pEmitter.nSpreadAngle;
                int nRandomAngle = rand.Next(nLowAngle, nHighAngle);

                //Convert degrees to radians
                float fRandomRadians = (float)nRandomAngle * ((float)Math.PI / 180.0f);

                //Trig functions take in radians and will calculate a 0-1 x and y proportion
                float fXVelComponent = (float)Math.Cos(fRandomRadians);
                float fYVelComponent = (float)Math.Sin(fRandomRadians);

                //Set Velocity using direction and speed
                m_pEmitter.pParticleArray[i].fVelX = fXVelComponent * (float)nSpeed;
                m_pEmitter.pParticleArray[i].fVelY = fYVelComponent * (float)nSpeed;

                //Set up Origins 
                switch (m_pEmitter.nSpawnRange)
                {
                    case (int)eSpawnRange.ORIGIN:
                        {
                            //Default the starting position and origin to the same single point
                            m_pEmitter.pParticleArray[i].fPosX = m_pEmitter.fEmitterPosX;
                            m_pEmitter.pParticleArray[i].fPosY = m_pEmitter.fEmitterPosY;
                            m_pEmitter.pParticleArray[i].fOriginX = m_pEmitter.fEmitterPosX;
                            m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
                            break;
                        }
                    case (int)eSpawnRange.CIRCLE:
                        {
                            //Get the numeric input
                            float fRadius = (float)numericUpDown14.Value;

                            //Randomize the radius length
                            int RandRadius = rand.Next(0, (int)(fRadius * 1000.0f));

                            //Figure out the angle using the random x and y velocity components from previous code
                            int nRandomX = (int)m_pEmitter.fEmitterPosX + (int)(fXVelComponent * (float)(RandRadius / 1000));
                            int nRandomY = (int)m_pEmitter.fEmitterPosY + (int)(fYVelComponent * (float)(RandRadius / 1000));

                            //Set the start position and origin to the same place
                            m_pEmitter.pParticleArray[i].fPosX = nRandomX;
                            m_pEmitter.pParticleArray[i].fPosY = nRandomY;
                            m_pEmitter.pParticleArray[i].fOriginX = nRandomX;
                            m_pEmitter.pParticleArray[i].fOriginY = nRandomY;

                            break;
                        }
                    case (int)eSpawnRange.RECT:
                        {
                            //Get numbers from numeric input
                            int nWidth = (int)numericUpDown21.Value;
                            int nHeight = (int)numericUpDown22.Value;

                            //Set up int variables to randomize from
                            int nWidthMin = (int)m_pEmitter.fEmitterPosX - (nWidth / 2);
                            int nWidthMax = (int)m_pEmitter.fEmitterPosX + (nWidth / 2);
                            int nRandomX = rand.Next(nWidthMin, nWidthMax);

                            int nHeightMin = (int)m_pEmitter.fEmitterPosY - (nHeight / 2);
                            int nHeightMax = (int)m_pEmitter.fEmitterPosY + (nHeight / 2);
                            int nRandomY = rand.Next(nHeightMin, nHeightMax);

                            //Randomize a negative
                            int nRandomNegative = (rand.Next(0, 100) % 2);
                            if (nRandomNegative == 0)
                                nRandomNegative = -1;
                            else
                                nRandomNegative = 1;

                            //Set up particle positions and origin
                            m_pEmitter.pParticleArray[i].fPosX = nRandomX;
                            m_pEmitter.pParticleArray[i].fPosY = nRandomY;
                            m_pEmitter.pParticleArray[i].fOriginX = nRandomX;
                            m_pEmitter.pParticleArray[i].fOriginY = nRandomY;
                            break;
                        }
                    case (int)eSpawnRange.LINE:
                        {
                            //Get the numeric input
                            int nLineWidth = (int)numericUpDown23.Value;
                            int nMax = (int)m_pEmitter.fEmitterPosX + (nLineWidth / 2);
                            int nMin = (int)m_pEmitter.fEmitterPosX;
                            int nRandomX = rand.Next(nMin, nMax);
                            //Randomize a negative
                            int nRandomNegative = (rand.Next(0, 100) % 2);
                            if (nRandomNegative == 0)
                                nRandomNegative = -1;
                            else
                                nRandomNegative = 1;

                            //Set up particle positions and origin
                            m_pEmitter.pParticleArray[i].fPosX = nRandomX * nRandomNegative;
                            m_pEmitter.pParticleArray[i].fPosY = m_pEmitter.fEmitterPosY;
                            m_pEmitter.pParticleArray[i].fOriginX = nRandomX * nRandomNegative;
                            m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
                            break;
                        }
                }

                //Set up lifetimes
                bool bRandomLifetime = checkBox4.Checked;

                if (bRandomLifetime == false)
                {
                    //Use this if not random, just get the float and put it in all lifetime values
                    float fStaticLifetime = (float)numericUpDown24.Value;
                    m_pEmitter.pParticleArray[i].fMinLifetime = fStaticLifetime;
                    m_pEmitter.pParticleArray[i].fMaxLifetime = fStaticLifetime;
                    m_pEmitter.pParticleArray[i].fLifetime = fStaticLifetime;
                    m_pEmitter.pParticleArray[i].fAge = 0.0f;
                    m_pEmitter.pParticleArray[i].bAlive = true;
                }
                else if (bRandomLifetime == true)
                {
                    //Calculate a random number between the given min and max values
                    //have to use some float to int math to not truncate the float values
                    m_pEmitter.pParticleArray[i].fMinLifetime = (float)numericUpDown15.Value;
                    m_pEmitter.pParticleArray[i].fMaxLifetime = (float)numericUpDown6.Value;

                    int nLifeMin = (int)(m_pEmitter.pParticleArray[i].fMinLifetime * 1000.0f);
                    int nLifeMax = (int)(m_pEmitter.pParticleArray[i].fMaxLifetime * 1000.0f);
                    //Check so it doesnt crash the random function call
                    if (nLifeMin < nLifeMax)
                    {
                        int nLifeBuffer = rand.Next(nLifeMin, nLifeMax);
                        m_pEmitter.pParticleArray[i].fLifetime = (float)nLifeBuffer / 1000.0f;
                        //Give a random age within the lifetime so the particles come out one by one
                        m_pEmitter.pParticleArray[i].fAge = (float)rand.Next(nLifeMin, nLifeBuffer) / 1000.0f;
                    }
                }

                //Scale Info
                bool bInterpScale = checkBox5.Checked;
                bool bRandomScale = checkBox3.Checked;
                m_pEmitter.pParticleArray[i].fStartScale = (float)numericUpDown4.Value;
                m_pEmitter.pParticleArray[i].fEndScale = (float)numericUpDown5.Value;

                //Interpolate the Scale
                if (bInterpScale == true && bRandomScale == false)
                {
                    float fScaleChange = m_pEmitter.pParticleArray[i].fEndScale - m_pEmitter.pParticleArray[i].fStartScale;
                    m_pEmitter.pParticleArray[i].fScaleModifier = fScaleChange / m_pEmitter.pParticleArray[i].fLifetime;
                }

                //Randomize the Scale
                if (bInterpScale == false && bRandomScale == true)
                {
                    rand = new Random(Environment.TickCount + i);
                    //Int-ify the scales
                    int nMinScale = (int)(m_pEmitter.pParticleArray[i].fStartScale * 1000.0f);
                    int nMaxScale = (int)(m_pEmitter.pParticleArray[i].fEndScale * 1000.0f);

                    //Swap stuff so you dont break the random call
                    if (nMinScale > nMaxScale)
                    {
                        int nBufferScale = nMinScale;
                        nMinScale = nMaxScale;
                        nMaxScale = nBufferScale;
                    }

                    //Set the scale
                    m_pEmitter.pParticleArray[i].fScale = (float)(rand.Next(nMinScale, nMaxScale) / 1000.0f);
                    m_pEmitter.pParticleArray[i].fStartScale = m_pEmitter.pParticleArray[i].fScale;
                    m_pEmitter.pParticleArray[i].fEndScale = m_pEmitter.pParticleArray[i].fScale;

                    //Set the scale modifier to 0 so it doesnt change the scale over time
                    m_pEmitter.pParticleArray[i].fScaleModifier = 0.0f;
                }

                //Set up Rotation info
                bool bRandomRotate = checkBox2.Checked;

                //Calculate new Rotations
                m_pEmitter.pParticleArray[i].fStartRotation = (float)numericUpDown17.Value;
                m_pEmitter.pParticleArray[i].fEndRotation = (float)numericUpDown16.Value;

                //Rotation determined by a min and max
                if (bRandomRotate == true)
                {
                    int nRotMin = (int)(m_pEmitter.pParticleArray[i].fStartRotation * 1000.0f);
                    int nRotMax = (int)(m_pEmitter.pParticleArray[i].fEndRotation * 1000.0f);

                    if (nRotMin < nRotMax)
                    {
                        int nRotBuffer = rand.Next(nRotMin, nRotMax);
                        m_pEmitter.pParticleArray[i].fRotationModifier = (float)nRotBuffer / 1000.0f;

                        //Randomize Direction
                        int RotNegativeMod = rand.Next(0, 100);
                        if (RotNegativeMod % 2 == 1)
                            m_pEmitter.pParticleArray[i].fRotationModifier *= -1.0f;
                    }
                }
                //Just rotate by one static value
                else if (bRandomRotate == false)
                {
                    //Set up the rotation modifier based of the one rotation #
                    m_pEmitter.pParticleArray[i].fRotationModifier = (float)numericUpDown26.Value;
                    //Reset the rotation to start at 0
                    m_pEmitter.pParticleArray[i].fRotation = 0.0f;
                }

                //Alpha Checkbox variable booleans
                bool bInterpAlpha = checkBox8.Checked;
                bool bRandomAlpha = checkBox12.Checked;

                //Get the Alpha Values
                m_pEmitter.pParticleArray[i].nStartAlpha = (int)numericUpDown8.Value;
                m_pEmitter.pParticleArray[i].nEndAlpha = (int)numericUpDown9.Value;

                //Interpolate Alpha
                if (bInterpAlpha == true && bRandomAlpha == false)
                {
                    float fAlphaChange = (float)m_pEmitter.pParticleArray[i].nEndAlpha - m_pEmitter.pParticleArray[i].nStartAlpha;
                    m_pEmitter.pParticleArray[i].fAlphaModifier = fAlphaChange / m_pEmitter.pParticleArray[i].fLifetime;
                }
                //Randomize the Alpha
                else if (bInterpAlpha == false && bRandomAlpha == true)
                {
                    //Set Modifier to 0.0f because particles have static values now
                    m_pEmitter.pParticleArray[i].fAlphaModifier = 0.0f;

                    int nMinAlpha = m_pEmitter.pParticleArray[i].nStartAlpha;
                    int nMaxAlpha = m_pEmitter.pParticleArray[i].nEndAlpha;

                    //Swap mins and maxs to not break the random function call
                    if (nMinAlpha > nMaxAlpha)
                    {
                        int nBufferAlpha = nMinAlpha;
                        nMinAlpha = nMaxAlpha;
                        nMaxAlpha = nBufferAlpha;
                    }

                    //Reset all alphas to the same 
                    m_pEmitter.pParticleArray[i].fAlpha = rand.Next(nMinAlpha, nMaxAlpha);
                    m_pEmitter.pParticleArray[i].nStartAlpha = (int)m_pEmitter.pParticleArray[i].fAlpha;
                    m_pEmitter.pParticleArray[i].nEndAlpha = (int)m_pEmitter.pParticleArray[i].fAlpha;

                }

                //Color Checkbox variable booleans
                bool bInterpColor = checkBox7.Checked;
                bool bRandomColor = checkBox6.Checked;

                //Get Color Values
                m_pEmitter.pParticleArray[i].tStartColor = button7.BackColor;
                m_pEmitter.pParticleArray[i].tEndColor = button8.BackColor;

                //Interpolate Color
                if (bInterpColor == true && bRandomColor == false)
                {
                    int nRedChange = m_pEmitter.pParticleArray[i].tEndColor.R - m_pEmitter.pParticleArray[i].tStartColor.R;
                    int nGreenChange = m_pEmitter.pParticleArray[i].tEndColor.G - m_pEmitter.pParticleArray[i].tStartColor.G;
                    int nBlueChange = m_pEmitter.pParticleArray[i].tEndColor.B - m_pEmitter.pParticleArray[i].tStartColor.B;

                    m_pEmitter.pParticleArray[i].fRedModifier = (float)nRedChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fGreenModifier = (float)nGreenChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fBlueModifier = (float)nBlueChange / m_pEmitter.pParticleArray[i].fLifetime;
                }
                //Randomize Color
                else if (bInterpColor == false && bRandomColor == true)
                {

                    //Color Modifiers
                    m_pEmitter.pParticleArray[i].fRedModifier = 0.0f;
                    m_pEmitter.pParticleArray[i].fGreenModifier = 0.0f;
                    m_pEmitter.pParticleArray[i].fBlueModifier = 0.0f;

                    //Get the color starts and ends for random call
                    int nMinRed = m_pEmitter.pParticleArray[i].tStartColor.R;
                    int nMaxRed = m_pEmitter.pParticleArray[i].tEndColor.R;

                    int nMinGreen = m_pEmitter.pParticleArray[i].tStartColor.G;
                    int nMaxGreen = m_pEmitter.pParticleArray[i].tEndColor.G;

                    int nMinBlue = m_pEmitter.pParticleArray[i].tStartColor.B;
                    int nMaxBlue = m_pEmitter.pParticleArray[i].tEndColor.B;

                    //Swap mins and maxs to not break the random function call
                    if (nMinRed > nMaxRed)
                    {
                        int nBufferRed = nMinRed;
                        nMinRed = nMaxRed;
                        nMaxRed = nBufferRed;
                    }

                    if (nMinGreen > nMaxGreen)
                    {
                        int nBufferGreen = nMinGreen;
                        nMinGreen = nMaxGreen;
                        nMaxGreen = nBufferGreen;
                    }

                    if (nMinBlue > nMaxBlue)
                    {
                        int nBufferBlue = nMinBlue;
                        nMinBlue = nMaxBlue;
                        nMaxBlue = nBufferBlue;
                    }

                    int newRed = rand.Next(nMinRed, nMaxRed);
                    int newGreen = rand.Next(nMinGreen, nMaxGreen);
                    int newBlue = rand.Next(nMinBlue, nMaxBlue);

                    //Reset all color structures to the same
                    m_pEmitter.pParticleArray[i].tColor = Color.FromArgb(newRed, newGreen, newBlue);
                    m_pEmitter.pParticleArray[i].tStartColor = m_pEmitter.pParticleArray[i].tColor;
                    m_pEmitter.pParticleArray[i].tEndColor = m_pEmitter.pParticleArray[i].tColor;
                }

                //Get width and height
                m_pEmitter.pParticleArray[i].fWidth = (int)numericUpDown7.Value;
                m_pEmitter.pParticleArray[i].fHeight = (int)numericUpDown10.Value;

                //Age and Lifetime and Looping
                m_pEmitter.fEmitterLifetime = (float)numericUpDown3.Value;
                m_pEmitter.fAge = 0.0f;
                m_pEmitter.bLooping = checkBox1.Checked;
            }
        }

        //Emitter Particle Number via Scroll
        private void trackBar3_Scroll(object sender, EventArgs e)
        {
            numericUpDown1.Value = trackBar3.Value;
        }

        //Min Particle Lifetime ( Random ) 
        private void numericUpDown15_ValueChanged(object sender, EventArgs e)
        {
            //for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            //{
            //    m_pEmitter.pParticleArray[i].fMinLifetime = (float)numericUpDown15.Value;

            //    //Calculate the new random lifetime
            //    Random rand = new Random(Environment.TickCount + i);
            //    int nMin = (int)(m_pEmitter.pParticleArray[i].fMinLifetime * 1000.0f);
            //    int nMax = (int)(m_pEmitter.pParticleArray[i].fMaxLifetime * 1000.0f);
            //    if (nMin < nMax)
            //    {
            //        int nLifeBuffer = rand.Next(nMin, nMax);
            //        m_pEmitter.pParticleArray[i].fLifetime = (float)nLifeBuffer / 1000.0f;
            //    }

            //    //Calculate the new Alphas
            //    m_pEmitter.pParticleArray[i].nStartAlpha = (int)numericUpDown8.Value;
            //    m_pEmitter.pParticleArray[i].nEndAlpha = (int)numericUpDown9.Value;

            //    //Interpolate the Alpha
            //    float fAlphaChange = (float)m_pEmitter.pParticleArray[i].nEndAlpha - m_pEmitter.pParticleArray[i].nStartAlpha;
            //    m_pEmitter.pParticleArray[i].fAlphaModifier = fAlphaChange / m_pEmitter.pParticleArray[i].fLifetime;

            //    //Get Scale Values
            //    m_pEmitter.pParticleArray[i].fStartScale = (float)numericUpDown4.Value;
            //    m_pEmitter.pParticleArray[i].fEndScale = (float)numericUpDown5.Value;

            //    //Interpolate the Scale
            //    float fScaleChange = m_pEmitter.pParticleArray[i].fEndScale - m_pEmitter.pParticleArray[i].fStartScale;
            //    m_pEmitter.pParticleArray[i].fScaleModifier = fScaleChange / m_pEmitter.pParticleArray[i].fLifetime;

            //    //Get Color Values
            //    m_pEmitter.pParticleArray[i].tStartColor = button7.BackColor;
            //    m_pEmitter.pParticleArray[i].tEndColor = button8.BackColor;

            //    //Interpolate the Color
            //    int nRedChange = m_pEmitter.pParticleArray[i].tEndColor.R - m_pEmitter.pParticleArray[i].tStartColor.R;
            //    int nGreenChange = m_pEmitter.pParticleArray[i].tEndColor.G - m_pEmitter.pParticleArray[i].tStartColor.G;
            //    int nBlueChange = m_pEmitter.pParticleArray[i].tEndColor.B - m_pEmitter.pParticleArray[i].tStartColor.B;

            //    m_pEmitter.pParticleArray[i].fRedModifier = (float)nRedChange / m_pEmitter.pParticleArray[i].fLifetime;
            //    m_pEmitter.pParticleArray[i].fGreenModifier = (float)nGreenChange / m_pEmitter.pParticleArray[i].fLifetime;
            //    m_pEmitter.pParticleArray[i].fBlueModifier = (float)nBlueChange / m_pEmitter.pParticleArray[i].fLifetime;

            //}
        }

        //Max Particle Lifetime ( Random )
        private void numericUpDown6_ValueChanged(object sender, EventArgs e)
        {
            //for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            //{
            //    m_pEmitter.pParticleArray[i].fMaxLifetime = (float)numericUpDown6.Value;

            //    //Calculate the new random lifetime
            //    Random rand = new Random(Environment.TickCount + i);
            //    int nMin = (int)(m_pEmitter.pParticleArray[i].fMinLifetime * 1000.0f);
            //    int nMax = (int)(m_pEmitter.pParticleArray[i].fMaxLifetime * 1000.0f);
            //    if (nMin < nMax)
            //    {
            //        int nLifeBuffer = rand.Next(nMin, nMax);
            //        m_pEmitter.pParticleArray[i].fLifetime = (float)nLifeBuffer / 1000.0f;
            //    }

            //    //Calculate the new Alphas
            //    m_pEmitter.pParticleArray[i].nStartAlpha = (int)numericUpDown8.Value;
            //    m_pEmitter.pParticleArray[i].nEndAlpha = (int)numericUpDown9.Value;

            //    //Interpolate the Alpha
            //    float fAlphaChange = (float)m_pEmitter.pParticleArray[i].nEndAlpha - m_pEmitter.pParticleArray[i].nStartAlpha;
            //    m_pEmitter.pParticleArray[i].fAlphaModifier = fAlphaChange / m_pEmitter.pParticleArray[i].fLifetime;

            //    //Get Scale Values
            //    m_pEmitter.pParticleArray[i].fStartScale = (float)numericUpDown4.Value;
            //    m_pEmitter.pParticleArray[i].fEndScale = (float)numericUpDown5.Value;

            //    //Interpolate the Scale
            //    float fScaleChange = m_pEmitter.pParticleArray[i].fEndScale - m_pEmitter.pParticleArray[i].fStartScale;
            //    m_pEmitter.pParticleArray[i].fScaleModifier = fScaleChange / m_pEmitter.pParticleArray[i].fLifetime;

            //    //Get Color Values
            //    m_pEmitter.pParticleArray[i].tStartColor = button7.BackColor;
            //    m_pEmitter.pParticleArray[i].tEndColor = button8.BackColor;

            //    //Interpolate the Color
            //    int nRedChange = m_pEmitter.pParticleArray[i].tEndColor.R - m_pEmitter.pParticleArray[i].tStartColor.R;
            //    int nGreenChange = m_pEmitter.pParticleArray[i].tEndColor.G - m_pEmitter.pParticleArray[i].tStartColor.G;
            //    int nBlueChange = m_pEmitter.pParticleArray[i].tEndColor.B - m_pEmitter.pParticleArray[i].tStartColor.B;

            //    m_pEmitter.pParticleArray[i].fRedModifier = (float)nRedChange / m_pEmitter.pParticleArray[i].fLifetime;
            //    m_pEmitter.pParticleArray[i].fGreenModifier = (float)nGreenChange / m_pEmitter.pParticleArray[i].fLifetime;
            //    m_pEmitter.pParticleArray[i].fBlueModifier = (float)nBlueChange / m_pEmitter.pParticleArray[i].fLifetime;

            //}
        }

        //Start Alpha
        private void numericUpDown8_ValueChanged(object sender, EventArgs e)
        {
            //for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            //{
            //    m_pEmitter.pParticleArray[i].nStartAlpha = (int)numericUpDown8.Value;

            //    //Interpolate the Alpha
            //    float fAlphaChange = (float)m_pEmitter.pParticleArray[i].nEndAlpha - m_pEmitter.pParticleArray[i].nStartAlpha;
            //    m_pEmitter.pParticleArray[i].fAlphaModifier = fAlphaChange / m_pEmitter.pParticleArray[i].fLifetime;
            //}
        }

        //End Alpha
        private void numericUpDown9_ValueChanged(object sender, EventArgs e)
        {
            //for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            //{
            //    m_pEmitter.pParticleArray[i].nEndAlpha = (int)numericUpDown9.Value;

            //    //Interpolate the Alpha
            //    float fAlphaChange = (float)m_pEmitter.pParticleArray[i].nEndAlpha - m_pEmitter.pParticleArray[i].nStartAlpha;
            //    m_pEmitter.pParticleArray[i].fAlphaModifier = fAlphaChange / m_pEmitter.pParticleArray[i].fLifetime;
            //}
        }

        //Min Scale
        private void numericUpDown4_ValueChanged(object sender, EventArgs e)
        {

        }

        //Max Scale
        private void numericUpDown5_ValueChanged(object sender, EventArgs e)
        {

        }

        //Min Rotation
        private void numericUpDown17_ValueChanged(object sender, EventArgs e)
        {

        }

        //Max Rotation
        private void numericUpDown16_ValueChanged(object sender, EventArgs e)
        {

        }

        //Start Color
        private void button7_Click(object sender, EventArgs e)
        {
            ColorDialog cdlg = new ColorDialog();

            if (DialogResult.OK == cdlg.ShowDialog())
            {
                for (int i = 0; i < m_pEmitter.nNumParticles; i++)
                {
                    m_pEmitter.pParticleArray[i].tStartColor = cdlg.Color;

                    //Interpolate the Color
                    int nRedChange = m_pEmitter.pParticleArray[i].tEndColor.R - m_pEmitter.pParticleArray[i].tStartColor.R;
                    int nGreenChange = m_pEmitter.pParticleArray[i].tEndColor.G - m_pEmitter.pParticleArray[i].tStartColor.G;
                    int nBlueChange = m_pEmitter.pParticleArray[i].tEndColor.B - m_pEmitter.pParticleArray[i].tStartColor.B;

                    m_pEmitter.pParticleArray[i].fRedModifier = (float)nRedChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fGreenModifier = (float)nGreenChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fBlueModifier = (float)nBlueChange / m_pEmitter.pParticleArray[i].fLifetime;

                }

                button7.BackColor = cdlg.Color;
            }

        }

        //End Color
        private void button8_Click(object sender, EventArgs e)
        {
            ColorDialog cdlg = new ColorDialog();

            if (DialogResult.OK == cdlg.ShowDialog())
            {
                for (int i = 0; i < m_pEmitter.nNumParticles; i++)
                {
                    m_pEmitter.pParticleArray[i].tEndColor = cdlg.Color;

                    //Interpolate the Color
                    int nRedChange = m_pEmitter.pParticleArray[i].tEndColor.R - m_pEmitter.pParticleArray[i].tStartColor.R;
                    int nGreenChange = m_pEmitter.pParticleArray[i].tEndColor.G - m_pEmitter.pParticleArray[i].tStartColor.G;
                    int nBlueChange = m_pEmitter.pParticleArray[i].tEndColor.B - m_pEmitter.pParticleArray[i].tStartColor.B;

                    m_pEmitter.pParticleArray[i].fRedModifier = (float)nRedChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fGreenModifier = (float)nGreenChange / m_pEmitter.pParticleArray[i].fLifetime;
                    m_pEmitter.pParticleArray[i].fBlueModifier = (float)nBlueChange / m_pEmitter.pParticleArray[i].fLifetime;
                }

                button8.BackColor = cdlg.Color;
            }
        }

        //Clicking and changing particle origin
        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            m_pEmitter.fEmitterPosX = e.X;
            m_pEmitter.fEmitterPosY = e.Y;

            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                m_pEmitter.pParticleArray[i].fOriginX = m_pEmitter.fEmitterPosX;
                m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
            }
        }

        //Clicking and changing particle origin
        private void panel1_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                m_pEmitter.fEmitterPosX = e.X;
                m_pEmitter.fEmitterPosY = e.Y;

                for (int i = 0; i < m_pEmitter.nNumParticles; i++)
                {
                    m_pEmitter.pParticleArray[i].fOriginX = m_pEmitter.fEmitterPosX;
                    m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
                }
            }
        }

        //Texture Width
        private void numericUpDown7_ValueChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                m_pEmitter.pParticleArray[i].fWidth = (int)numericUpDown7.Value;
            }
        }

        //Texture Height
        private void numericUpDown10_ValueChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                m_pEmitter.pParticleArray[i].fHeight = (int)numericUpDown10.Value;
            }
        }

        //Min Speed
        private void numericUpDown11_ValueChanged(object sender, EventArgs e)
        {
            m_pEmitter.fMinSpeed = (float)numericUpDown11.Value;
        }

        //Max Speed
        private void numericUpDown13_ValueChanged(object sender, EventArgs e)
        {
            m_pEmitter.fMaxSpeed = (float)numericUpDown13.Value;
        }

        //Spread Angle Numeric
        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            m_pEmitter.nSpreadAngle = (int)numericUpDown2.Value;
            trackBar2.Value = (int)numericUpDown2.Value;
        }

        //Spread Angle ScrollBar
        private void trackBar2_Scroll(object sender, EventArgs e)
        {
            numericUpDown2.Value = trackBar2.Value;
        }

        //Direction Numeric
        private void numericUpDown12_ValueChanged(object sender, EventArgs e)
        {
            m_pEmitter.nDirection = (int)numericUpDown12.Value;
            trackBar4.Value = (int)numericUpDown12.Value;
        }

        //Direction ScrollBar
        private void trackBar4_Scroll(object sender, EventArgs e)
        {
            numericUpDown12.Value = trackBar4.Value;
        }

        //Center Emitter
        private void button6_Click(object sender, EventArgs e)
        {
            m_pEmitter.fEmitterPosX = (float)panel1.Width / 2.0f;
            m_pEmitter.fEmitterPosY = (float)panel1.Height / 2.0f;

            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                m_pEmitter.pParticleArray[i].fOriginX = m_pEmitter.fEmitterPosX;
                m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
            }
        }

        //System Name
        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            m_pEmitter.szSystemName = textBox1.Text;
        }

        //Emitter Looping Checkbox
        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            m_pEmitter.bLooping = checkBox1.Checked;

            //Reset the particle age and lifetime
            m_pEmitter.fEmitterLifetime = (float)numericUpDown3.Value;
        }

        //Play effect button
        private void button2_Click(object sender, EventArgs e)
        {
            //Reset the emitter age so it can reset the particle effect
            m_pEmitter.fAge = 0.0f;
            m_pEmitter.fEmitterLifetime = (float)numericUpDown3.Value;

            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                m_pEmitter.pParticleArray[i].fAge = 0.0f;
                m_pEmitter.pParticleArray[i].bAlive = true;
            }
        }

        //Spawn On Origin radio button pressed
        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            m_pEmitter.nSpawnRange = (int)eSpawnRange.ORIGIN;
        }

        //Circle radio button pressed
        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            m_pEmitter.nSpawnRange = (int)eSpawnRange.CIRCLE;
            groupBox24.Visible = !groupBox24.Visible;
        }

        //Rectangle radio button pressed
        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            m_pEmitter.nSpawnRange = (int)eSpawnRange.RECT;
            groupBox25.Visible = !groupBox25.Visible;
        }

        //Line Radio button pressed
        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            m_pEmitter.nSpawnRange = (int)eSpawnRange.LINE;
            groupBox26.Visible = !groupBox26.Visible;
        }

        //Lifetime Toggle Checkbox
        private void checkBox4_CheckedChanged(object sender, EventArgs e)
        {
            //Turn min and max off
            groupBox8.Visible = checkBox4.Checked;
            f.Visible = checkBox4.Checked;
            //Turn Single Lifetime on
            groupBox27.Visible = !checkBox4.Checked;

            //Manually place this pos(piece of shit)
            groupBox27.Location = new Point(6, 19);
        }

        //Particle Speed Toggle Checkbox
        private void checkBox11_CheckedChanged(object sender, EventArgs e)
        {
            //Turn min and max off
            groupBox31.Visible = checkBox11.Checked;
            groupBox32.Visible = checkBox11.Checked;
            //Turn single Speed on
            groupBox33.Visible = !checkBox11.Checked;
            //Have to manually do this for some reason, its being weird
            groupBox33.Location = new Point(6, 19);
        }

        //Rotation Toggle Checkbox
        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            //Turn min and max off
            groupBox15.Visible = checkBox2.Checked;
            groupBox16.Visible = checkBox2.Checked;

            //Turn single Rotation on
            groupBox34.Visible = !checkBox2.Checked;
            //Have to manually do this for some reason, i cant fit the groupbox
            groupBox34.Location = new Point(7, 19);
        }

        //Source blend combobox
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                if (comboBox1.SelectedItem.ToString() == "Zero")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.Zero;
                }
                if (comboBox1.SelectedItem.ToString() == "One")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.One;
                }
                if (comboBox1.SelectedItem.ToString() == "SourceColor")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.SourceColor;
                }
                if (comboBox1.SelectedItem.ToString() == "InvSourceColor")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.InvSourceColor;
                }
                if (comboBox1.SelectedItem.ToString() == "SourceAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.SourceAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "InvSourceAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.InvSourceAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "DestinationAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.DestinationAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "InvDestinationAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.InvDestinationAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "InvDestinationColor")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.InvDestinationColor;
                }
                if (comboBox1.SelectedItem.ToString() == "SourceAlphaSat")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.SourceAlphaSat;
                }
                if (comboBox1.SelectedItem.ToString() == "BothSourceAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.BothSourceAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "BothInvSourceAlpha")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.BothInvSourceAlpha;
                }
                if (comboBox1.SelectedItem.ToString() == "BlendFactor")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.BlendFactor;
                }
                if (comboBox1.SelectedItem.ToString() == "InvBlendFactor")
                {
                    m_pEmitter.SourceBlend = Microsoft.DirectX.Direct3D.Blend.InvBlendFactor;
                }
            }
        }

        //Destination Blend
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < m_pEmitter.nNumParticles; i++)
            {
                if (comboBox2.SelectedItem.ToString() == "Zero")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.Zero;
                }
                if (comboBox2.SelectedItem.ToString() == "One")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.One;
                }
                if (comboBox2.SelectedItem.ToString() == "SourceColor")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.SourceColor;
                }
                if (comboBox2.SelectedItem.ToString() == "InvSourceColor")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.InvSourceColor;
                }
                if (comboBox2.SelectedItem.ToString() == "SourceAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.SourceAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "InvSourceAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.InvSourceAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "DestinationAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.DestinationAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "InvDestinationAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.InvDestinationAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "InvDestinationColor")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.InvDestinationColor;
                }
                if (comboBox2.SelectedItem.ToString() == "SourceAlphaSat")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.SourceAlphaSat;
                }
                if (comboBox2.SelectedItem.ToString() == "BothSourceAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.BothSourceAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "BothInvSourceAlpha")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.BothInvSourceAlpha;
                }
                if (comboBox2.SelectedItem.ToString() == "BlendFactor")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.BlendFactor;
                }
                if (comboBox2.SelectedItem.ToString() == "InvBlendFactor")
                {
                    m_pEmitter.DestBlend = Microsoft.DirectX.Direct3D.Blend.InvBlendFactor;
                }
            }
        }

        //Helper Function for outputing text
        private static void AddText(FileStream fs, string value)
        {
            byte[] info = new UTF8Encoding(true).GetBytes(value);
            fs.Write(info, 0, info.Length);
        }

        //Save as text
        private void textToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Delete the file if it exists.
            if (File.Exists(SavePath))
            {
                File.Delete(SavePath);
            }

            //Create the file.
            using (FileStream fs = File.Create(SavePath))
            {
                //Particle Effect Name
                AddText(fs, "ParticleEffectName=");
                AddText(fs, textBox1.Text);
                AddText(fs, "\n");

                //Emitter Lifetime
                AddText(fs, "EmitterLifetime=");
                AddText(fs, numericUpDown3.Value.ToString());
                AddText(fs, "\n");

                //Emitter Looping Boolean
                AddText(fs, "EmitterLooping=");
                AddText(fs, checkBox1.Checked.ToString());
                AddText(fs, "\n");

                //Emitter Spawn Range
                //Single Origin
                AddText(fs, "SingleOrigin=");
                AddText(fs, radioButton1.Checked.ToString());
                AddText(fs, "\n");

                //Circle
                AddText(fs, "Circle=");
                AddText(fs, radioButton4.Checked.ToString());
                AddText(fs, "\n");

                AddText(fs, "CircleRadius=");
                AddText(fs, numericUpDown14.Value.ToString());
                AddText(fs, "\n");

                //Rectangle
                AddText(fs, "Rectangle=");
                AddText(fs, radioButton2.Checked.ToString());
                AddText(fs, "\n");

                AddText(fs, "RectangleWidth=");
                AddText(fs, numericUpDown21.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "RectangleHeight=");
                AddText(fs, numericUpDown22.Value.ToString());
                AddText(fs, "\n");

                //Line
                AddText(fs, "Line=");
                AddText(fs, radioButton3.Checked.ToString());
                AddText(fs, "\n");

                AddText(fs, "LineLength=");
                AddText(fs, numericUpDown23.Value.ToString());
                AddText(fs, "\n");

                //Texture Size
                AddText(fs, "TextureWidth=");
                AddText(fs, numericUpDown7.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "TextureHeight=");
                AddText(fs, numericUpDown10.Value.ToString());
                AddText(fs, "\n");

                //Spread Angle
                AddText(fs, "SpreadAngle=");
                AddText(fs, numericUpDown2.Value.ToString());
                AddText(fs, "\n");

                //Direction numericUpDown12
                AddText(fs, "Direction=");
                AddText(fs, numericUpDown12.Value.ToString());
                AddText(fs, "\n");

                //Number of Particles
                AddText(fs, "NumParticles=");
                AddText(fs, numericUpDown1.Value.ToString());
                AddText(fs, "\n");

                //Source Blend
                AddText(fs, "SourceBlend=");
                AddText(fs, comboBox1.SelectedItem.ToString());
                AddText(fs, "\n");

                //Destination Blend
                AddText(fs, "DestinationBlend=");
                AddText(fs, comboBox2.SelectedItem.ToString());
                AddText(fs, "\n");

                //Now Particle Stuff
                //Particle Texture name
                AddText(fs, "TextureName=");
                AddText(fs, textBox2.Text);
                AddText(fs, "\n");

                //Lifetime
                AddText(fs, "MinLifetime=");
                AddText(fs, numericUpDown15.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "MaxLifetime=");
                AddText(fs, numericUpDown6.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "Time=");
                AddText(fs, numericUpDown24.Value.ToString());
                AddText(fs, "\n");

                //Lifetime checkbox
                AddText(fs, "LifeTimeRandom=");
                AddText(fs, checkBox4.Checked.ToString());
                AddText(fs, "\n");

                //Colors
                AddText(fs, "StartColor=");
                AddText(fs, button7.BackColor.R.ToString());
                AddText(fs, " ");
                AddText(fs, button7.BackColor.G.ToString());
                AddText(fs, " ");
                AddText(fs, button7.BackColor.B.ToString());
                AddText(fs, "\n");

                AddText(fs, "EndColor=");
                AddText(fs, button8.BackColor.R.ToString());
                AddText(fs, " ");
                AddText(fs, button8.BackColor.G.ToString());
                AddText(fs, " ");
                AddText(fs, button8.BackColor.B.ToString());
                AddText(fs, "\n");

                //Interp Color Checkbox
                AddText(fs, "InterpColor=");
                AddText(fs, checkBox7.Checked.ToString());
                AddText(fs, "\n");

                //Randomize Color checkbox 
                AddText(fs, "RandomizeColor=");
                AddText(fs, checkBox6.Checked.ToString());
                AddText(fs, "\n");

                //Alpha
                AddText(fs, "AlphaStart=");
                AddText(fs, numericUpDown8.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "AlphaEnd=");
                AddText(fs, numericUpDown9.Value.ToString());
                AddText(fs, "\n");

                //Interp Alpha Checkbox
                AddText(fs, "InterpAlpha=");
                AddText(fs, checkBox8.Checked.ToString());
                AddText(fs, "\n");

                //Randomize Alpha Checkbox
                AddText(fs, "RandomizeAlpha=");
                AddText(fs, checkBox12.Checked.ToString());
                AddText(fs, "\n");

                //Scale 
                AddText(fs, "MinScale=");
                AddText(fs, numericUpDown4.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "MaxScale=");
                AddText(fs, numericUpDown5.Value.ToString());
                AddText(fs, "\n");

                //Interp Scale Checkbox
                AddText(fs, "InterpScale=");
                AddText(fs, checkBox5.Checked.ToString());
                AddText(fs, "\n");

                //Randomize Scale Checkbox
                AddText(fs, "RandomizeScale=");
                AddText(fs, checkBox3.Checked.ToString());
                AddText(fs, "\n");

                //Rotation
                AddText(fs, "MinRotation=");
                AddText(fs, numericUpDown17.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "MaxRotation=");
                AddText(fs, numericUpDown16.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "Rotation=");
                AddText(fs, numericUpDown26.Value.ToString());
                AddText(fs, "\n");

                //Rotation Randomize Checkbox
                AddText(fs, "RandomizeScale=");
                AddText(fs, checkBox2.Checked.ToString());
                AddText(fs, "\n");

                //Particle Speed
                AddText(fs, "MinSpeed=");
                AddText(fs, numericUpDown11.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "MaxSpeed=");
                AddText(fs, numericUpDown13.Value.ToString());
                AddText(fs, "\n");

                AddText(fs, "Speed=");
                AddText(fs, numericUpDown25.Value.ToString());
                AddText(fs, "\n");

                //Randomize Speed Checkbox
                AddText(fs, "RandomizeSpeed=");
                AddText(fs, checkBox11.Checked.ToString());
                AddText(fs, "\n");
            }
        }

        //File Save ( Binary )
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.InitialDirectory = m_szWorkSpace;
            dlg.Filter = "Text Files|*.txt";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                using (BinaryWriter binWriter = new BinaryWriter(File.Open(dlg.FileName, FileMode.Create)))
                {
                    //Particle Effect Name
                    binWriter.Write(textBox1.Text);

                    //Emitter Lifetime
                    binWriter.Write(numericUpDown3.Value);

                    //Emitter Looping Boolean
                    binWriter.Write(checkBox1.Checked);

                    //Emitter Spawn Range
                    //Single Origin
                    binWriter.Write(radioButton1.Checked);

                    //Circle---------
                    //Radio button
                    binWriter.Write(radioButton4.Checked);

                    //Radius
                    binWriter.Write(numericUpDown14.Value);

                    //Rectangle-----------
                    //Radio button
                    binWriter.Write(radioButton2.Checked);

                    //Width
                    binWriter.Write(numericUpDown21.Value);

                    //Height
                    binWriter.Write(numericUpDown22.Value);

                    //Line
                    binWriter.Write(radioButton3.Checked);

                    //Length
                    binWriter.Write(numericUpDown23.Value);

                    //Texture Size---------
                    //Width
                    binWriter.Write(numericUpDown7.Value);

                    //Height
                    binWriter.Write(numericUpDown10.Value);

                    //Spread Angle------
                    binWriter.Write(numericUpDown2.Value);

                    //Direction numericUpDown12
                    binWriter.Write(numericUpDown12.Value);

                    //Number of Particles
                    binWriter.Write(numericUpDown1.Value);

                    //Source Blend
                    binWriter.Write(comboBox1.SelectedItem.ToString());

                    //Destination Blend
                    binWriter.Write(comboBox2.SelectedItem.ToString());

                    //Now Particle Stuff-----
                    //Particle Texture name
                    binWriter.Write(textBox2.Text);

                    //Particle Texture path
                    binWriter.Write(m_pEmitter.szParticleTexturePath);

                    //Lifetime----
                    //MinLife
                    binWriter.Write(numericUpDown15.Value);

                    //MaxLife
                    binWriter.Write(numericUpDown6.Value);

                    //Single Time
                    binWriter.Write(numericUpDown24.Value);

                    //Lifetime checkbox
                    binWriter.Write(checkBox4.Checked);

                    //Colors----
                    //StartColor
                    binWriter.Write(button7.BackColor.R);
                    binWriter.Write(button7.BackColor.G);
                    binWriter.Write(button7.BackColor.B);

                    //EndColor
                    binWriter.Write(button8.BackColor.R);
                    binWriter.Write(button8.BackColor.G);
                    binWriter.Write(button8.BackColor.B);

                    //Interp Color Checkbox
                    binWriter.Write(checkBox7.Checked);

                    //Randomize Color checkbox 
                    binWriter.Write(checkBox6.Checked);

                    //Alpha----
                    //AlphaStart
                    binWriter.Write(numericUpDown8.Value);

                    //AlphaEnd
                    binWriter.Write(numericUpDown9.Value);

                    //Interp Alpha Checkbox
                    binWriter.Write(checkBox8.Checked);

                    //Randomize Alpha Checkbox
                    binWriter.Write(checkBox12.Checked);

                    //Scale----
                    //MinScale
                    binWriter.Write(numericUpDown4.Value);

                    //MaxScale
                    binWriter.Write(numericUpDown5.Value);

                    //Interp Scale Checkbox
                    binWriter.Write(checkBox5.Checked);

                    //Randomize Scale Checkbox
                    binWriter.Write(checkBox3.Checked);

                    //Rotation----
                    //MinRotation
                    binWriter.Write(numericUpDown17.Value);

                    //MaxRotation
                    binWriter.Write(numericUpDown16.Value);

                    //Single Rotation
                    binWriter.Write(numericUpDown26.Value);

                    //Rotation Randomize Checkbox
                    binWriter.Write(checkBox2.Checked);

                    //Particle Speed----
                    //MinSpeed
                    binWriter.Write(numericUpDown11.Value);

                    //MaxSpeed
                    binWriter.Write(numericUpDown13.Value);

                    //SingleSpeed
                    binWriter.Write(numericUpDown25.Value);

                    //Randomize Speed Checkbox
                    binWriter.Write(checkBox11.Checked);
                }
            }
        }

        //File Load ( Binary )
        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = m_szWorkSpace;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                if (File.Exists(dlg.FileName))
                {
                    BinaryReader binReader = new BinaryReader(File.Open(dlg.FileName, FileMode.Open));

                    //Particle Effect Name
                    textBox1.Text = binReader.ReadString();

                    //Emitter Lifetime
                    numericUpDown3.Value = binReader.ReadDecimal();

                    //Emitter Looping Boolean
                    checkBox1.Checked = binReader.ReadBoolean();

                    //Emitter Spawn Range
                    //Single Origin
                    radioButton1.Checked = binReader.ReadBoolean();

                    //Circle---------
                    //Radio button
                    radioButton4.Checked = binReader.ReadBoolean();

                    //Radius
                    numericUpDown14.Value = binReader.ReadDecimal();

                    //Rectangle-----------
                    //Radio button
                    radioButton2.Checked = binReader.ReadBoolean();

                    //Width
                    numericUpDown21.Value = binReader.ReadDecimal();

                    //Height
                    numericUpDown22.Value = binReader.ReadDecimal();

                    //Line
                    radioButton3.Checked = binReader.ReadBoolean();

                    //Length
                    numericUpDown23.Value = binReader.ReadDecimal();

                    //Texture Size---------
                    //Width
                    numericUpDown7.Value = binReader.ReadDecimal();

                    //Height
                    numericUpDown10.Value = binReader.ReadDecimal();

                    //Spread Angle------
                    numericUpDown2.Value = binReader.ReadDecimal();

                    //Direction numericUpDown12
                    numericUpDown12.Value = binReader.ReadDecimal();

                    //Number of Particles
                    numericUpDown1.Value = binReader.ReadDecimal();

                    //Source Blend
                    comboBox1.SelectedItem = binReader.ReadString();

                    //Destination Blend
                    comboBox2.SelectedItem = binReader.ReadString();

                    //Now Particle Stuff-----
                    //Particle Texture name
                    textBox2.Text = binReader.ReadString();

                    //Particle Texture Path
                    m_pEmitter.szParticleTexturePath = binReader.ReadString();
                    m_pEmitter.SetParticleTexture(m_TM.LoadTexture(m_pEmitter.szParticleTexturePath, Color.Black.ToArgb()));

                    //Lifetime----
                    //MinLife
                    numericUpDown15.Value = binReader.ReadDecimal();

                    //MaxLife
                    numericUpDown6.Value = binReader.ReadDecimal();

                    //Single Time
                    numericUpDown24.Value = binReader.ReadDecimal();

                    //Lifetime checkbox
                    checkBox4.Checked = binReader.ReadBoolean();

                    //Colors----
                    //StartColor
                    //Have to do a lame conversion
                    byte r = binReader.ReadByte();
                    byte g = binReader.ReadByte();
                    byte b = binReader.ReadByte();
                    button7.BackColor = Color.FromArgb(r, g, b);

                    //EndColor
                    byte r1 = binReader.ReadByte();
                    byte g1 = binReader.ReadByte();
                    byte b1 = binReader.ReadByte();
                    button8.BackColor = Color.FromArgb(r1, g1, b1);

                    //Interp Color Checkbox
                    checkBox7.Checked = binReader.ReadBoolean();

                    //Randomize Color checkbox 
                    checkBox6.Checked = binReader.ReadBoolean();

                    //Alpha----
                    //AlphaStart
                    numericUpDown8.Value = binReader.ReadDecimal();

                    //AlphaEnd
                    numericUpDown9.Value = binReader.ReadDecimal();

                    //Interp Alpha Checkbox
                    checkBox8.Checked = binReader.ReadBoolean();

                    //Randomize Alpha Checkbox
                    checkBox12.Checked = binReader.ReadBoolean();

                    //Scale----
                    //MinScale
                    numericUpDown4.Value = binReader.ReadDecimal();

                    //MaxScale
                    numericUpDown5.Value = binReader.ReadDecimal();

                    //Interp Scale Checkbox
                    checkBox5.Checked = binReader.ReadBoolean();

                    //Randomize Scale Checkbox
                    checkBox3.Checked = binReader.ReadBoolean();

                    //Rotation----
                    //MinRotation
                    numericUpDown17.Value = binReader.ReadDecimal();

                    //MaxRotation
                    numericUpDown16.Value = binReader.ReadDecimal();

                    //Single Rotation
                    numericUpDown26.Value = binReader.ReadDecimal();

                    //Rotation Randomize Checkbox
                    checkBox2.Checked = binReader.ReadBoolean();

                    //Particle Speed----
                    //MinSpeed
                    numericUpDown11.Value = binReader.ReadDecimal();

                    //MaxSpeed
                    numericUpDown13.Value = binReader.ReadDecimal();

                    //SingleSpeed
                    numericUpDown25.Value = binReader.ReadDecimal();

                    //Randomize Speed Checkbox
                    checkBox11.Checked = binReader.ReadBoolean();

                    //Awful work around to get the effect to reload
                    numericUpDown1.Value += 1;
                    numericUpDown1.Value -= 1;
                }
            }
        }

        //Reset Particle Function
        //public void ResetEffect()
        //{
        //    this.m_pEmitter.nNumParticles = (int)numericUpDown1.Value;
        //    trackBar3.Value = (int)numericUpDown1.Value;

        //    //Create the new particle array
        //    this.m_pEmitter.pParticleArray = new CParticle[m_pEmitter.nNumParticles];

        //    for (int i = 0; i < m_pEmitter.nNumParticles; i++)
        //    {
        //        //Create each new instance of a particle
        //        m_pEmitter.pParticleArray[i] = new CParticle(m_pEmitter.fEmitterPosX, m_pEmitter.fEmitterPosY);

        //        //Set their textures
        //        m_pEmitter.pParticleArray[i].nTexture = m_pEmitter.nParticleTexture;

        //        //set up the random # generator
        //        Random rand = new Random(Environment.TickCount + i);

        //        //Get speed values
        //        m_pEmitter.fMinSpeed = (float)numericUpDown11.Value;
        //        m_pEmitter.fMaxSpeed = (float)numericUpDown13.Value;

        //        //Set up speed
        //        bool bRandomSpeed = checkBox11.Checked;
        //        int nSpeed = 0;

        //        //Randomize 2 elements
        //        if (bRandomSpeed == true)
        //        {
        //            //Randomize between the two min and max speeds
        //            nSpeed = rand.Next((int)m_pEmitter.fMinSpeed, (int)m_pEmitter.fMaxSpeed);
        //        }
        //        //Set static speed
        //        else if (bRandomSpeed == false)
        //        {
        //            //Single speed value
        //            nSpeed = (int)numericUpDown25.Value;
        //        }

        //        //Set angle and spread via velocity
        //        int nLowAngle = m_pEmitter.nDirection;
        //        int nHighAngle = m_pEmitter.nDirection + m_pEmitter.nSpreadAngle;
        //        int nRandomAngle = rand.Next(nLowAngle, nHighAngle);

        //        //Convert degrees to radians
        //        float fRandomRadians = (float)nRandomAngle * ((float)Math.PI / 180.0f);

        //        //Trig functions take in radians and will calculate a 0-1 x and y proportion
        //        float fXVelComponent = (float)Math.Cos(fRandomRadians);
        //        float fYVelComponent = (float)Math.Sin(fRandomRadians);

        //        //Set Velocity using direction and speed
        //        m_pEmitter.pParticleArray[i].fVelX = fXVelComponent * (float)nSpeed;
        //        m_pEmitter.pParticleArray[i].fVelY = fYVelComponent * (float)nSpeed;

        //        //Set up Origins 
        //        switch (m_pEmitter.nSpawnRange)
        //        {
        //            case (int)eSpawnRange.ORIGIN:
        //                {
        //                    //Default the starting position and origin to the same single point
        //                    m_pEmitter.pParticleArray[i].fPosX = m_pEmitter.fEmitterPosX;
        //                    m_pEmitter.pParticleArray[i].fPosY = m_pEmitter.fEmitterPosY;
        //                    m_pEmitter.pParticleArray[i].fOriginX = m_pEmitter.fEmitterPosX;
        //                    m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
        //                    break;
        //                }
        //            case (int)eSpawnRange.CIRCLE:
        //                {
        //                    //Get the numeric input
        //                    float fRadius = (float)numericUpDown14.Value;

        //                    //Randomize the radius length
        //                    int RandRadius = rand.Next(0, (int)(fRadius * 1000.0f));

        //                    //Figure out the angle using the random x and y velocity components from previous code
        //                    int nRandomX = (int)m_pEmitter.fEmitterPosX + (int)(fXVelComponent * (float)(RandRadius / 1000));
        //                    int nRandomY = (int)m_pEmitter.fEmitterPosY + (int)(fYVelComponent * (float)(RandRadius / 1000));

        //                    //Set the start position and origin to the same place
        //                    m_pEmitter.pParticleArray[i].fPosX = nRandomX;
        //                    m_pEmitter.pParticleArray[i].fPosY = nRandomY;
        //                    m_pEmitter.pParticleArray[i].fOriginX = nRandomX;
        //                    m_pEmitter.pParticleArray[i].fOriginY = nRandomY;

        //                    break;
        //                }
        //            case (int)eSpawnRange.RECT:
        //                {
        //                    //Get numbers from numeric input
        //                    int nWidth = (int)numericUpDown21.Value;
        //                    int nHeight = (int)numericUpDown22.Value;

        //                    //Set up int variables to randomize from
        //                    int nWidthMin = (int)m_pEmitter.fEmitterPosX - (nWidth / 2);
        //                    int nWidthMax = (int)m_pEmitter.fEmitterPosX + (nWidth / 2);
        //                    int nRandomX = rand.Next(nWidthMin, nWidthMax);

        //                    int nHeightMin = (int)m_pEmitter.fEmitterPosY - (nHeight / 2);
        //                    int nHeightMax = (int)m_pEmitter.fEmitterPosY + (nHeight / 2);
        //                    int nRandomY = rand.Next(nHeightMin, nHeightMax);

        //                    //Randomize a negative
        //                    int nRandomNegative = (rand.Next(0, 100) % 2);
        //                    if (nRandomNegative == 0)
        //                        nRandomNegative = -1;
        //                    else
        //                        nRandomNegative = 1;

        //                    //Set up particle positions and origin
        //                    m_pEmitter.pParticleArray[i].fPosX = nRandomX;
        //                    m_pEmitter.pParticleArray[i].fPosY = nRandomY;
        //                    m_pEmitter.pParticleArray[i].fOriginX = nRandomX;
        //                    m_pEmitter.pParticleArray[i].fOriginY = nRandomY;
        //                    break;
        //                }
        //            case (int)eSpawnRange.LINE:
        //                {
        //                    //Get the numeric input
        //                    int nLineWidth = (int)numericUpDown23.Value;
        //                    int nMax = (int)m_pEmitter.fEmitterPosX + (nLineWidth / 2);
        //                    int nMin = (int)m_pEmitter.fEmitterPosX;
        //                    int nRandomX = rand.Next(nMin, nMax);
        //                    //Randomize a negative
        //                    int nRandomNegative = (rand.Next(0, 100) % 2);
        //                    if (nRandomNegative == 0)
        //                        nRandomNegative = -1;
        //                    else
        //                        nRandomNegative = 1;

        //                    //Set up particle positions and origin
        //                    m_pEmitter.pParticleArray[i].fPosX = nRandomX * nRandomNegative;
        //                    m_pEmitter.pParticleArray[i].fPosY = m_pEmitter.fEmitterPosY;
        //                    m_pEmitter.pParticleArray[i].fOriginX = nRandomX * nRandomNegative;
        //                    m_pEmitter.pParticleArray[i].fOriginY = m_pEmitter.fEmitterPosY;
        //                    break;
        //                }
        //        }

        //        //Set up lifetimes
        //        bool bRandomLifetime = checkBox4.Checked;

        //        if (bRandomLifetime == false)
        //        {
        //            //Use this if not random, just get the float and put it in all lifetime values
        //            float fStaticLifetime = (float)numericUpDown24.Value;
        //            m_pEmitter.pParticleArray[i].fMinLifetime = fStaticLifetime;
        //            m_pEmitter.pParticleArray[i].fMaxLifetime = fStaticLifetime;
        //            m_pEmitter.pParticleArray[i].fLifetime = fStaticLifetime;
        //            m_pEmitter.pParticleArray[i].fAge = 0.0f;
        //            m_pEmitter.pParticleArray[i].bAlive = true;
        //        }
        //        else if (bRandomLifetime == true)
        //        {
        //            //Calculate a random number between the given min and max values
        //            //have to use some float to int math to not truncate the float values
        //            m_pEmitter.pParticleArray[i].fMinLifetime = (float)numericUpDown15.Value;
        //            m_pEmitter.pParticleArray[i].fMaxLifetime = (float)numericUpDown6.Value;

        //            int nLifeMin = (int)(m_pEmitter.pParticleArray[i].fMinLifetime * 1000.0f);
        //            int nLifeMax = (int)(m_pEmitter.pParticleArray[i].fMaxLifetime * 1000.0f);
        //            //Check so it doesnt crash the random function call
        //            if (nLifeMin < nLifeMax)
        //            {
        //                int nLifeBuffer = rand.Next(nLifeMin, nLifeMax);
        //                m_pEmitter.pParticleArray[i].fLifetime = (float)nLifeBuffer / 1000.0f;
        //                //Give a random age within the lifetime so the particles come out one by one
        //                m_pEmitter.pParticleArray[i].fAge = (float)rand.Next(nLifeMin, nLifeBuffer) / 1000.0f;
        //            }
        //        }
        //    }
        //}
        
    }
}
