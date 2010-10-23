using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Animation;
using SGD;

namespace Animation
{
    public partial class Preview : Form
    {
        CAnimation _currentlyPlaying = new CAnimation();
        public float fTotalTime = new float();
        public float fPrevTime = new float();
        public float fElapsedtime = new float();
        public bool bLooping = new bool();
        public bool bPlay = new bool();
        public int nImageID = new int();

        public CAnimation CurrentlyPlaying
        {
            get { return _currentlyPlaying; }
            set { _currentlyPlaying = value; }
        }

        public Preview()
        {
            InitializeComponent();
            fTotalTime = 0.0f;
            fPrevTime = 0.0f;
            fElapsedtime = 0.0f;
            bLooping = true;
            bPlay = false;
        }

        public void Render(  )
        {
            if (bPlay)
            {
                Point offset = new Point();
                ManagedDirect3D D3D = ManagedDirect3D.Instance;
                ManagedTextureManager pTM = ManagedTextureManager.Instance;
                fPrevTime = fTotalTime;
                fTotalTime = (Environment.TickCount / 1000.0f);
                fElapsedtime += (fTotalTime - fPrevTime);

                if (fElapsedtime >= 1)//_currentlyPlaying.TimeBetweenFrames)
                {
                    fElapsedtime = 0.0f;
                    _currentlyPlaying.CurrentFrame++;

                    if (bLooping)
                    {
                        if (_currentlyPlaying.CurrentFrame > _currentlyPlaying.GetTotalFrames - 1)
                        {
                            _currentlyPlaying.CurrentFrame = 0;
                        }
                    }
                    else
                    {
                        if (_currentlyPlaying.CurrentFrame > _currentlyPlaying.GetTotalFrames - 1)
                            _currentlyPlaying.CurrentFrame = _currentlyPlaying.GetTotalFrames - 1;
                    }
                }
                    
                    offset.X = _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Anchor.X - _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.left;
                    offset.Y = _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Anchor.Y - _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.top;

                    Rectangle _rect = new Rectangle(_currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.left, _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.top,
                   _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.right - _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.left,
                   _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.bottom - _currentlyPlaying.GetFrames[_currentlyPlaying.CurrentFrame].Frame.top);
                 
                    D3D.Clear(Panel1, 240, 240, 240);

                    D3D.DeviceBegin();
                    D3D.SpriteBegin();
                    
                    pTM.Draw(nImageID, 100 - offset.X, 100 - offset.Y, 1, 1, _rect, 0, 0, 0, Color.White.ToArgb());

                    D3D.SpriteEnd();
                    D3D.DeviceEnd();

                    D3D.Present(Panel1);
                
            }
        }

        private void Button3_Click(object sender, EventArgs e)
        {
            bPlay = true;
            bLooping = false;
            _currentlyPlaying.CurrentFrame = 0;
            fElapsedtime = 0.0f;
            fPrevTime = (Environment.TickCount / 1000.0f);
            fTotalTime = (Environment.TickCount / 1000.0f);
            ToolStripStatusLabel2.Text = _currentlyPlaying.AnimationName;
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            bPlay = true;
            bLooping = true;
            _currentlyPlaying.CurrentFrame = 0;
            fElapsedtime = 0.0f;
            fPrevTime = (Environment.TickCount / 1000.0f);
            fTotalTime = (Environment.TickCount / 1000.0f);
            ToolStripStatusLabel2.Text = _currentlyPlaying.AnimationName;
        }

    }
}
