using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Types
{
    public class RECT
    {
        public int Left;
        public int Top;
        public int Right;
        public int Bottom;

        public RECT()
        {
            Left = 0;
            Top = 0;
            Right = 0;
            Bottom = 0;
        }
    }

    enum Enemies { Turret_Gun = 0, Turret_Frost, Turret_Fire, Turret_Multi,
                   Drone_Attack, Drone_Seeker, Drone_Heavy,
                   Ground_Mech, Ground_Siege, Ground_FLCL,
                   Boss_Apple, Boss_Pimp, Boss_Pirate      
                 };

    enum Spawners { Turret_Spawner = 0,
                   Drone_Spawner,
                   Ground_Spawner,
                   Boss_Spawner
                 };

    enum Collisions { Coll_Solid = 0,
                      Coll_Partial,
                      Coll_Moving,
                      Coll_Unstable,
                      Coll_Trap
                    };

    public struct Scales
    {
        public float m_fGridWidth;
        public float m_fGridHeight;
        public float m_fSelectionWidth;
        public float m_fSelectionHeight;

        public Scales(float gWidth, float gHeight, float sWidth, float sHeight)
        {
            m_fGridWidth = gWidth;
            m_fGridHeight = gHeight;
            m_fSelectionWidth = sWidth;
            m_fSelectionHeight = sHeight;
        }
    }

    public struct Grid
    {
        public int m_nColumns;
        public int m_nRows;
        public int m_nWidth;
        public int m_nHeight;


        public Grid(int nColumns, int nRows, int nWidth, int nHeight)
        {
            m_nColumns = nColumns;
            m_nRows = nRows;
            m_nWidth = nWidth;
            m_nHeight = nHeight;
        }
    };

    public struct Selection
    {
        public int m_nColumns;
        public int m_nRows;
        public int m_nWidth;
        public int m_nHeight;
        public int m_nSelectedIndex;
        public int m_nImageID;


        public Selection(int nColumns, int nRows, int nWidth, int nHeight)
        {
            m_nColumns = nColumns;
            m_nRows = nRows;
            m_nWidth = nWidth;
            m_nHeight = nHeight;
            m_nImageID = -1;
            m_nSelectedIndex = 0;
        }
    };

    public struct Enemy
    {
        public int m_nType;
        public int m_nPosX;
        public int m_nPosY;


        public Enemy(int nType, int nPosX, int nPosY)
        {
            m_nType = nType;
            m_nPosX = nPosX;
            m_nPosY = nPosY;
        }
    };

    public struct Spawner
    {
        public int m_nType;
        public int m_nPosX;
        public int m_nPosY;


        public Spawner(int nType, int nPosX, int nPosY)
        {
            m_nType = nType;
            m_nPosX = nPosX;
            m_nPosY = nPosY;
        }
    };

    public struct Collision
    {
        public int m_nType;
        public RECT m_rWorldPos;


        public Collision(int nType, int nLeft, int nTop, int nRight, int nBottom)
        {
            m_nType = nType;
            m_rWorldPos = new RECT();
            m_rWorldPos.Left = nLeft;
            m_rWorldPos.Top = nTop;
            m_rWorldPos.Right = nRight;
            m_rWorldPos.Bottom = nBottom;
        }
    };

    public struct ListObject
    {
        public Enemy m_eEnemy;
        public Spawner m_sSpawner;
        public Collision m_cCollision;
        public bool m_bEnemy;
        public bool m_bSpawner;
        public bool m_bCollision;
        public int m_nDrawImage;

        public ListObject(bool Enemy, bool Spawner, bool Collision, int DrawImage)
        {
            m_eEnemy = new Enemy();
            m_sSpawner = new Spawner();
            m_cCollision = new Collision();
            m_bEnemy = Enemy;
            m_bSpawner = Spawner;
            m_bCollision = Collision;
            m_nDrawImage = DrawImage;
        }
    }
}