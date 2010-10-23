using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SGD;
using System.Drawing;

namespace Animation
{       
    public class RECT
    {
        public int  left;
        public int  top;
        public int  right;
        public int  bottom;

        public RECT(  )
        {
            left = 0;
            top = 0;
            right = 0;
            bottom = 0;
        }
    }

    public class CAnimation
    {
        int         m_nTotalFrames;
        int         m_nCurrentFrame;
        float       m_nTimeBetweenFrames;
        float       m_nTotalAnimationTime;
        List<CFrame> m_lstFrames = new List<CFrame>();
        String      m_szAnimationName;

        public CAnimation(  )
        {
            m_nTotalFrames      = 0;
            m_nCurrentFrame     = 0;
            m_nTimeBetweenFrames = 0.0f;
            m_nTotalAnimationTime = 0.0f;
            m_lstFrames.Clear();
            m_szAnimationName = String.Empty;
        }

        // Animation Methods
        public void AddFrame( CFrame _frame )
        {
            ++m_nTotalFrames;
            m_lstFrames.Add( _frame );

            m_nTimeBetweenFrames = m_nTotalAnimationTime / m_nTotalFrames;
        }

        public bool RemoveFrame( int _FrameIndex )
        {
            if ( m_lstFrames[_FrameIndex] != null )
            {
                --m_nTotalFrames;
                m_lstFrames.RemoveAt(_FrameIndex);

                m_nTimeBetweenFrames = m_nTotalAnimationTime / m_nTotalFrames;
                return true;
            }
            else
            {
                return false;
            }
        }

        public void Render()
        {
            for (int i = 0; i < GetTotalFrames; ++i)
                m_lstFrames[i].Render();
        }


        // Accessors / Mutators
        public String AnimationName
        {
            get { return m_szAnimationName; }
            set { m_szAnimationName = value; }
        }

        public int GetTotalFrames
        {
            get { return m_nTotalFrames; }
            set { m_nTotalFrames = value; }
        }

        public List<CFrame> GetFrames
        {
            get { return m_lstFrames; }
        }

        public int CurrentFrame
        {
            get { return m_nCurrentFrame; }
            set { m_nCurrentFrame = value; }
        }

        public float TimeBetweenFrames
        {
            get { return m_nTimeBetweenFrames; }
            set { m_nTimeBetweenFrames = value; }
        }

        public override string ToString()
        {
            return AnimationName;
        }
    }

    public class CFrame
    {
        RECT        m_rFrame = new RECT();
        Point       m_pAnchor;
        int         m_nTotalHitRects;
        int         m_nTotalCollisionRects;
        int         m_nTrigger;
        List<RECT>  m_lstHitRects = new List<RECT>();
        List<RECT>  m_lstCollisionRects = new List<RECT>();

        public CFrame(  )
        {
           m_rFrame.left            = 0;
           m_rFrame.top             = 0;
           m_rFrame.right           = 0;
           m_rFrame.bottom          = 0;
           m_pAnchor.X              = 0;
           m_pAnchor.Y              = 0;
           m_nTotalHitRects         = 0;
           m_nTotalCollisionRects   = 0;
           m_nTrigger               = 0;
           m_lstHitRects.Clear();
           m_lstCollisionRects.Clear();
        }

        // CFrame Methods
        public void AddHitRECT( RECT _rect )
        {
            ++m_nTotalHitRects;
            m_lstHitRects.Add( _rect );
        }

        public void Render()
        {
        }

        public void RectRender()
        { 
            //DirectX singletons
            ManagedDirect3D pD3D = ManagedDirect3D.Instance;

            // Draw Frame
            pD3D.DrawLine(Frame.left, Frame.top, Frame.right, Frame.top, 255, 0, 0);
            pD3D.DrawLine(Frame.right, Frame.top, Frame.right, Frame.bottom, 255, 0, 0);
            pD3D.DrawLine(Frame.right, Frame.bottom, Frame.left, Frame.bottom, 255, 0, 0);
            pD3D.DrawLine(Frame.left, Frame.bottom, Frame.left, Frame.top, 255, 0, 0);

            pD3D.DrawLine(Anchor.X - 2, Anchor.Y - 2, Anchor.X + 2, Anchor.Y + 2, 255, 0, 255);
            pD3D.DrawLine(Anchor.X + 2, Anchor.Y - 2, Anchor.X - 2, Anchor.Y + 2, 255, 0, 255);
            // Draw Hit Rects
            for (int i = 0; i < GetTotalHitRECTS; ++i)
            {
                pD3D.DrawLine(m_lstHitRects[i].left, m_lstHitRects[i].top, m_lstHitRects[i].right, m_lstHitRects[i].top, 0, 255, 0);
                pD3D.DrawLine(m_lstHitRects[i].right, m_lstHitRects[i].top, m_lstHitRects[i].right, m_lstHitRects[i].bottom, 0, 255, 0);
                pD3D.DrawLine(m_lstHitRects[i].right, m_lstHitRects[i].bottom, m_lstHitRects[i].left, m_lstHitRects[i].bottom, 0, 255, 0);
                pD3D.DrawLine(m_lstHitRects[i].left, m_lstHitRects[i].bottom, m_lstHitRects[i].left, m_lstHitRects[i].top, 0, 255, 0);
            }

            // Draw Collision Rects
            for (int i = 0; i < GetTotalCollisionRECTS; ++i)
            {
                pD3D.DrawLine(m_lstCollisionRects[i].left, m_lstCollisionRects[i].top, m_lstCollisionRects[i].right, m_lstCollisionRects[i].top, 0, 0, 255);
                pD3D.DrawLine(m_lstCollisionRects[i].right, m_lstCollisionRects[i].top, m_lstCollisionRects[i].right, m_lstCollisionRects[i].bottom, 0, 0, 255);
                pD3D.DrawLine(m_lstCollisionRects[i].right, m_lstCollisionRects[i].bottom, m_lstCollisionRects[i].left, m_lstCollisionRects[i].bottom, 0, 0, 255);
                pD3D.DrawLine(m_lstCollisionRects[i].left, m_lstCollisionRects[i].bottom, m_lstCollisionRects[i].left, m_lstCollisionRects[i].top, 0, 0, 255);
            }
        }

        public bool RemoveHitRECT(int _RECTIndex)
        {
            if( m_lstHitRects[_RECTIndex] != null )
            {
                --m_nTotalHitRects;
                m_lstHitRects.RemoveAt( _RECTIndex );
                return true;
            }
            else
            {
                return false;
            }
        }

        public void AddCollisionRECT( RECT _rect )
        {
            ++m_nTotalCollisionRects;
            m_lstCollisionRects.Add(_rect);
        }

        public bool RemoveCollisionRECT( int _RECTIndex )
        {
            if( m_lstCollisionRects[_RECTIndex] != null )
            {
                --m_nTotalCollisionRects;
                m_lstCollisionRects.RemoveAt( _RECTIndex );
                return true;
            }
            else
            {
                return false;
            }
        }

        //Accessors / Mutators
        public RECT Frame
        {
            get { return m_rFrame; }
            set { m_rFrame = value; }
        }

        public Point Anchor
        {
            get { return m_pAnchor; }
            set { m_pAnchor = value; }
        }

        public int GetTotalHitRECTS
        {
            get { return m_nTotalHitRects; }
            set { m_nTotalHitRects = value; }
        }

        public int GetTotalCollisionRECTS
        {
            get { return m_nTotalCollisionRects; }
            set { m_nTotalCollisionRects = value; }
        }

        public List<RECT> GetHitRects
        {
            get { return m_lstHitRects; }
        }

        public List<RECT> GetCollisionRects
        {
            get { return m_lstCollisionRects; }
        }

        public int Trigger
        {
            get { return m_nTrigger; }
            set { m_nTrigger = value; }
        }
    }

    public class CAnimations
    {
        int                 m_nTotalAnimations;
        String              m_szFilename;
        List<CAnimation>    m_lstAnimtaions = new List<CAnimation>();

        // Default Constructor
        public CAnimations( )
        {
            m_nTotalAnimations      = 0;
            m_szFilename            = "\0";
            m_lstAnimtaions.Clear();
        }

        // CAnimation Methods
        public void AddAnimation( CAnimation _Animation )
        {
            ++m_nTotalAnimations;
            m_lstAnimtaions.Add( _Animation );
        }

        public bool RemoveAnimation( int _AnimationIndex )
        {
            if ( m_lstAnimtaions[_AnimationIndex] != null )
            {
                --m_nTotalAnimations;
                m_lstAnimtaions.RemoveAt(_AnimationIndex);
                return true;
            }
            else
            {
                return false;
            }
        }

        // Accessors / Mutators
        public int GetTotalAnimations
        {
            get { return m_nTotalAnimations; }
            set { m_nTotalAnimations = value; }
        }

        public List<CAnimation> GetAnimations
        {
            get { return m_lstAnimtaions; }
        }

        public String Filename
        {
            get { return m_szFilename; }
            set { m_szFilename = value; }
        }

        public override string ToString()
        {
            return Filename;
        }
    }
}