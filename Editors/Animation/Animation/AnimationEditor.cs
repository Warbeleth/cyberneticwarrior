using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Linq;
using SGD;
using System.IO;
using Animation;

namespace Animation
{
    public partial class AnimationEditor : Form
    {
        static CAnimations AnimationsEditor = new CAnimations();
        public int spritesheet;
        public int width;
        public int height;
        public String szFilepath;
        bool bSetupComplete;
        bool bAddingComplete;
        public bool bPreview;
        public Preview _previewWindow = new Preview();

        public bool BAddingComplete
        {
            get { return bAddingComplete; }
            set { bAddingComplete = value; }
        }

        public bool BSetupComplete
        {
            get { return bSetupComplete; }
            set { bSetupComplete = value; }
        }

        public class CIndex
        {
            int m_nId;
            int m_nType;

            public int _nId
            {
                get { return m_nId; }
                set { m_nId = value; }
            }

            public int _nType
            {
                get { return m_nType; }
                set { m_nType = value; }
            }

            public override string ToString()
            {
                switch (m_nType)
                {
                    case 0: // Animation box
                        {
                            return AnimationsEditor.GetAnimations[_nId].AnimationName;
                        }
                    case 1: // Frame box
                        {
                            return "Frame - [" + _nId + "]";
                        }
                    case 2: // HitRECTS box
                        {
                            return "Hit Rect - [" + _nId + "]";
                        }
                    case 3:
                        {
                            return "Collision Rect - [" + _nId + "]";
                        }
                    default:
                        {
                            return "Error - DOES NOT EXIST";
                        }
                }
            }
        }

        private bool m_bLooping;
        Point startpoint, endpoint;

        //DirectX singletons
        ManagedDirect3D m_pD3D;
        ManagedTextureManager m_pTM;
        
        public bool Looping
        {
            get { return m_bLooping; }
            set { m_bLooping = value; }
        }

        public ManagedDirect3D D3D
        {
            get { return m_pD3D; }
            set { m_pD3D = value; }
        }
        
        public ManagedTextureManager TM
        {
            get { return m_pTM; }
            set { m_pTM = value; }
        }

        public AnimationEditor()
        {
            BSetupComplete = false;
            InitializeComponent();
            Looping = true;
            spritesheet = -1;

            //Init the singletons
            D3D = ManagedDirect3D.Instance;
            TM = ManagedTextureManager.Instance;
            D3D.InitManagedDirect3D(SplitContainer2.Panel1, true);
            D3D.InitManagedDirect3D(_previewWindow.Panel1, true );
            TM.InitManagedTextureManager(ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);
            CorrectScrolling();
            AutoScrollMinSize= new Size( SplitContainer2.Panel1.Width, SplitContainer2.Panel1.Height );
            BAddingComplete = false;
            bPreview = false;

            FolderBrowserDialog dlg = new FolderBrowserDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                szFilepath = dlg.SelectedPath + "\\";
            }
            BSetupComplete = true;
        }

        public void Render()
        {
            int nOffsetX = 0 + SplitContainer2.Panel1.AutoScrollPosition.X;
            int nOffsetY = 0 + SplitContainer2.Panel1.AutoScrollPosition.Y;
            
            if (bPreview)
            {
                _previewWindow.Render();
            }

            D3D.Clear(SplitContainer2.Panel1, 240, 240, 240);

            D3D.DeviceBegin();
            D3D.SpriteBegin();

            
            if( spritesheet != -1 )
               TM.Draw(spritesheet, 0 + nOffsetX, 0 + nOffsetY, 1, 1, Rectangle.Empty, 0, 0, 0, 0 );
            
            D3D.Sprite.Flush(); // DRAW ALL SPRITES NOW!!!

            D3D.DrawLine(startpoint.X + nOffsetX, startpoint.Y + nOffsetY, endpoint.X + nOffsetX, startpoint.Y + nOffsetY, 0, 0, 0);
            D3D.DrawLine(endpoint.X + nOffsetX, startpoint.Y + nOffsetY, endpoint.X + nOffsetX, endpoint.Y + nOffsetY, 0, 0, 0);
            D3D.DrawLine(endpoint.X + nOffsetX, endpoint.Y + nOffsetY, startpoint.X + nOffsetX, endpoint.Y + nOffsetY, 0, 0, 0);
            D3D.DrawLine(startpoint.X + nOffsetX, endpoint.Y + nOffsetY, startpoint.X + nOffsetX, startpoint.Y + nOffsetY, 0, 0, 0);

            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex <= AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetTotalFrames - 1)
                    AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].RectRender( nOffsetX, nOffsetY);
   
            D3D.SpriteEnd();
            D3D.DeviceEnd();

            D3D.Present(SplitContainer2.Panel1);
        }

        public string GetAnimationName
        {
            get { return TextBox_Spritesheet.Text; }
        }

        private void AnimationEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
        }

        private void SplitContainer2_Panel1_MouseDown(object sender, MouseEventArgs e)
        {
            int nOffsetX = e.Location.X - SplitContainer2.Panel1.AutoScrollPosition.X;
            int nOffsetY = e.Location.Y - SplitContainer2.Panel1.AutoScrollPosition.Y;
            startpoint = new Point(nOffsetX, nOffsetY);
            endpoint = new Point(nOffsetX, nOffsetY);
        }

        private void SplitContainer2_Panel1_MouseMove(object sender, MouseEventArgs e)
        {
            int nOffsetX = e.Location.X - SplitContainer2.Panel1.AutoScrollPosition.X;
            int nOffsetY = e.Location.Y - SplitContainer2.Panel1.AutoScrollPosition.Y;

            if (e.Button == MouseButtons.Left)
            {
                if (nOffsetX < startpoint.X)
                {
                    startpoint.X = nOffsetX;
                }
                else if (nOffsetX > startpoint.X)
                {
                    endpoint.X = nOffsetX;
                }

                if (nOffsetY < startpoint.Y)
                {
                    startpoint.Y = nOffsetY;
                }
                else if (nOffsetY > startpoint.Y)
                {
                    endpoint.Y = nOffsetY;
                }
            }
        }

        private void SplitContainer2_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            RECT rect = new RECT();
            rect.left = startpoint.X;
            rect.top = startpoint.Y;
            rect.right = endpoint.X;
            rect.bottom = endpoint.Y;
        }

        private void Button_AddAnimation_Click(object sender, EventArgs e)
        {
            BAddingComplete = false;
            CAnimation _newAnimation = new CAnimation();
            CIndex Index = new CIndex();

            // Name
            _newAnimation.AnimationName = TextBox_AnimationTitle.Text;
            Index._nId = AnimationsEditor.GetTotalAnimations;
            Index._nType = 0;

            // Animation Time
            _newAnimation.TimeBetweenFrames = (float)numUpDown_AnimationTime.Value;

            AnimationsEditor.AddAnimation(_newAnimation);
            AnimationsListBox.Items.Add(Index);
            AnimationsListBox.SelectedIndex = Index._nId;

            ToolStripStatusLabel2.Text = AnimationsEditor.GetTotalAnimations.ToString();

            BAddingComplete = true;
        }

        private void Button_RemoveAnimation_Click(object sender, EventArgs e)
        {
            bAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.RemoveAnimation(AnimationsListBox.SelectedIndex);
                AnimationsListBox.Items.RemoveAt(AnimationsListBox.SelectedIndex);

                AnimationsListBox.SelectedIndex = -1;
                AnimationsListBox.Items.Clear();

                CIndex Index = new CIndex();

                Index._nType = 0;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetTotalAnimations; ++Index._nId)
                {
                    AnimationsListBox.Items.Add(Index);
                }

                ToolStripStatusLabel2.Text = AnimationsEditor.GetTotalAnimations.ToString();
            }
            bAddingComplete = true;
        }

        private void Button_AddFrame_Click(object sender, EventArgs e)
        {
            BAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0)
            {
                CFrame _newFrame = new CFrame();
                CIndex Index = new CIndex();
                RECT _rect = new RECT();

                // Frame Rect
                _rect.left = startpoint.X;
                _rect.top = startpoint.Y;
                _rect.right = endpoint.X;
                _rect.bottom = endpoint.Y;
                _newFrame.Frame = _rect;

                // Set The Current Rect
                numUpDown_Left.Value = startpoint.X;
                numUpDown_Top.Value = startpoint.Y;
                numUpDown_Right.Value = endpoint.X;
                numUpDown_Bottom.Value = endpoint.Y;

                // Anchor Point
                Point _Anchor = new Point();
                _Anchor.X = (int)((endpoint.X - startpoint.X) / 2.0f + startpoint.X);
                _Anchor.Y = (int)(endpoint.Y);
                _newFrame.Anchor = new Point(_Anchor.X, _Anchor.Y); 

                // Set The Current Anchor
                numUpDown_AnchorX.Value = _Anchor.X;
                numUpDown_AnchorY.Value = _Anchor.Y;

                // Trigger
                _newFrame.Trigger = 0;
                ComboBox_Triggers.SelectedIndex = 0;

                // Index
                Index._nId = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetTotalFrames;
                Index._nType = 1;

                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].AddFrame(_newFrame);
                FramesListBox.Items.Add(Index);
                FramesListBox.SelectedIndex = Index._nId;
               
            }
            BAddingComplete = true;
        }

        private void Button_RemoveFrame_Click(object sender, EventArgs e)
        {
            bAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].RemoveFrame(FramesListBox.SelectedIndex);

                FramesListBox.SelectedIndex = -1;
                FramesListBox.Items.Clear();

                CIndex Index = new CIndex();

                Index._nType = 1;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetTotalFrames; ++Index._nId)
                {
                    FramesListBox.Items.Add(Index);
                }
            }
            bAddingComplete = true;
        }

        private void Button_AddHitRECT_Click(object sender, EventArgs e)
        {
            BAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0)
            {
                RECT _rect = new RECT();
                CIndex Index = new CIndex();

                // Hit Rect
                _rect.left = startpoint.X;
                _rect.top = startpoint.Y;
                _rect.right = endpoint.X;
                _rect.bottom = endpoint.Y;

                // Set The Current Rect
                numUpDown_Left.Value = startpoint.X;
                numUpDown_Top.Value = startpoint.Y;
                numUpDown_Right.Value = endpoint.X;
                numUpDown_Bottom.Value = endpoint.Y;

                // Index
                Index._nId = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalHitRECTS;
                Index._nType = 2;

                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].AddHitRECT(_rect);
                HitRectsListBox.Items.Add(Index);
            }
            BAddingComplete = true;
        }

        private void Button_AddCollisionRECT_Click(object sender, EventArgs e)
        {
            BAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0)
            {
                RECT _rect = new RECT();
                CIndex Index = new CIndex();

                // Collision Rect
                _rect.left = startpoint.X;
                _rect.top = startpoint.Y;
                _rect.right = endpoint.X;
                _rect.bottom = endpoint.Y;

                // Set The Current Rect
                numUpDown_Left.Value = startpoint.X;
                numUpDown_Top.Value = startpoint.Y;
                numUpDown_Right.Value = endpoint.X;
                numUpDown_Bottom.Value = endpoint.Y;

                // Index
                Index._nId = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalCollisionRECTS;
                Index._nType = 3;

                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].AddCollisionRECT(_rect);
                CollisionRectsListBox.Items.Add(Index);
            }
            BAddingComplete = true;
        }

        private void Button_RemoveHitRECT_Click(object sender, EventArgs e)
        {
            bAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0 && HitRectsListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].RemoveHitRECT(HitRectsListBox.SelectedIndex);

                HitRectsListBox.SelectedIndex = -1;
                HitRectsListBox.Items.Clear();

                CIndex Index = new CIndex();

                Index._nType = 2;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalHitRECTS; ++Index._nId)
                {
                    HitRectsListBox.Items.Add(Index);
                }
            }
            bAddingComplete = true;
        }

        private void Button_RemoveCollisionRECT_Click(object sender, EventArgs e)
        {
            bAddingComplete = false;
            if (AnimationsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0 && CollisionRectsListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].RemoveCollisionRECT(CollisionRectsListBox.SelectedIndex);

                CollisionRectsListBox.SelectedIndex = -1;
                CollisionRectsListBox.Items.Clear();

                CIndex Index = new CIndex();

                Index._nType = 3;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalCollisionRECTS; ++Index._nId)
                {
                    CollisionRectsListBox.Items.Add(Index);
                }
            }
            bAddingComplete = true;
        }

        private void FramesListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            HitRectsListBox.SelectedIndex = -1;
            HitRectsListBox.Items.Clear();
            CollisionRectsListBox.SelectedIndex = -1;
            CollisionRectsListBox.Items.Clear();
            if( FramesListBox.SelectedIndex >= 0 && AnimationsListBox.SelectedIndex >= 0 )
            {
                CIndex Index = new CIndex();

                Index._nType = 2;
                for ( Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalHitRECTS; ++Index._nId)
                {
                    HitRectsListBox.Items.Add(Index);
                }

                Index._nType = 3;
                for ( Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalCollisionRECTS; ++Index._nId )
                {
                    CollisionRectsListBox.Items.Add(Index);
                }

                // Set The Current Rect
                numUpDown_Left.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.left;
                numUpDown_Top.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.top;
                numUpDown_Right.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.right;
                numUpDown_Bottom.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.bottom;

                // Set The Current Anchor
                numUpDown_AnchorX.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor.X;
                numUpDown_AnchorY.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor.Y;

                // Set The Current Trigger
                ComboBox_Triggers.SelectedIndex = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Trigger;
            }
        }

        private void AnimationsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
                FramesListBox.SelectedIndex = -1;
                FramesListBox.Items.Clear();
                if (AnimationsListBox.SelectedIndex >= 0)
                {
                    CIndex Index = new CIndex();

                    // Animation Name
                    TextBox_AnimationTitle.Text = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].AnimationName;

                    // Animation Time
                    //numUpDown_AnimationTime.Value = Convert.ToDecimal(AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].TimeBetweenFrames);

                    Index._nType = 1;
                    for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetTotalFrames; ++Index._nId)
                    {
                        FramesListBox.Items.Add(Index);

                        if (Index._nId == 0)
                            FramesListBox.SelectedIndex = 0;
                    }

                    if (FramesListBox.SelectedIndex == 0)
                    {
                        HitRectsListBox.SelectedIndex = -1;
                        HitRectsListBox.Items.Clear();
                        CollisionRectsListBox.SelectedIndex = -1;
                        CollisionRectsListBox.Items.Clear();

                        Index._nType = 2;
                        for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalHitRECTS; ++Index._nId)
                            HitRectsListBox.Items.Add(Index);

                        Index._nType = 3;
                        for (Index._nId = 0; Index._nId < AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetTotalCollisionRECTS; ++Index._nId)
                            CollisionRectsListBox.Items.Add(Index);

                        FramesListBox.SelectedIndex = -1;
                    }
                }
        }

        private void Button_Browse_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            if (DialogResult.OK == dlg.ShowDialog())
            {
                if (spritesheet != -1)
                {
                    TM.ReleaseTexture(spritesheet);
                    spritesheet = -1;
                }

                AnimationsEditor.Filename = dlg.SafeFileName;
                Bitmap bmpImage = new Bitmap(dlg.FileName);
                width = bmpImage.Width;
                height = bmpImage.Height;

                SplitContainer2.Panel1.AutoScrollMinSize = new Size(width, height);
                
                CorrectScrolling();
                spritesheet = TM.LoadTexture( szFilepath + AnimationsEditor.Filename, 0);
                TextBox_Spritesheet.Text = dlg.SafeFileName;
            }
        }

        private void FramesListBox_MouseClick(object sender, MouseEventArgs e)
        {
            CollisionRectsListBox.SelectedIndex = -1;
            HitRectsListBox.SelectedIndex = -1;
        }

        private void HitRectsListBox_MouseClick(object sender, MouseEventArgs e)
        {
            CollisionRectsListBox.SelectedIndex = -1;

            if (HitRectsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0 && AnimationsListBox.SelectedIndex >= 0)
            {
                // Set The Current Rect
                numUpDown_Left.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetHitRects[HitRectsListBox.SelectedIndex].left;
                numUpDown_Top.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetHitRects[HitRectsListBox.SelectedIndex].top;
                numUpDown_Right.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetHitRects[HitRectsListBox.SelectedIndex].right;
                numUpDown_Bottom.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetHitRects[HitRectsListBox.SelectedIndex].bottom;
            }
        }

        private void CollisionRectsListBox_MouseClick(object sender, MouseEventArgs e)
        {
            HitRectsListBox.SelectedIndex = -1;
           
            if (CollisionRectsListBox.SelectedIndex >= 0 && FramesListBox.SelectedIndex >= 0 && AnimationsListBox.SelectedIndex >= 0)
            {
                // Set The Current Rect
                numUpDown_Left.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].left;
                numUpDown_Top.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].top;
                numUpDown_Right.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].right;
                numUpDown_Bottom.Value = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].bottom;
            }
        }

        private void AnimationsListBox_MouseClick(object sender, MouseEventArgs e)
        {
            FramesListBox.SelectedIndex = -1;
            HitRectsListBox.SelectedIndex = -1;
            CollisionRectsListBox.SelectedIndex = -1;
        }

        private void SplitContainer2_Panel1_ClientSizeChanged(object sender, EventArgs e)
        {
            if (BSetupComplete)
            {
                CorrectScrolling();
            }
        }

        private void CorrectScrolling()
        {
            if (SplitContainer2.Panel1.Width >= width && SplitContainer2.Panel1.Height >= height)
                D3D.ChangeDisplayParam(SplitContainer2.Panel1, true);
            else if (SplitContainer2.Panel1.Width >= width)
                D3D.ChangeDisplayParam(SplitContainer2.Panel1, SplitContainer2.Panel1.Width - 15, SplitContainer2.Panel1.Height, true);
            else if (SplitContainer2.Panel1.Height >= height)
                D3D.ChangeDisplayParam(SplitContainer2.Panel1, SplitContainer2.Panel1.Width, SplitContainer2.Panel1.Height - 15, true);
            else
                D3D.ChangeDisplayParam(SplitContainer2.Panel1, SplitContainer2.Panel1.Width - 15, SplitContainer2.Panel1.Height - 15, true);
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void numUpDown_Left_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (CollisionRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].left = (int)numUpDown_Left.Value;
                    return;
                }

                if (HitRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[HitRectsListBox.SelectedIndex].left = (int)numUpDown_Left.Value;
                    return;
                }

                if (FramesListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.left = (int)numUpDown_Left.Value;
                    return;
                }
            }
        }

        private void numUpDown_Right_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (CollisionRectsListBox.SelectedIndex >= 0)
                {
                      AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].right = (int)numUpDown_Right.Value;
                    return;
                }

                if (HitRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[HitRectsListBox.SelectedIndex].right = (int)numUpDown_Right.Value;
                    return;
                }

                if (FramesListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.right = (int)numUpDown_Right.Value;
                    return;
                }
            }
        }

        private void numUpDown_Top_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (CollisionRectsListBox.SelectedIndex >= 0)
                {
                    AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].top = (int)numUpDown_Top.Value;
                    return;
                }

                if (HitRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[HitRectsListBox.SelectedIndex].top = (int)numUpDown_Top.Value;
                    return;
                }

                if (FramesListBox.SelectedIndex >= 0)
                {
                    AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.top = (int)numUpDown_Top.Value;
                    return;
                }
            }
        }

        private void numUpDown_Bottom_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (CollisionRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[CollisionRectsListBox.SelectedIndex].bottom = (int)numUpDown_Bottom.Value;
                    return;
                }

                if (HitRectsListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].GetCollisionRects[HitRectsListBox.SelectedIndex].bottom = (int)numUpDown_Bottom.Value;
                    return;
                }

                if (FramesListBox.SelectedIndex >= 0)
                {
                     AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Frame.bottom = (int)numUpDown_Bottom.Value;
                    return;
                }
            }
        }

        private void numUpDown_AnchorX_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (FramesListBox.SelectedIndex >= 0)
                {
                    Point ptNewAnchor = new Point();
                    ptNewAnchor.Y = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor.Y;
                    ptNewAnchor.X = (int)numUpDown_AnchorX.Value;

                    AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor = ptNewAnchor;
                }
            }
        }

        private void numUpDown_AnchorY_ValueChanged(object sender, EventArgs e)
        {
            if (BAddingComplete)
            {
                if (FramesListBox.SelectedIndex >= 0)
                {

                    Point ptNewAnchor = new Point();
                    ptNewAnchor.Y = (int)numUpDown_AnchorY.Value;
                    ptNewAnchor.X = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor.X;

                    AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Anchor = ptNewAnchor;
                }
            }
        }

        private void PreviewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (AnimationsListBox.SelectedIndex >= 0)
            {
                _previewWindow.CurrentlyPlaying = AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex];
                _previewWindow.nImageID = spritesheet;

                _previewWindow.Show(this);
            }
            bPreview = true;
        }

        private void ComboBox_Triggers_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (FramesListBox.SelectedIndex >= 0 && AnimationsListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetFrames[FramesListBox.SelectedIndex].Trigger = ComboBox_Triggers.SelectedIndex;
            }
        }

        private void numUpDown_AnimationTime_ValueChanged(object sender, EventArgs e)
        {
            if (AnimationsListBox.SelectedIndex >= 0)
            {
                AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].TimeBetweenFrames = (float)numUpDown_AnimationTime.Value / AnimationsEditor.GetAnimations[AnimationsListBox.SelectedIndex].GetTotalFrames;
            }
        }

        private void ExportAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "Binary Animation File (*.bae)|*.bae";
            dlg.DefaultExt = "bae";
           
            if (DialogResult.OK == dlg.ShowDialog())
            {
                BinaryWriter _export = new BinaryWriter(File.Create(dlg.FileName));

                // spritesheet filename
                _export.Write(AnimationsEditor.Filename);

                // total animations
                _export.Write(AnimationsEditor.GetTotalAnimations);

                // each animation
                for( int i = 0; i < AnimationsEditor.GetTotalAnimations; ++i )
                {
                    // time between frames
                    _export.Write(AnimationsEditor.GetAnimations[i].TimeBetweenFrames);

                    // total frames
                    _export.Write(AnimationsEditor.GetAnimations[i].GetTotalFrames);

                    // each frame
                    for (int x = 0; x < AnimationsEditor.GetAnimations[i].GetTotalFrames; ++x)
                    {
                        // anchor
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Anchor.X);
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Anchor.Y);

                        // frame
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Frame.left);
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Frame.top);
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Frame.right);
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Frame.bottom);

                        // trigger
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].Trigger);

                        // total hit rects
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetTotalHitRECTS);

                        // each hit rect
                        for (int y = 0; y < AnimationsEditor.GetAnimations[i].GetFrames[x].GetTotalHitRECTS; ++y)
                        {
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetHitRects[y].left);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetHitRects[y].top);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetHitRects[y].right);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetHitRects[y].bottom);
                        }

                        // total collision rects
                        _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetTotalCollisionRECTS);

                        // each collision rect
                        for (int z = 0; z < AnimationsEditor.GetAnimations[i].GetFrames[x].GetTotalCollisionRECTS; ++z)
                        {
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetCollisionRects[z].left);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetCollisionRects[z].top);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetCollisionRects[z].right);
                            _export.Write(AnimationsEditor.GetAnimations[i].GetFrames[x].GetCollisionRects[z].bottom);
                        }
                    }
                }
                _export.Close();
            }
        }

        private void LoadAnimationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "Binary Animation File (*.bae)|*.bae";
            dlg.DefaultExt = "bae";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                AnimationsListBox.Items.Clear();
                AnimationsListBox.SelectedIndex = -1;
                FramesListBox.Items.Clear();
                FramesListBox.SelectedIndex = -1;
                HitRectsListBox.Items.Clear();
                HitRectsListBox.SelectedIndex = -1;
                CollisionRectsListBox.Items.Clear();
                CollisionRectsListBox.SelectedIndex = -1;
                bAddingComplete = false;

                AnimationsEditor = new CAnimations();
                BinaryReader _import = new BinaryReader(File.Open(dlg.FileName, FileMode.Open));

                // spritesheet filename
                AnimationsEditor.Filename = _import.ReadString();

                // total animations
                AnimationsEditor.GetTotalAnimations = _import.ReadInt32();

                // each animation
                CIndex Index = new CIndex();
                Index._nType = 0;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetTotalAnimations; ++Index._nId)
                {
                    // add the animation
                    CAnimation _animation = new CAnimation();
                    AnimationsEditor.AddAnimation(_animation);
                    AnimationsEditor.GetTotalAnimations--;

                    // time between frames
                    AnimationsEditor.GetAnimations[Index._nId].TimeBetweenFrames = _import.ReadInt32();

                    // Animation Name
                    AnimationsEditor.GetAnimations[Index._nId].AnimationName = "Default[" + Index._nId + "]";

                    // total frames
                    AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames = _import.ReadInt32();

                    // each frame
                    CIndex Index2 = new CIndex();
                    Index2._nType = 1;
                    for (Index2._nId = 0; Index2._nId < AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames; ++Index2._nId)
                    {
                        // add the frame
                        CFrame _frame = new CFrame();
                        AnimationsEditor.GetAnimations[Index._nId].AddFrame(_frame);
                        AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames--;

                        // anchor
                        Point _anchor = new Point();
                        _anchor.X = _import.ReadInt32();
                        _anchor.Y = _import.ReadInt32();
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Anchor = _anchor;

                        // frame
                        RECT _frameRect = new RECT();
                        _frameRect.left = _import.ReadInt32();
                        _frameRect.top = _import.ReadInt32();
                        _frameRect.right = _import.ReadInt32();
                        _frameRect.bottom = _import.ReadInt32();
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame = _frameRect;

                        // trigger
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Trigger = _import.ReadInt32();

                        // total hit rects
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS = _import.ReadInt32();

                        // each hit rect
                        CIndex Index3 = new CIndex();
                        Index3._nType = 2;
                        for (Index3._nId = 0; Index3._nId < AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS; ++Index3._nId)
                        {
                            RECT _hitRect = new RECT();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].AddHitRECT(_hitRect);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS--;
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].left = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].top = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].right = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].bottom = _import.ReadInt32();
                        }

                        // total collision rects
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS = _import.ReadInt32();

                        // each collision rect
                        Index3._nType = 3;
                        for (Index3._nId = 0; Index3._nId < AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS; ++Index3._nId)
                        {
                            RECT _collisionRect = new RECT();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].AddCollisionRECT(_collisionRect);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS--;
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].left = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].top = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].right = _import.ReadInt32();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].bottom = _import.ReadInt32();
                        }
                    }
                    AnimationsListBox.Items.Add(Index);
                }
                _import.Close();
                bAddingComplete = true;

                if( spritesheet != -1)
                {
                    TM.ReleaseTexture(spritesheet);
                    spritesheet = -1;
                }


                Bitmap bmpImage = new Bitmap(szFilepath + AnimationsEditor.Filename);
                width = bmpImage.Width;
                height = bmpImage.Height;
                CorrectScrolling();

                spritesheet = TM.LoadTexture(szFilepath + AnimationsEditor.Filename, 0);
                TextBox_AnimationTitle.Text = AnimationsEditor.Filename;
            }


        }

        private void ToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "XML Animation File (*.xae)|*.xae";
            dlg.DefaultExt = "xae";


            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement _export = new XElement("XMLAnimations");

                // spritesheet filename
                XAttribute xSpritesheetName = new XAttribute("SpritesheetName", AnimationsEditor.Filename );
                _export.Add(xSpritesheetName);

                // total animations
                XAttribute xTotalAnimations = new XAttribute("TotalAnimations", AnimationsEditor.GetTotalAnimations);
                _export.Add(xTotalAnimations);

                // each animation
                CIndex Index = new CIndex();
                Index._nType = 0;
                for (Index._nId = 0; Index._nId < AnimationsEditor.GetTotalAnimations; ++Index._nId)
                {
                    // add the animation
                    XElement xAnimation = new XElement("Animation");
                    _export.Add(xAnimation);

                    // time between frames
                    XAttribute xTimeBetweenFrames = new XAttribute("TimeBetweenFrames", AnimationsEditor.GetAnimations[Index._nId].TimeBetweenFrames);
                    xAnimation.Add(xTimeBetweenFrames);

                    // Animation Name
                    XAttribute xAnimationName = new XAttribute("AnimationName", AnimationsEditor.GetAnimations[Index._nId].AnimationName);
                    xAnimation.Add(xAnimationName);

                    // total frames
                    XAttribute xTotalFrames = new XAttribute("TotalFrames", AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames);
                    xAnimation.Add(xTotalFrames);

                    // each frame
                    CIndex Index2 = new CIndex();
                    Index2._nType = 1;
                    for (Index2._nId = 0; Index2._nId < AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames; ++Index2._nId)
                    {
                        // add the frame
                        XElement xFrame = new XElement("Frame");
                        xAnimation.Add(xFrame);

                        // anchor
                        XAttribute xAnchorX = new XAttribute("Anchor.X", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Anchor.X);
                        XAttribute xAnchorY = new XAttribute("Anchor.Y", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Anchor.Y);
                        xFrame.Add(xAnchorX);
                        xFrame.Add(xAnchorY);

                        // frame
                        XAttribute xFrameLeft = new XAttribute("Frame.left", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.left);
                        XAttribute xFrameTop = new XAttribute("Frame.top", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.top);
                        XAttribute xFrameRight = new XAttribute("Frame.right", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.right);
                        XAttribute xFrameBottom = new XAttribute("Frame.bottom", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.bottom);
                        xFrame.Add(xFrameLeft);
                        xFrame.Add(xFrameTop);
                        xFrame.Add(xFrameRight);
                        xFrame.Add(xFrameBottom);

                        // trigger
                        XAttribute xTrigger = new XAttribute("Trigger", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Trigger);
                        xFrame.Add(xTrigger);

                        // total hit rects
                        XAttribute xTotalHitRects = new XAttribute("TotalHitRects", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS);
                        xFrame.Add(xTotalHitRects);

                        // each hit rect
                        CIndex Index3 = new CIndex();
                        Index3._nType = 2;
                        for (Index3._nId = 0; Index3._nId < AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS; ++Index3._nId)
                        {
                            XElement xHitRect = new XElement("HitRect");
                            xFrame.Add(xHitRect);

                            XAttribute xHitRectLeft = new XAttribute("HitRect.left", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].left);
                            XAttribute xHitRectTop = new XAttribute("HitRect.top", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].top);
                            XAttribute xHitRectRight = new XAttribute("HitRect.right", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].right);
                            XAttribute xHitRectBottom = new XAttribute("HitRect.bottom", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].bottom);
                            xHitRect.Add(xHitRectLeft);
                            xHitRect.Add(xHitRectTop);
                            xHitRect.Add(xHitRectRight);
                            xHitRect.Add(xHitRectBottom);
                        }

                        // total collision rects
                        XAttribute xTotalCollisionRects = new XAttribute("TotalCollisionRects", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS);
                        xFrame.Add(xTotalCollisionRects);

                        // each collision rect
                        Index3._nType = 3;
                        for (Index3._nId = 0; Index3._nId < AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS; ++Index3._nId)
                        {
                            XElement xCollisionRect = new XElement("CollisionRect");
                            xFrame.Add(xCollisionRect);

                            XAttribute xCollisionRectLeft = new XAttribute("CollisionRect.left", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].left);
                            XAttribute xCollisionRectTop = new XAttribute("CollisionRect.top", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].top);
                            XAttribute xCollisionRectRight = new XAttribute("CollisionRect.right", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].right);
                            XAttribute xCollisionRectBottom = new XAttribute("CollisionRect.bottom", AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].bottom);
                            xCollisionRect.Add(xCollisionRectLeft);
                            xCollisionRect.Add(xCollisionRectTop);
                            xCollisionRect.Add(xCollisionRectRight);
                            xCollisionRect.Add(xCollisionRectBottom);
                        }
                    }
                }
                _export.Save(dlg.FileName);
            }
        }

        private void ToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "XML Animation File (*.xae)|*.xae";
            dlg.DefaultExt = "xae";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                bAddingComplete = false;
                AnimationsListBox.Items.Clear();
                AnimationsListBox.SelectedIndex = -1;
                FramesListBox.Items.Clear();
                FramesListBox.SelectedIndex = -1;
                HitRectsListBox.Items.Clear();
                HitRectsListBox.SelectedIndex = -1;
                CollisionRectsListBox.Items.Clear();
                CollisionRectsListBox.SelectedIndex = -1;

                AnimationsEditor = new CAnimations();
                XElement _import = XElement.Load(dlg.FileName);
                IEnumerable<XElement> xAnimations = _import.Elements("Animation");
                IEnumerable<XElement> xFrames = xAnimations.Elements("Frame");
                IEnumerable<XElement> xHitRects = xFrames.Elements("HitRect");
                IEnumerable<XElement> xCollisionRects = xFrames.Elements("CollisionRect");

                // spritesheet filename
                XAttribute xSpritesheetName = _import.Attribute("SpritesheetName");
                AnimationsEditor.Filename = Convert.ToString(xSpritesheetName.Value);

                // total animations
                XAttribute xTotalAnimations = _import.Attribute("TotalAnimations");
                AnimationsEditor.GetTotalAnimations = Convert.ToInt32(xTotalAnimations.Value);

                // each animation
                CIndex Index = new CIndex();
                Index._nType = 0;
                Index._nId = 0; 
                foreach (XElement xAnimation in xAnimations)
                {
                    // add the animation
                    CAnimation _animation = new CAnimation();
                    AnimationsEditor.AddAnimation(_animation);
                    AnimationsEditor.GetTotalAnimations--;

                    // time between frames
                    XAttribute xTimeBetweenFrames = xAnimation.Attribute("TimeBetweenFrames");
                    AnimationsEditor.GetAnimations[Index._nId].TimeBetweenFrames = (float)Convert.ToDecimal(xTimeBetweenFrames.Value);

                    // Animation Name
                    XAttribute xAnimationName = xAnimation.Attribute("AnimationName");
                    AnimationsEditor.GetAnimations[Index._nId].AnimationName = Convert.ToString(xAnimationName.Value);

                    // total frames
                    XAttribute xTotalFrames = xAnimation.Attribute("TotalFrames");
                    AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames = Convert.ToInt32(xTotalFrames.Value);

                    // each frame
                    CIndex Index2 = new CIndex();
                    Index2._nType = 1;
                    Index2._nId = 0;
                    foreach (XElement xFrame in xFrames)
                    {
                        // add the frame
                        CFrame _frame = new CFrame();
                        AnimationsEditor.GetAnimations[Index._nId].AddFrame(_frame);
                        AnimationsEditor.GetAnimations[Index._nId].GetTotalFrames--;

                        // anchor
                        XAttribute xAnchorX = xFrame.Attribute("Anchor.X");
                        XAttribute xAnchorY = xFrame.Attribute("Anchor.Y");
                        Point ptAnchor = new Point( Convert.ToInt32(xAnchorX.Value), Convert.ToInt32(xAnchorY.Value) );
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Anchor = ptAnchor;

                        // frame
                        XAttribute xFrameLeft = xFrame.Attribute("Frame.left");
                        XAttribute xFrameTop = xFrame.Attribute("Frame.top"); 
                        XAttribute xFrameRight = xFrame.Attribute("Frame.right");
                        XAttribute xFrameBottom = xFrame.Attribute("Frame.bottom");
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.left = Convert.ToInt32(xFrameLeft.Value);
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.top = Convert.ToInt32(xFrameTop.Value);
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.right = Convert.ToInt32(xFrameRight.Value);
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Frame.bottom = Convert.ToInt32(xFrameBottom.Value);

                        // trigger
                        XAttribute xTrigger = xFrame.Attribute("Trigger");
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].Trigger = Convert.ToInt32(xTrigger.Value);

                        // total hit rects
                        XAttribute xTotalHitRects = xFrame.Attribute("TotalHitRects");
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS = Convert.ToInt32(xTotalHitRects.Value);

                        // each hit rect
                        CIndex Index3 = new CIndex();
                        Index3._nType = 2;
                        Index3._nId = 0;
                        foreach (XElement xHitRect in xHitRects)
                        {
                            RECT _rect = new RECT();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].AddHitRECT(_rect);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalHitRECTS--;
                            
                            XAttribute xHitRectLeft = xHitRect.Attribute("HitRect.left");
                            XAttribute xHitRectTop = xHitRect.Attribute("HitRect.top");
                            XAttribute xHitRectRight = xHitRect.Attribute("HitRect.right");
                            XAttribute xHitRectBottom = xHitRect.Attribute("HitRect.bottom");
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].left = Convert.ToInt32(xHitRectLeft.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].top = Convert.ToInt32(xHitRectTop.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].right = Convert.ToInt32(xHitRectRight.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetHitRects[Index3._nId].bottom = Convert.ToInt32(xHitRectBottom.Value);
                       
                            Index3._nId++;
                        }


                        // total collision rects
                        XAttribute xTotalCollisionRects = xFrame.Attribute("TotalCollisionRects");
                        AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS = Convert.ToInt32(xTotalCollisionRects.Value);

                        // each collision rect
                        Index3._nType = 3;
                        Index3._nId = 0;
                        foreach (XElement xCollisionRect in xCollisionRects)
                        {
                            RECT _rect = new RECT();
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].AddCollisionRECT(_rect);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetTotalCollisionRECTS--;

                            XAttribute xCollisionRectLeft = xCollisionRect.Attribute("CollisionRect.left");
                            XAttribute xCollisionRectTop = xCollisionRect.Attribute("CollisionRect.top");
                            XAttribute xCollisionRectRight = xCollisionRect.Attribute("CollisionRect.right");
                            XAttribute xCollisionRectBottom = xCollisionRect.Attribute("CollisionRect.bottom");
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].left = Convert.ToInt32(xCollisionRectLeft.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].top = Convert.ToInt32(xCollisionRectTop.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].right = Convert.ToInt32(xCollisionRectRight.Value);
                            AnimationsEditor.GetAnimations[Index._nId].GetFrames[Index2._nId].GetCollisionRects[Index3._nId].bottom = Convert.ToInt32(xCollisionRectBottom.Value);
                            
                            Index3._nId++;
                        }
                        Index2._nId++;
                    }
                    AnimationsListBox.Items.Add(Index);
                    Index._nId++;
                }
                bAddingComplete = true;

                if( spritesheet != -1 )
                {
                    TM.ReleaseTexture(spritesheet);
                    spritesheet = -1;
                } 
                
                Bitmap bmpImage = new Bitmap(szFilepath+AnimationsEditor.Filename);
                width = bmpImage.Width;
                height = bmpImage.Height;
                CorrectScrolling();

                spritesheet = TM.LoadTexture(szFilepath + AnimationsEditor.Filename, 0);
                TextBox_AnimationTitle.Text = AnimationsEditor.Filename;

            }
        }

        private void SplitContainer2_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}