using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SGD;
using Types;
using System.IO;

namespace SGP_Map_Editor
{  
    //Formulas
    //Index = Row * TotalCols + Cols
    //y = i/width
    //x = i%width
    //Left = Col * Width
    //Right = Left + Width
    //Top = Row * Height
    //Bottom = Top * Height

    public partial class Map_Editor : Form
    {
        //Editor Loop
        int m_nCollisionSelection;
        bool m_bLooping;
        bool m_bSetupComplete;
        bool m_bDrawing;
        float m_fScaleX;
        float m_fScaleY;

        //Variables required for the map and the selection
        public Scales m_sScale;
        public Grid m_gTileMap;
        public Selection m_sTileSelection;
        public List<ListObject> m_lMap;
        public List<ListObject> m_lTempMap;
        public string m_szWorkSpace;
        public string m_szTileSet;

        //DirectX singletons
        ManagedDirect3D m_pD3D;
        ManagedTextureManager m_pTM;

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
        public bool Looping
        {
            get { return m_bLooping; }
            set { m_bLooping = value; }
        }
        public bool SetupComplete
        {
            get { return m_bSetupComplete; }
            set { m_bSetupComplete = value; }
        }
        public bool Drawing
        {
            get { return m_bDrawing; }
            set { m_bDrawing = value; }
        }

        public float ScaleX
        {
            get { return m_fScaleX; }
            set { m_fScaleX = value; }
        }
        public float ScaleY
        {
            get { return m_fScaleY; }
            set { m_fScaleY = value; }
        }
        public int CollisionSelection
        {
            get { return m_nCollisionSelection; }
            set { m_nCollisionSelection = value; }
        }

        public Map_Editor()
        {
            SetupComplete = false;

            Looping = true;
            InitializeComponent();

            //Initialize Numeric Up Down Panels

            //Grid
            numericGridCol.Value = 10;
            numericGridRow.Value = 10;
            numericGridTileWidth.Value = 64;
            numericGridTileHeight.Value = 64;

            //Tileset
            numericSourceCol.Value = 4;
            numericSourceRow.Value = 2;
            numericSourceTileWidth.Value = 64;
            numericSourceTileHeight.Value = 64;
            
            //Run Editor
            ScaleX = (float)(numericGridTileWidth.Value / numericSourceTileWidth.Value);
            ScaleY = (float)(numericGridTileHeight.Value / numericSourceTileHeight.Value);

            m_sScale = new Scales((float)numericGridTileWidth.Value, (float)numericGridTileHeight.Value, (float)numericSourceTileWidth.Value, (float)numericSourceTileHeight.Value);
            m_gTileMap = new Grid(Convert.ToInt32(numericGridCol.Value), Convert.ToInt32(numericGridRow.Value), Convert.ToInt32(numericGridTileWidth.Value), Convert.ToInt32(numericGridTileHeight.Value));
            m_sTileSelection = new Selection(Convert.ToInt32(numericSourceCol.Value), Convert.ToInt32(numericSourceRow.Value), Convert.ToInt32(numericSourceTileWidth.Value), Convert.ToInt32(numericSourceTileHeight.Value));
            m_lMap = new List<ListObject>();
            m_lTempMap = new List<ListObject>();

            //Initialize the Map
            CreateList(m_gTileMap.m_nRows, m_gTileMap.m_nColumns);


            //Tab setup
            tabControlMain.SelectedIndex = 1;

            //Collision tab setup- includes default selection, and colors
            CollisionSelection = 0;
            comboEnemy.SelectedIndex = 0;
            comboCollision.SelectedIndex = 0;
            comboSpawn.SelectedIndex = 0;
            CollisionSelection = 0;
            buttonEnemy.BackColor = Color.Green;
            buttonSpawner.BackColor = Color.Red;
            buttonCollision.BackColor = Color.Red;

            //Auto-Scrolling
            splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);

            //Init the singletons
            D3D = ManagedDirect3D.Instance;
            TM = ManagedTextureManager.Instance;
            D3D.InitManagedDirect3D(splitContainerMap.Panel2, true);
            D3D.InitManagedDirect3D(splitContainerBackground.Panel2, true);
            D3D.InitManagedDirect3D(splitContainerCollision.Panel2, true);
            D3D.InitManagedDirect3D(splitContainerTiles.Panel1, true);
            TM.InitManagedTextureManager(ManagedDirect3D.Instance.Device, ManagedDirect3D.Instance.Sprite);

            //Initialize the workspace
            m_szWorkSpace = string.Empty;
            m_szTileSet = string.Empty;

            FolderBrowserDialog dlg = new FolderBrowserDialog();

            if (DialogResult.OK == dlg.ShowDialog())
                m_szWorkSpace = dlg.SelectedPath + "\\";

            //Initialize the tileset
            m_szTileSet = "default.bmp";
            m_sTileSelection.m_nImageID = LoadImage(m_sTileSelection.m_nImageID, m_szTileSet);

            AutoscrollAllWindows();

            SetupComplete = true;
        }


        public void Render()
        {
            switch (tabControlMain.SelectedIndex)
            {
                case 0: //Primary Tab
                    {
                        D3D.Clear(splitContainerMap.Panel2, 240, 240, 240);

                        D3D.DeviceBegin();
                        D3D.SpriteBegin();

                        Point Offset = Point.Empty;
                        Offset.X = splitContainerMap.Panel2.AutoScrollPosition.X;
                        Offset.Y = splitContainerMap.Panel2.AutoScrollPosition.Y;

                        for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                            for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                            {
                                int Index = Row * m_gTileMap.m_nColumns + Col;

                                int PosX = Col * m_gTileMap.m_nWidth + Offset.X;
                                int PosY = Row * m_gTileMap.m_nHeight + Offset.Y;

                                //m_lMap[Index].m_nDrawImage;
                                //y = i/width
                                //x = i%width

                                int ImageRow = m_lMap[Index].m_nDrawImage / m_sTileSelection.m_nColumns;
                                int ImageCol = m_lMap[Index].m_nDrawImage % m_sTileSelection.m_nColumns;

                                if (PosX > splitContainerMap.Panel2.Width || PosX + m_sTileSelection.m_nWidth < 0)
                                    continue;

                                if (PosY > splitContainerMap.Panel2.Height || PosY + m_sTileSelection.m_nHeight < 0)
                                    continue;

                                if ((ImageRow == (m_sTileSelection.m_nRows - 1)) && (ImageCol == (m_sTileSelection.m_nColumns - 1)))
                                {
                                    D3D.DrawText("NULL", PosX, PosY, 0, 0, 0);
                                    continue;
                                }

                                Rectangle rDrawRect = new Rectangle
                                    (
                                    ImageCol * m_sTileSelection.m_nWidth, ImageRow * m_sTileSelection.m_nHeight,
                                    m_sTileSelection.m_nWidth, m_sTileSelection.m_nHeight
                                    );

                                TM.Draw(m_sTileSelection.m_nImageID, PosX, PosY, ScaleX, ScaleY, rDrawRect, 0, 0, 0, 0);
                            }

                        D3D.Sprite.Flush();	// DRAW ALL SPRITES NOW!!!

                        for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                            for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                            {
                                int Index = Row * m_gTileMap.m_nColumns + Col;

                                int PosX = Col * m_gTileMap.m_nWidth + Offset.X;
                                int PosY = Row * m_gTileMap.m_nHeight + Offset.Y;

                                if (PosX > splitContainerMap.Panel2.Width || PosX + m_sTileSelection.m_nWidth < 0)
                                    continue;

                                if (PosY > splitContainerMap.Panel2.Height || PosY + m_sTileSelection.m_nHeight < 0)
                                    continue;

                                if (m_lMap[Index].m_bEnemy)
                                {
                                    switch (m_lMap[Index].m_eEnemy.m_nType)
                                    {
                                        case (int)Enemies.Turret_Gun:
                                            D3D.DrawText("E-GT", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Turret_Frost:
                                            D3D.DrawText("E-FT", PosX, PosY, 0, 0, 255);
                                            break;
                                        case (int)Enemies.Turret_Fire:
                                            D3D.DrawText("E-FT", PosX, PosY, 255, 0, 0);
                                            break;
                                        case (int)Enemies.Turret_Multi:
                                            D3D.DrawText("E-MT", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Attack:
                                            D3D.DrawText("E-AD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Seeker:
                                            D3D.DrawText("E-SD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Heavy:
                                            D3D.DrawText("E-HD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_Mech:
                                            D3D.DrawText("E-M", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_Siege:
                                            D3D.DrawText("E-SW", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_FLCL:
                                            D3D.DrawText("E-FM", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Boss_Apple:
                                            D3D.DrawText("E-AM", PosX, PosY, 155, 0, 255);
                                            break;
                                        case (int)Enemies.Boss_Pimp:
                                            D3D.DrawText("E-PS", PosX, PosY, 155, 0, 255);
                                            break;
                                        case (int)Enemies.Boss_Pirate:
                                            D3D.DrawText("E-DP", PosX, PosY, 155, 0, 255);
                                            break;
                                    }
                                }
                                if (m_lMap[Index].m_bCollision)
                                {
                                    switch (m_lMap[Index].m_cCollision.m_nType)
                                    {
                                        case (int)Collisions.Coll_Solid:
                                            D3D.DrawText("C-S", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Partial:
                                            D3D.DrawText("C-PS", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Moving:
                                            D3D.DrawText("C-M", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Unstable:
                                            D3D.DrawText("C-U", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Trap:
                                            D3D.DrawText("C-T", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                    }
                                }
                                if (m_lMap[Index].m_bSpawner)
                                {
                                    switch (m_lMap[Index].m_sSpawner.m_nType)
                                    {
                                        case (int)Spawners.Turret_Spawner:
                                            D3D.DrawText("SP-T", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Drone_Spawner:
                                            D3D.DrawText("SP-A", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Ground_Spawner:
                                            D3D.DrawText("SP-G", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Boss_Spawner:
                                            D3D.DrawText("SG-B", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                    }
                                }
                            }

                        //Creates the horizontal line segments
                        for (int Row = 0; Row <= m_gTileMap.m_nRows; Row++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = 0 * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Right = m_gTileMap.m_nColumns * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = Row * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Top < 0 || rDrawRect.Top > splitContainerMap.Panel2.Height)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Right, rDrawRect.Top, 0, 0, 0);
                        }
                       
                        //Creates the vertical line segments
                        for (int Col = 0; Col <= m_gTileMap.m_nColumns; Col++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = Col * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = 0 * m_gTileMap.m_nHeight + Offset.Y;
                            rDrawRect.Bottom = m_gTileMap.m_nRows * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Left < 0 || rDrawRect.Left > splitContainerMap.Panel2.Width)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Left, rDrawRect.Bottom, 0, 0, 0);
                        }
                    


                        D3D.SpriteEnd();
                        D3D.DeviceEnd();

                        D3D.Present(splitContainerMap.Panel2);
                    }
                    break;

                case 1: //Background Tab
                    {
                        D3D.Clear(splitContainerBackground.Panel2, 240, 240, 240);

                        D3D.DeviceBegin();
                        D3D.SpriteBegin();

                        Point Offset = Point.Empty;
                        Offset.X = splitContainerBackground.Panel2.AutoScrollPosition.X;
                        Offset.Y = splitContainerBackground.Panel2.AutoScrollPosition.Y;

                        for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                            for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                            {
                                int Index = Row * m_gTileMap.m_nColumns + Col;

                                int PosX = Col * m_gTileMap.m_nWidth + Offset.X;
                                int PosY = Row * m_gTileMap.m_nHeight + Offset.Y;

                                //m_lMap[Index].m_nDrawImage;
                                //y = i/width
                                //x = i%width

                                int ImageRow = m_lMap[Index].m_nDrawImage / m_sTileSelection.m_nColumns;
                                int ImageCol = m_lMap[Index].m_nDrawImage % m_sTileSelection.m_nColumns;

                                if (PosX > splitContainerBackground.Panel2.Width || PosX + m_sTileSelection.m_nWidth < 0)
                                    continue;

                                if (PosY > splitContainerBackground.Panel2.Height || PosY + m_sTileSelection.m_nHeight < 0)
                                    continue;

                                if ((ImageRow == (m_sTileSelection.m_nRows - 1)) && (ImageCol == (m_sTileSelection.m_nColumns - 1)))
                                {
                                    D3D.DrawText("NULL", PosX, PosY, 0, 0, 0);
                                    continue;
                                }

                                Rectangle rDrawRect = new Rectangle
                                    (
                                    ImageCol * m_sTileSelection.m_nWidth, ImageRow * m_sTileSelection.m_nHeight,
                                    m_sTileSelection.m_nWidth, m_sTileSelection.m_nHeight
                                    );

                                TM.Draw(m_sTileSelection.m_nImageID, PosX, PosY, ScaleX, ScaleY, rDrawRect, 0, 0, 0, 0);
                            }

                        D3D.Sprite.Flush();	// DRAW ALL SPRITES NOW!!!


                        //Creates the horizontal line segments
                        for (int Row = 0; Row <= m_gTileMap.m_nRows; Row++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = 0 * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Right = m_gTileMap.m_nColumns * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = Row * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Top < 0 || rDrawRect.Top > splitContainerBackground.Panel2.Height)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Right, rDrawRect.Top, 0, 0, 0);
                        }

                        //Creates the vertical line segments
                        for (int Col = 0; Col <= m_gTileMap.m_nColumns; Col++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = Col * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = 0 * m_gTileMap.m_nHeight + Offset.Y;
                            rDrawRect.Bottom = m_gTileMap.m_nRows * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Left < 0 || rDrawRect.Left > splitContainerBackground.Panel2.Width)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Left, rDrawRect.Bottom, 0, 0, 0);
                        }


                        D3D.SpriteEnd();
                        D3D.DeviceEnd();

                        D3D.Present(splitContainerBackground.Panel2);


                        //Tileset draw call
                        D3D.Clear(splitContainerTiles.Panel1, 240, 240, 240);

                        D3D.DeviceBegin();
                        D3D.SpriteBegin();

                        Offset.X = splitContainerTiles.Panel1.AutoScrollPosition.X;
                        Offset.Y = splitContainerTiles.Panel1.AutoScrollPosition.Y;

                        TM.Draw(m_sTileSelection.m_nImageID, 0 + Offset.X, 0 + Offset.Y, 1, 1, Rectangle.Empty, 0, 0, 0, 0);

                        D3D.Sprite.Flush();	// DRAW ALL SPRITES NOW!!!


                        //Creates the horizontal line segments
                        for (int Row = 0; Row <= m_sTileSelection.m_nRows; Row++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = 0 * m_sTileSelection.m_nWidth + Offset.X;
                            rDrawRect.Right = m_sTileSelection.m_nColumns * m_sTileSelection.m_nWidth + Offset.X;
                            rDrawRect.Top = Row * m_sTileSelection.m_nHeight + Offset.Y;

                            if (rDrawRect.Top < 0 || rDrawRect.Top > splitContainerTiles.Panel1.Height)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Right, rDrawRect.Top, 0, 0, 0);
                        }

                        //Creates the vertical line segments
                        for (int Col = 0; Col <= m_sTileSelection.m_nColumns; Col++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = Col * m_sTileSelection.m_nWidth + Offset.X;
                            rDrawRect.Top = 0 * m_sTileSelection.m_nHeight + Offset.Y;
                            rDrawRect.Bottom = m_sTileSelection.m_nRows * m_sTileSelection.m_nHeight + Offset.Y;

                            if (rDrawRect.Left < 0 || rDrawRect.Left > splitContainerTiles.Panel1.Width)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Left, rDrawRect.Bottom, 0, 0, 0);
                        }



                        //Draws a red box around the selected tile
                        {
                            int ImageTileRow = m_sTileSelection.m_nSelectedIndex / m_sTileSelection.m_nColumns;
                            int ImageTileCol = m_sTileSelection.m_nSelectedIndex % m_sTileSelection.m_nColumns;

                            RECT rDrawSelection = new RECT();
                            rDrawSelection.Left = ImageTileCol * m_sTileSelection.m_nWidth + Offset.X;
                            rDrawSelection.Top = ImageTileRow * m_sTileSelection.m_nHeight + Offset.Y;
                            rDrawSelection.Right = rDrawSelection.Left + m_sTileSelection.m_nWidth;
                            rDrawSelection.Bottom = rDrawSelection.Top + m_sTileSelection.m_nHeight;

                            D3D.DrawLine(rDrawSelection.Left, rDrawSelection.Top, rDrawSelection.Left, rDrawSelection.Bottom, 255, 0, 0);
                            D3D.DrawLine(rDrawSelection.Right, rDrawSelection.Top, rDrawSelection.Right, rDrawSelection.Bottom, 255, 0, 0);
                            D3D.DrawLine(rDrawSelection.Left, rDrawSelection.Top, rDrawSelection.Right, rDrawSelection.Top, 255, 0, 0);
                            D3D.DrawLine(rDrawSelection.Left, rDrawSelection.Bottom, rDrawSelection.Right, rDrawSelection.Bottom, 255, 0, 0);
                        }

                        D3D.SpriteEnd();
                        D3D.DeviceEnd();

                        D3D.Present(splitContainerTiles.Panel1);
                    }
                    break;

                case 2: //Collision Tab
                    {
                        D3D.Clear(splitContainerCollision.Panel2, 240, 240, 240);

                        D3D.DeviceBegin();
                        D3D.SpriteBegin();

                        Point Offset = Point.Empty;
                        Offset.X = splitContainerCollision.Panel2.AutoScrollPosition.X;
                        Offset.Y = splitContainerCollision.Panel2.AutoScrollPosition.Y;

                        for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                            for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                            {
                                int Index = Row * m_gTileMap.m_nColumns + Col;

                                int PosX = Col * m_gTileMap.m_nWidth + Offset.X;
                                int PosY = Row * m_gTileMap.m_nHeight + Offset.Y;

                                //m_lMap[Index].m_nDrawImage;
                                //y = i/width
                                //x = i%width

                                int ImageRow = m_lMap[Index].m_nDrawImage / m_sTileSelection.m_nColumns;
                                int ImageCol = m_lMap[Index].m_nDrawImage % m_sTileSelection.m_nColumns;

                                if (PosX > splitContainerCollision.Panel2.Width || PosX + m_sTileSelection.m_nWidth < 0)
                                    continue;

                                if (PosY > splitContainerCollision.Panel2.Height || PosY + m_sTileSelection.m_nHeight < 0)
                                    continue;

                                if ((ImageRow == (m_sTileSelection.m_nRows - 1)) && (ImageCol == (m_sTileSelection.m_nColumns - 1)))
                                {
                                    D3D.DrawText("NULL", PosX, PosY, 0, 0, 0);
                                    continue;
                                }

                                Rectangle rDrawRect = new Rectangle
                                    (
                                    ImageCol * m_sTileSelection.m_nWidth, ImageRow * m_sTileSelection.m_nHeight,
                                    m_sTileSelection.m_nWidth, m_sTileSelection.m_nHeight
                                    );

                                TM.Draw(m_sTileSelection.m_nImageID, PosX, PosY, ScaleX, ScaleY, rDrawRect, 0, 0, 0, Color.FromArgb(180, 255, 255, 255).ToArgb());
                            }

                        D3D.Sprite.Flush();	// DRAW ALL SPRITES NOW!!!

                        for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                            for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                            {
                                int Index = Row * m_gTileMap.m_nColumns + Col;

                                int PosX = Col * m_gTileMap.m_nWidth + Offset.X;
                                int PosY = Row * m_gTileMap.m_nHeight + Offset.Y;

                                if (PosX > splitContainerCollision.Panel2.Width || PosX + m_sTileSelection.m_nWidth < 0)
                                    continue;

                                if (PosY > splitContainerCollision.Panel2.Height || PosY + m_sTileSelection.m_nHeight < 0)
                                    continue;

                                if (m_lMap[Index].m_bEnemy)
                                {
                                    switch (m_lMap[Index].m_eEnemy.m_nType)
                                    {
                                        case (int)Enemies.Turret_Gun:
                                            D3D.DrawText("E-GT", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Turret_Frost:
                                            D3D.DrawText("E-FT", PosX, PosY, 0, 0, 255);
                                            break;
                                        case (int)Enemies.Turret_Fire:
                                            D3D.DrawText("E-FT", PosX, PosY, 255, 0, 0);
                                            break;
                                        case (int)Enemies.Turret_Multi:
                                            D3D.DrawText("E-MT", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Attack:
                                            D3D.DrawText("E-AD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Seeker:
                                            D3D.DrawText("E-SD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Drone_Heavy:
                                            D3D.DrawText("E-HD", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_Mech:
                                            D3D.DrawText("E-M", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_Siege:
                                            D3D.DrawText("E-SW", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Ground_FLCL:
                                            D3D.DrawText("E-FM", PosX, PosY, 0, 0, 0);
                                            break;
                                        case (int)Enemies.Boss_Apple:
                                            D3D.DrawText("E-AM", PosX, PosY, 155, 0, 255);
                                            break;
                                        case (int)Enemies.Boss_Pimp:
                                            D3D.DrawText("E-PS", PosX, PosY, 155, 0, 255);
                                            break;
                                        case (int)Enemies.Boss_Pirate:
                                            D3D.DrawText("E-DP", PosX, PosY, 155, 0, 255);
                                            break;
                                    }
                                }
                                if (m_lMap[Index].m_bCollision)
                                {
                                    switch (m_lMap[Index].m_cCollision.m_nType)
                                    {
                                        case (int)Collisions.Coll_Solid:
                                            D3D.DrawText("C-S", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Partial:
                                            D3D.DrawText("C-PS", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Moving:
                                            D3D.DrawText("C-M", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Unstable:
                                            D3D.DrawText("C-U", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                        case (int)Collisions.Coll_Trap:
                                            D3D.DrawText("C-T", PosX, PosY + m_gTileMap.m_nHeight / 2 - (m_gTileMap.m_nHeight / 6), 0, 0, 0);
                                            break;
                                    }
                                }
                                if (m_lMap[Index].m_bSpawner)
                                {
                                    switch (m_lMap[Index].m_sSpawner.m_nType)
                                    {
                                        case (int)Spawners.Turret_Spawner:
                                            D3D.DrawText("SP-T", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Drone_Spawner:
                                            D3D.DrawText("SP-A", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Ground_Spawner:
                                            D3D.DrawText("SP-G", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                        case (int)Spawners.Boss_Spawner:
                                            D3D.DrawText("SG-B", PosX, PosY + m_gTileMap.m_nHeight / 2 + (m_gTileMap.m_nHeight / 4), 0, 0, 0);
                                            break;
                                    }
                                }
                               }


                        //Creates the horizontal line segments
                        for (int Row = 0; Row <= m_gTileMap.m_nRows; Row++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = 0 * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Right = m_gTileMap.m_nColumns * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = Row * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Top < 0 || rDrawRect.Top > splitContainerCollision.Panel2.Height)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Right, rDrawRect.Top, 0, 0, 0);
                        }

                        //Creates the vertical line segments
                        for (int Col = 0; Col <= m_gTileMap.m_nColumns; Col++)
                        {
                            RECT rDrawRect = new RECT();
                            rDrawRect.Left = Col * m_gTileMap.m_nWidth + Offset.X;
                            rDrawRect.Top = 0 * m_gTileMap.m_nHeight + Offset.Y;
                            rDrawRect.Bottom = m_gTileMap.m_nRows * m_gTileMap.m_nHeight + Offset.Y;

                            if (rDrawRect.Left < 0 || rDrawRect.Left > splitContainerCollision.Panel2.Width)
                                continue;

                            D3D.DrawLine(rDrawRect.Left, rDrawRect.Top, rDrawRect.Left, rDrawRect.Bottom, 0, 0, 0);
                        }

                        D3D.SpriteEnd();
                        D3D.DeviceEnd();

                        D3D.Present(splitContainerCollision.Panel2);
                    }
                    break;

                case 3: //Google, no render necessary.
                    break;
                    
                default:
                    break;
            }
        }


        private void Map_Editor_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
        }

        private void numericGridCol_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            int Cols = m_gTileMap.m_nColumns;
            int Rows = m_gTileMap.m_nRows;

            m_gTileMap.m_nColumns = Convert.ToInt32(numericGridCol.Value);

            splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);

            ReCreateList(Rows, Cols, m_gTileMap.m_nRows, m_gTileMap.m_nColumns);
        }

        private void numericGridRow_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            int Cols = m_gTileMap.m_nColumns;
            int Rows = m_gTileMap.m_nRows;

            m_gTileMap.m_nRows = Convert.ToInt32(numericGridRow.Value);
            splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);

            ReCreateList(Rows, Cols, m_gTileMap.m_nRows, m_gTileMap.m_nColumns);
        }

        private void numericGridTileWidth_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            m_gTileMap.m_nWidth = Convert.ToInt32(numericGridTileWidth.Value);
            m_sScale.m_fGridWidth = (float)numericGridTileWidth.Value;

            splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);

            ScaleX = m_sScale.m_fGridWidth / m_sScale.m_fSelectionWidth;
            ScaleY = m_sScale.m_fGridHeight / m_sScale.m_fSelectionHeight;
        }

        private void numericGridTileHeight_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            m_gTileMap.m_nHeight = Convert.ToInt32(numericGridTileHeight.Value);
            m_sScale.m_fGridHeight = (float)numericGridTileHeight.Value;

            splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
            splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);

            ScaleX = m_sScale.m_fGridWidth / m_sScale.m_fSelectionWidth;
            ScaleY = m_sScale.m_fGridHeight / m_sScale.m_fSelectionHeight;
        }

        private void numericSourceCol_ValueChanged(object sender, EventArgs e)
        {
            m_sTileSelection.m_nColumns = Convert.ToInt32(numericSourceCol.Value);
            splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);
        }

        private void numericSourceRow_ValueChanged(object sender, EventArgs e)
        {
            m_sTileSelection.m_nRows = Convert.ToInt32(numericSourceRow.Value);
            splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);
        }

        private void numericSourceTileWidth_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            m_sTileSelection.m_nWidth = Convert.ToInt32(numericSourceTileWidth.Value);
            m_sScale.m_fSelectionWidth = (float)numericSourceTileWidth.Value;

            splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);

            ScaleX = m_sScale.m_fGridWidth / m_sScale.m_fSelectionWidth;
            ScaleY = m_sScale.m_fGridHeight / m_sScale.m_fSelectionHeight;
        }

        private void numericSourceTileHeight_ValueChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            m_sTileSelection.m_nHeight = Convert.ToInt32(numericSourceTileHeight.Value);
            m_sScale.m_fSelectionHeight = (float)numericSourceTileHeight.Value;

            splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);

            ScaleX = m_sScale.m_fGridWidth / m_sScale.m_fSelectionWidth;
            ScaleY = m_sScale.m_fGridHeight / m_sScale.m_fSelectionHeight;
        }

        private void splitContainerMap_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            Render();
        }

        private void splitContainerTiles_Panel1_Scroll(object sender, ScrollEventArgs e)
        {
            Render();
        }

        private void splitContainerBackground_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            Render();
        }

        private void splitContainerCollision_Panel2_Scroll(object sender, ScrollEventArgs e)
        {
            Render();
        }

        private void CreateList(int Rows, int Cols)
        {
            int ListSize = Rows * Cols;

            m_lMap.RemoveRange(0, m_lMap.Count);

            for (int i = 0; i < ListSize; i++)
            {
                ListObject NewObject = new ListObject(false, false, false, 0);
                m_lMap.Add(NewObject);
            }
        }
        
        private void ReCreateList(int OldRows, int OldCols, int NewRows, int NewCols)
        {
            m_lTempMap = null;
            m_lTempMap = new List<ListObject>();

            int ListSize = NewRows * NewCols;
            for (int i = 0; i < ListSize; i++)
            {
                ListObject NewObject = new ListObject(false, false, false, 0);
                m_lTempMap.Add(NewObject);
            }


            for(int Rows = 0; Rows < OldRows; Rows++)
                for (int Cols = 0; Cols < OldCols; Cols++)
                {
                    int FirstIndex = Rows * OldCols + Cols;

                    if (Cols < NewCols && Rows < NewRows)
                    {
                        int SecondIndex = Rows * NewCols + Cols;

                        m_lTempMap[SecondIndex] = m_lMap[FirstIndex];
                    }
                }

            CreateList(NewRows, NewCols);
            m_lMap = m_lTempMap;
        }

        private int LoadImage(int nImageID, string szFileName)
        {
            if (nImageID != -1)
                TM.ReleaseTexture(nImageID);

            string szFullPath = string.Empty;
            szFullPath = m_szWorkSpace + szFileName;

            return TM.LoadTexture(szFullPath, 0);
        }

        private void splitContainerTiles_Panel1_MouseClick(object sender, MouseEventArgs e)
        {
            //Handles Mouse Selection and Input
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerTiles.Panel1.AutoScrollPosition.X;
            Offset.Y = splitContainerTiles.Panel1.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            toolStripStatusXValue.Text = Mouse.X.ToString();
            toolStripStatusYValue.Text = Mouse.Y.ToString();
            
            for (int Row = 0; Row < m_sTileSelection.m_nRows; Row++)
                for (int Col = 0; Col < m_sTileSelection.m_nColumns; Col++)
                {
                    int Index = Row * m_sTileSelection.m_nColumns + Col;

                    RECT rDrawSelection = new RECT();
                    rDrawSelection.Left = Col * m_sTileSelection.m_nWidth;
                    rDrawSelection.Top = Row * m_sTileSelection.m_nHeight;
                    rDrawSelection.Right = rDrawSelection.Left + m_sTileSelection.m_nWidth;
                    rDrawSelection.Bottom = rDrawSelection.Top + m_sTileSelection.m_nHeight;

                    if ((Mouse.X >= rDrawSelection.Left && Mouse.X <= rDrawSelection.Right)
                      && (Mouse.Y >= rDrawSelection.Top && Mouse.Y <= rDrawSelection.Bottom))
                    {
                        m_sTileSelection.m_nSelectedIndex = Index;
                        return;
                    }
                }

        }

        private void splitContainerBackground_Panel2_MouseClick(object sender, MouseEventArgs e)
        {
            //Handles Mouse Selection and Input
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerBackground.Panel2.AutoScrollPosition.X;
            Offset.Y = splitContainerBackground.Panel2.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                {
                    int Index = Row * m_gTileMap.m_nColumns + Col;

                    RECT rDrawSelection = new RECT();
                    rDrawSelection.Left = Col * m_gTileMap.m_nWidth;
                    rDrawSelection.Top = Row * m_gTileMap.m_nHeight;
                    rDrawSelection.Right = rDrawSelection.Left + m_gTileMap.m_nWidth;
                    rDrawSelection.Bottom = rDrawSelection.Top + m_gTileMap.m_nHeight;

                    if ((Mouse.X >= rDrawSelection.Left && Mouse.X <= rDrawSelection.Right)
                      && (Mouse.Y >= rDrawSelection.Top && Mouse.Y <= rDrawSelection.Bottom))
                    {
                        ListObject NewNode = new ListObject();
                        NewNode = m_lMap[Index];
                        NewNode.m_nDrawImage = m_sTileSelection.m_nSelectedIndex;
                        m_lMap[Index] = NewNode;
                        return;
                    }
                }
        }

        private void splitContainerCollision_Panel2_MouseClick(object sender, MouseEventArgs e)
        {
            //Handles Mouse Selection and Input
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerCollision.Panel2.AutoScrollPosition.X;
            Offset.Y = splitContainerCollision.Panel2.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                {
                    int Index = Row * m_gTileMap.m_nColumns + Col;

                    RECT rDrawSelection = new RECT();
                    rDrawSelection.Left = Col * m_gTileMap.m_nWidth;
                    rDrawSelection.Top = Row * m_gTileMap.m_nHeight;
                    rDrawSelection.Right = rDrawSelection.Left + m_gTileMap.m_nWidth;
                    rDrawSelection.Bottom = rDrawSelection.Top + m_gTileMap.m_nHeight;

                    if ((Mouse.X >= rDrawSelection.Left && Mouse.X <= rDrawSelection.Right)
                      && (Mouse.Y >= rDrawSelection.Top && Mouse.Y <= rDrawSelection.Bottom))
                    {
                        ListObject NewNode = new ListObject();
                        NewNode = m_lMap[Index];

                        if (e.Button == MouseButtons.Left)
                        {
                            if (CollisionSelection == 0)
                            {
                                NewNode.m_bEnemy = true;
                                NewNode.m_bCollision = false;
                                NewNode.m_eEnemy = new Enemy(comboEnemy.SelectedIndex, rDrawSelection.Left, rDrawSelection.Top);
                            }
                            else if (CollisionSelection == 1)
                            {
                                NewNode.m_bCollision = true;
                                NewNode.m_bEnemy = false ;
                                NewNode.m_cCollision = new Collision(comboCollision.SelectedIndex, rDrawSelection.Left, rDrawSelection.Top, rDrawSelection.Right, rDrawSelection.Bottom);
                            }
                            else if (CollisionSelection == 2)
                            {
                                NewNode.m_bSpawner = true;
                                NewNode.m_sSpawner = new Spawner(comboSpawn.SelectedIndex, rDrawSelection.Left, rDrawSelection.Top);
                            }
                        }
                        else if (e.Button == MouseButtons.Right)
                        {
                            if (CollisionSelection == 0)
                            {
                                NewNode.m_bEnemy = false;
                            }
                            else if (CollisionSelection == 1)
                            {
                                NewNode.m_bCollision = false;
                            }
                            else if (CollisionSelection == 2)
                            {
                                NewNode.m_bSpawner = false;
                            }
                        }

                        m_lMap[Index] = NewNode;
                        return;
                    }
                }
        }

        private void splitContainerBackground_Panel2_MouseDown(object sender, MouseEventArgs e)
        {
            Drawing = true;
            splitContainerBackground_Panel2_MouseClick(sender, e);
        }

        private void splitContainerBackground_Panel2_MouseMove(object sender, MouseEventArgs e)
        {
            if (Drawing)
            {
                splitContainerBackground_Panel2_MouseClick(sender, e);
            }
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerBackground.Panel2.AutoScrollPosition.X;
            Offset.Y = splitContainerBackground.Panel2.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            toolStripStatusXValue.Text = Mouse.X.ToString();
            toolStripStatusYValue.Text = Mouse.Y.ToString();
        }

        private void splitContainerBackground_Panel2_MouseUp(object sender, MouseEventArgs e)
        {
            Drawing = false;
        }

        private void splitContainerMap_Panel2_MouseMove(object sender, MouseEventArgs e)
        {
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerMap.Panel2.AutoScrollPosition.X;
            Offset.Y = splitContainerMap.Panel2.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            toolStripStatusXValue.Text = Mouse.X.ToString();
            toolStripStatusYValue.Text = Mouse.Y.ToString();
        }

        private void splitContainerCollision_Panel2_MouseMove(object sender, MouseEventArgs e)
        {
            Point Offset = Point.Empty;
            Point Mouse = Point.Empty;

            Offset.X = splitContainerCollision.Panel2.AutoScrollPosition.X;
            Offset.Y = splitContainerCollision.Panel2.AutoScrollPosition.Y;

            Mouse.X = e.Location.X - Offset.X;
            Mouse.Y = e.Location.Y - Offset.Y;

            toolStripStatusXValue.Text = Mouse.X.ToString();
            toolStripStatusYValue.Text = Mouse.Y.ToString();
        }

        private void splitContainerBackground_Panel2_ClientSizeChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            AutoscrollAllWindows();
        }

        private void splitContainerMap_Panel2_ClientSizeChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            AutoscrollAllWindows();
        }

        private void splitContainerTiles_Panel1_ClientSizeChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            AutoscrollAllWindows();
        }

        private void splitContainerCollision_Panel2_ClientSizeChanged(object sender, EventArgs e)
        {
            if (!SetupComplete)
                return;

            AutoscrollAllWindows();
        }



        private void AutoscrollAllWindows()
        {
            //Layer 1
            if (splitContainerMap.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns && splitContainerMap.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerMap.Panel2, true);
            else if (splitContainerMap.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns)
                D3D.ChangeDisplayParam(splitContainerMap.Panel2, splitContainerMap.Panel2.Width - 15, splitContainerMap.Panel2.Height, true);
            else if (splitContainerMap.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerMap.Panel2, splitContainerMap.Panel2.Width, splitContainerMap.Panel2.Height - 15, true);
            else
                D3D.ChangeDisplayParam(splitContainerMap.Panel2, splitContainerMap.Panel2.Width - 15, splitContainerMap.Panel2.Height - 15, true);

            //Layer 2
            if (splitContainerBackground.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns && splitContainerBackground.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerBackground.Panel2, true);
            else if (splitContainerBackground.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns)
                D3D.ChangeDisplayParam(splitContainerBackground.Panel2, splitContainerBackground.Panel2.Width - 15, splitContainerBackground.Panel2.Height, true);
            else if (splitContainerBackground.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerBackground.Panel2, splitContainerBackground.Panel2.Width, splitContainerBackground.Panel2.Height - 15, true);
            else
                D3D.ChangeDisplayParam(splitContainerBackground.Panel2, splitContainerBackground.Panel2.Width - 15, splitContainerBackground.Panel2.Height - 15, true);

            //Layer 2a
            if (splitContainerTiles.Panel1.Width >= m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns && splitContainerTiles.Panel1.Height >= m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows)
                D3D.ChangeDisplayParam(splitContainerTiles.Panel1, true);
            else if (splitContainerBackground.Panel1.Width >= m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns)
                D3D.ChangeDisplayParam(splitContainerTiles.Panel1, splitContainerTiles.Panel1.Width - 15, splitContainerTiles.Panel1.Height, true);
            else if (splitContainerBackground.Panel1.Height >= m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows)
                D3D.ChangeDisplayParam(splitContainerTiles.Panel1, splitContainerTiles.Panel1.Width, splitContainerTiles.Panel1.Height - 15, true);
            else
                D3D.ChangeDisplayParam(splitContainerTiles.Panel1, splitContainerTiles.Panel1.Width - 15, splitContainerTiles.Panel1.Height - 15, true);

            //Layer 3
            if (splitContainerCollision.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns && splitContainerCollision.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerCollision.Panel2, true);
            else if (splitContainerCollision.Panel2.Width >= m_gTileMap.m_nWidth * m_gTileMap.m_nColumns)
                D3D.ChangeDisplayParam(splitContainerCollision.Panel2, splitContainerCollision.Panel2.Width - 15, splitContainerCollision.Panel2.Height, true);
            else if (splitContainerCollision.Panel2.Height >= m_gTileMap.m_nHeight * m_gTileMap.m_nRows)
                D3D.ChangeDisplayParam(splitContainerCollision.Panel2, splitContainerCollision.Panel2.Width, splitContainerCollision.Panel2.Height - 15, true);
            else
                D3D.ChangeDisplayParam(splitContainerCollision.Panel2, splitContainerCollision.Panel2.Width - 15, splitContainerCollision.Panel2.Height - 15, true);
            
        }

        private void buttonEnemy_Click(object sender, EventArgs e)
        {
            CollisionSelection = 0;
            buttonEnemy.BackColor = Color.Green;
            buttonSpawner.BackColor = Color.Red;
            buttonCollision.BackColor = Color.Red;
        }

        private void buttonCollision_Click(object sender, EventArgs e)
        {
            CollisionSelection = 1;
            buttonEnemy.BackColor = Color.Red;
            buttonSpawner.BackColor = Color.Red;
            buttonCollision.BackColor = Color.Green;
        }

        private void buttonSpawner_Click(object sender, EventArgs e)
        {
            CollisionSelection = 2;
            buttonEnemy.BackColor = Color.Red;
            buttonSpawner.BackColor = Color.Green;
            buttonCollision.BackColor = Color.Red;
        }

        private void buttonTileset_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "Portable Network Graphics (*.png)|*.png|Bitmap (*.bmp)|*.bmp";
            dlg.InitialDirectory = m_szWorkSpace;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                m_szTileSet = dlg.SafeFileName;
                m_sTileSelection.m_nImageID = LoadImage(m_sTileSelection.m_nImageID, m_szTileSet);
            }
        }

        private void buttonWork_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dlg = new FolderBrowserDialog();

            if (DialogResult.OK == dlg.ShowDialog())
                m_szWorkSpace = dlg.SelectedPath + "\\";
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                {
                    int Index = Row * m_gTileMap.m_nColumns + Col;

                    m_lMap[Index] = new ListObject();
                }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "CW Map File (*.CWM)|*.CWM";
            dlg.InitialDirectory = m_szWorkSpace;
            dlg.DefaultExt = "cwm";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                BinaryWriter bWrite = new BinaryWriter(File.Create(dlg.FileName));

                bWrite.Write(m_gTileMap.m_nColumns);
                bWrite.Write(m_gTileMap.m_nRows);
                bWrite.Write(m_gTileMap.m_nWidth);
                bWrite.Write(m_gTileMap.m_nHeight);
                bWrite.Write(m_sTileSelection.m_nColumns);
                bWrite.Write(m_sTileSelection.m_nRows);
                bWrite.Write(m_sTileSelection.m_nWidth);
                bWrite.Write(m_sTileSelection.m_nHeight);
                bWrite.Write(m_szTileSet);

                for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                    for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                    {
                        int Index = Row * m_gTileMap.m_nColumns + Col;

                        bWrite.Write(m_lMap[Index].m_nDrawImage);
                    }

                int nEnemyCount = 0;
                int nCollisionCount = 0;
                int nSpawnerCount = 0;

                for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                    for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                    {
                        int Index = Row * m_gTileMap.m_nColumns + Col;

                        if (m_lMap[Index].m_bEnemy)
                            nEnemyCount++;
                        if (m_lMap[Index].m_bSpawner)
                            nSpawnerCount++;
                        if (m_lMap[Index].m_bCollision)
                            nCollisionCount++;
                    }

                //Write out enemy count
                bWrite.Write(nEnemyCount);

                for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                    for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                    {
                        int Index = Row * m_gTileMap.m_nColumns + Col;

                        if (m_lMap[Index].m_bEnemy)
                        {
                            bWrite.Write(Index);
                            bWrite.Write(m_lMap[Index].m_eEnemy.m_nType);
                            bWrite.Write(m_lMap[Index].m_eEnemy.m_nPosX);
                            bWrite.Write(m_lMap[Index].m_eEnemy.m_nPosY);
                        }
                    }

                //Write out spawner count
                bWrite.Write(nSpawnerCount);

                for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                    for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                    {
                        int Index = Row * m_gTileMap.m_nColumns + Col;

                        if (m_lMap[Index].m_bSpawner)
                        {
                            bWrite.Write(Index);
                            bWrite.Write(m_lMap[Index].m_sSpawner.m_nType);
                            bWrite.Write(m_lMap[Index].m_sSpawner.m_nPosX);
                            bWrite.Write(m_lMap[Index].m_sSpawner.m_nPosY);
                        }
                    }

                //Write out collision count
                bWrite.Write(nCollisionCount);

                for (int Row = 0; Row < m_gTileMap.m_nRows; Row++)
                    for (int Col = 0; Col < m_gTileMap.m_nColumns; Col++)
                    {
                        int Index = Row * m_gTileMap.m_nColumns + Col;

                        if (m_lMap[Index].m_bCollision)
                        {
                            bWrite.Write(Index);
                            bWrite.Write(m_lMap[Index].m_cCollision.m_nType);
                            bWrite.Write(m_lMap[Index].m_cCollision.m_rWorldPos.Left);
                            bWrite.Write(m_lMap[Index].m_cCollision.m_rWorldPos.Top);
                            bWrite.Write(m_lMap[Index].m_cCollision.m_rWorldPos.Right);
                            bWrite.Write(m_lMap[Index].m_cCollision.m_rWorldPos.Bottom);
                        }
                    }

            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "CW Map File (*.CWM)|*.CWM";
            dlg.InitialDirectory = m_szWorkSpace;

            if (DialogResult.OK == dlg.ShowDialog())
            {
                BinaryReader bRead = new BinaryReader(File.Open(dlg.FileName, FileMode.Open));

                SetupComplete = false;
                numericGridCol.Value = m_gTileMap.m_nColumns = bRead.ReadInt32();
                numericGridRow.Value = m_gTileMap.m_nRows = bRead.ReadInt32();
                numericGridTileWidth.Value = m_gTileMap.m_nWidth = bRead.ReadInt32();
                numericGridTileHeight.Value = m_gTileMap.m_nHeight = bRead.ReadInt32();
                numericSourceCol.Value = m_sTileSelection.m_nColumns = bRead.ReadInt32();
                numericSourceRow.Value = m_sTileSelection.m_nRows = bRead.ReadInt32();
                numericSourceTileWidth.Value = m_sTileSelection.m_nWidth = bRead.ReadInt32();
                numericSourceTileHeight.Value = m_sTileSelection.m_nHeight = bRead.ReadInt32();

                CreateList(m_gTileMap.m_nRows, m_gTileMap.m_nColumns);

                splitContainerTiles.Panel1.AutoScrollMinSize = new Size(m_sTileSelection.m_nWidth * m_sTileSelection.m_nColumns, m_sTileSelection.m_nHeight * m_sTileSelection.m_nRows);
                splitContainerMap.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
                splitContainerBackground.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);
                splitContainerCollision.Panel2.AutoScrollMinSize = new Size(m_gTileMap.m_nWidth * m_gTileMap.m_nColumns, m_gTileMap.m_nHeight * m_gTileMap.m_nRows);

                m_sScale.m_fGridWidth = m_gTileMap.m_nWidth;
                m_sScale.m_fGridHeight = m_gTileMap.m_nHeight;
                m_sScale.m_fSelectionWidth = m_sTileSelection.m_nWidth;
                m_sScale.m_fSelectionHeight = m_sTileSelection.m_nHeight;

                ScaleX = m_sScale.m_fGridWidth / m_sScale.m_fSelectionWidth;
                ScaleY = m_sScale.m_fGridHeight / m_sScale.m_fSelectionHeight;

                SetupComplete = true;

                m_szTileSet = bRead.ReadString();

                m_sTileSelection.m_nImageID = LoadImage(m_sTileSelection.m_nImageID, m_szTileSet);
                AutoscrollAllWindows();


                int nEnemyCount;
                int nSpawnerCount;
                int nCollisionCount;

                for (int i = 0; i < m_gTileMap.m_nColumns * m_gTileMap.m_nRows; i++)
                {
                    m_lMap[i] = new ListObject(false, false, false, bRead.ReadInt32());
                }

                nEnemyCount = bRead.ReadInt32();

                for (int i = 0; i < nEnemyCount; i++)
                {
                    int Index = bRead.ReadInt32();

                    ListObject NewNode = new ListObject();
                    NewNode = m_lMap[Index];

                    NewNode.m_bEnemy = true;
                    NewNode.m_eEnemy = new Enemy(bRead.ReadInt32(), bRead.ReadInt32(), bRead.ReadInt32());

                    m_lMap[Index] = NewNode;
                }

                nSpawnerCount = bRead.ReadInt32();

                for (int i = 0; i < nSpawnerCount; i++)
                {
                    int Index = bRead.ReadInt32();

                    ListObject NewNode = new ListObject();
                    NewNode = m_lMap[Index];

                    NewNode.m_bSpawner = true;
                    NewNode.m_sSpawner = new Spawner(bRead.ReadInt32(), bRead.ReadInt32(), bRead.ReadInt32());

                    m_lMap[Index] = NewNode;
                }

                nCollisionCount = bRead.ReadInt32();

                for (int i = 0; i < nCollisionCount; i++)
                {
                    int Index = bRead.ReadInt32();

                    ListObject NewNode = new ListObject();
                    NewNode = m_lMap[Index];

                    NewNode.m_bCollision = true;
                    NewNode.m_cCollision = new Collision(bRead.ReadInt32(), bRead.ReadInt32(), bRead.ReadInt32(), bRead.ReadInt32(), bRead.ReadInt32());

                    m_lMap[Index] = NewNode;
                }
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Looping = false;
            this.Close();
        }



    }
}