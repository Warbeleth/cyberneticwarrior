namespace SGP_Map_Editor
{
    partial class Map_Editor
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusX = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusXValue = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusY = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusYValue = new System.Windows.Forms.ToolStripStatusLabel();
            this.tabControlMain = new System.Windows.Forms.TabControl();
            this.tabPageMap = new System.Windows.Forms.TabPage();
            this.splitContainerMap = new System.Windows.Forms.SplitContainer();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.numericGridRow = new System.Windows.Forms.NumericUpDown();
            this.numericGridCol = new System.Windows.Forms.NumericUpDown();
            this.labelRow = new System.Windows.Forms.Label();
            this.labelCol = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.numericGridTileHeight = new System.Windows.Forms.NumericUpDown();
            this.numericGridTileWidth = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.numericSourceTileHeight = new System.Windows.Forms.NumericUpDown();
            this.numericSourceTileWidth = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.numericSourceRow = new System.Windows.Forms.NumericUpDown();
            this.numericSourceCol = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.tabPageBackground = new System.Windows.Forms.TabPage();
            this.splitContainerBackground = new System.Windows.Forms.SplitContainer();
            this.splitContainerTiles = new System.Windows.Forms.SplitContainer();
            this.buttonWork = new System.Windows.Forms.Button();
            this.buttonTileset = new System.Windows.Forms.Button();
            this.tabPageCollision = new System.Windows.Forms.TabPage();
            this.splitContainerCollision = new System.Windows.Forms.SplitContainer();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.buttonSpawner = new System.Windows.Forms.Button();
            this.comboCollision = new System.Windows.Forms.ComboBox();
            this.buttonCollision = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.buttonEnemy = new System.Windows.Forms.Button();
            this.comboSpawn = new System.Windows.Forms.ComboBox();
            this.label8 = new System.Windows.Forms.Label();
            this.comboEnemy = new System.Windows.Forms.ComboBox();
            this.tabPageHelp = new System.Windows.Forms.TabPage();
            this.webBrowser = new System.Windows.Forms.WebBrowser();
            this.menuStrip1.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.tabControlMain.SuspendLayout();
            this.tabPageMap.SuspendLayout();
            this.splitContainerMap.Panel1.SuspendLayout();
            this.splitContainerMap.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridCol)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridTileHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridTileWidth)).BeginInit();
            this.groupBox5.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceTileHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceTileWidth)).BeginInit();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceRow)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceCol)).BeginInit();
            this.tabPageBackground.SuspendLayout();
            this.splitContainerBackground.Panel1.SuspendLayout();
            this.splitContainerBackground.SuspendLayout();
            this.splitContainerTiles.Panel2.SuspendLayout();
            this.splitContainerTiles.SuspendLayout();
            this.tabPageCollision.SuspendLayout();
            this.splitContainerCollision.Panel1.SuspendLayout();
            this.splitContainerCollision.SuspendLayout();
            this.groupBox7.SuspendLayout();
            this.tabPageHelp.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(873, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.openToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusX,
            this.toolStripStatusXValue,
            this.toolStripStatusY,
            this.toolStripStatusYValue});
            this.statusStrip.Location = new System.Drawing.Point(0, 572);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(873, 22);
            this.statusStrip.TabIndex = 1;
            this.statusStrip.Text = "statusStrip1";
            // 
            // toolStripStatusX
            // 
            this.toolStripStatusX.Name = "toolStripStatusX";
            this.toolStripStatusX.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusX.Text = "Pos X:";
            // 
            // toolStripStatusXValue
            // 
            this.toolStripStatusXValue.Name = "toolStripStatusXValue";
            this.toolStripStatusXValue.Size = new System.Drawing.Size(13, 17);
            this.toolStripStatusXValue.Text = "0";
            // 
            // toolStripStatusY
            // 
            this.toolStripStatusY.Name = "toolStripStatusY";
            this.toolStripStatusY.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusY.Text = "Pos Y:";
            // 
            // toolStripStatusYValue
            // 
            this.toolStripStatusYValue.Name = "toolStripStatusYValue";
            this.toolStripStatusYValue.Size = new System.Drawing.Size(13, 17);
            this.toolStripStatusYValue.Text = "0";
            // 
            // tabControlMain
            // 
            this.tabControlMain.Controls.Add(this.tabPageMap);
            this.tabControlMain.Controls.Add(this.tabPageBackground);
            this.tabControlMain.Controls.Add(this.tabPageCollision);
            this.tabControlMain.Controls.Add(this.tabPageHelp);
            this.tabControlMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControlMain.Location = new System.Drawing.Point(0, 24);
            this.tabControlMain.Name = "tabControlMain";
            this.tabControlMain.SelectedIndex = 0;
            this.tabControlMain.Size = new System.Drawing.Size(873, 548);
            this.tabControlMain.TabIndex = 2;
            // 
            // tabPageMap
            // 
            this.tabPageMap.Controls.Add(this.splitContainerMap);
            this.tabPageMap.Location = new System.Drawing.Point(4, 22);
            this.tabPageMap.Name = "tabPageMap";
            this.tabPageMap.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageMap.Size = new System.Drawing.Size(865, 522);
            this.tabPageMap.TabIndex = 0;
            this.tabPageMap.Text = "Map";
            this.tabPageMap.UseVisualStyleBackColor = true;
            // 
            // splitContainerMap
            // 
            this.splitContainerMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerMap.IsSplitterFixed = true;
            this.splitContainerMap.Location = new System.Drawing.Point(3, 3);
            this.splitContainerMap.Name = "splitContainerMap";
            // 
            // splitContainerMap.Panel1
            // 
            this.splitContainerMap.Panel1.AutoScroll = true;
            this.splitContainerMap.Panel1.BackColor = System.Drawing.Color.Transparent;
            this.splitContainerMap.Panel1.Controls.Add(this.groupBox6);
            this.splitContainerMap.Panel1.Controls.Add(this.groupBox5);
            // 
            // splitContainerMap.Panel2
            // 
            this.splitContainerMap.Panel2.AutoScroll = true;
            this.splitContainerMap.Panel2.BackColor = System.Drawing.Color.Transparent;
            this.splitContainerMap.Panel2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.splitContainerMap_Panel2_MouseMove);
            this.splitContainerMap.Panel2.Scroll += new System.Windows.Forms.ScrollEventHandler(this.splitContainerMap_Panel2_Scroll);
            this.splitContainerMap.Panel2.ClientSizeChanged += new System.EventHandler(this.splitContainerMap_Panel2_ClientSizeChanged);
            this.splitContainerMap.Size = new System.Drawing.Size(859, 516);
            this.splitContainerMap.SplitterDistance = 270;
            this.splitContainerMap.TabIndex = 0;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.groupBox1);
            this.groupBox6.Controls.Add(this.groupBox2);
            this.groupBox6.Location = new System.Drawing.Point(10, 12);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(249, 206);
            this.groupBox6.TabIndex = 0;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Map Selection";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.numericGridRow);
            this.groupBox1.Controls.Add(this.numericGridCol);
            this.groupBox1.Controls.Add(this.labelRow);
            this.groupBox1.Controls.Add(this.labelCol);
            this.groupBox1.Location = new System.Drawing.Point(6, 19);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(234, 86);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Grid Dimensions";
            // 
            // numericGridRow
            // 
            this.numericGridRow.Location = new System.Drawing.Point(71, 53);
            this.numericGridRow.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericGridRow.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridRow.Name = "numericGridRow";
            this.numericGridRow.Size = new System.Drawing.Size(120, 20);
            this.numericGridRow.TabIndex = 3;
            this.numericGridRow.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridRow.ValueChanged += new System.EventHandler(this.numericGridRow_ValueChanged);
            // 
            // numericGridCol
            // 
            this.numericGridCol.Location = new System.Drawing.Point(72, 27);
            this.numericGridCol.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericGridCol.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridCol.Name = "numericGridCol";
            this.numericGridCol.Size = new System.Drawing.Size(119, 20);
            this.numericGridCol.TabIndex = 2;
            this.numericGridCol.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridCol.ValueChanged += new System.EventHandler(this.numericGridCol_ValueChanged);
            // 
            // labelRow
            // 
            this.labelRow.AutoSize = true;
            this.labelRow.Location = new System.Drawing.Point(31, 55);
            this.labelRow.Name = "labelRow";
            this.labelRow.Size = new System.Drawing.Size(34, 13);
            this.labelRow.TabIndex = 1;
            this.labelRow.Text = "Rows";
            // 
            // labelCol
            // 
            this.labelCol.AutoSize = true;
            this.labelCol.Location = new System.Drawing.Point(19, 29);
            this.labelCol.Name = "labelCol";
            this.labelCol.Size = new System.Drawing.Size(47, 13);
            this.labelCol.TabIndex = 0;
            this.labelCol.Text = "Columns";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.numericGridTileHeight);
            this.groupBox2.Controls.Add(this.numericGridTileWidth);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(6, 111);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(234, 86);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Tile Dimensions";
            // 
            // numericGridTileHeight
            // 
            this.numericGridTileHeight.Location = new System.Drawing.Point(71, 53);
            this.numericGridTileHeight.Maximum = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            this.numericGridTileHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridTileHeight.Name = "numericGridTileHeight";
            this.numericGridTileHeight.Size = new System.Drawing.Size(120, 20);
            this.numericGridTileHeight.TabIndex = 3;
            this.numericGridTileHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridTileHeight.ValueChanged += new System.EventHandler(this.numericGridTileHeight_ValueChanged);
            // 
            // numericGridTileWidth
            // 
            this.numericGridTileWidth.Location = new System.Drawing.Point(72, 27);
            this.numericGridTileWidth.Maximum = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            this.numericGridTileWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridTileWidth.Name = "numericGridTileWidth";
            this.numericGridTileWidth.Size = new System.Drawing.Size(119, 20);
            this.numericGridTileWidth.TabIndex = 2;
            this.numericGridTileWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericGridTileWidth.ValueChanged += new System.EventHandler(this.numericGridTileWidth_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(27, 53);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Height";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(31, 27);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Width";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.groupBox4);
            this.groupBox5.Controls.Add(this.groupBox3);
            this.groupBox5.Location = new System.Drawing.Point(10, 226);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(249, 202);
            this.groupBox5.TabIndex = 7;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Image Selection";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.numericSourceTileHeight);
            this.groupBox4.Controls.Add(this.numericSourceTileWidth);
            this.groupBox4.Controls.Add(this.label5);
            this.groupBox4.Controls.Add(this.label6);
            this.groupBox4.Location = new System.Drawing.Point(6, 111);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(234, 86);
            this.groupBox4.TabIndex = 6;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Tile Dimensions";
            // 
            // numericSourceTileHeight
            // 
            this.numericSourceTileHeight.Location = new System.Drawing.Point(71, 52);
            this.numericSourceTileHeight.Maximum = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            this.numericSourceTileHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceTileHeight.Name = "numericSourceTileHeight";
            this.numericSourceTileHeight.Size = new System.Drawing.Size(120, 20);
            this.numericSourceTileHeight.TabIndex = 7;
            this.numericSourceTileHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceTileHeight.ValueChanged += new System.EventHandler(this.numericSourceTileHeight_ValueChanged);
            // 
            // numericSourceTileWidth
            // 
            this.numericSourceTileWidth.Location = new System.Drawing.Point(72, 26);
            this.numericSourceTileWidth.Maximum = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            this.numericSourceTileWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceTileWidth.Name = "numericSourceTileWidth";
            this.numericSourceTileWidth.Size = new System.Drawing.Size(119, 20);
            this.numericSourceTileWidth.TabIndex = 6;
            this.numericSourceTileWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceTileWidth.ValueChanged += new System.EventHandler(this.numericSourceTileWidth_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(27, 52);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(38, 13);
            this.label5.TabIndex = 5;
            this.label5.Text = "Height";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(31, 26);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(35, 13);
            this.label6.TabIndex = 4;
            this.label6.Text = "Width";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.numericSourceRow);
            this.groupBox3.Controls.Add(this.numericSourceCol);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Location = new System.Drawing.Point(6, 19);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(234, 86);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Source Dimensions";
            // 
            // numericSourceRow
            // 
            this.numericSourceRow.Location = new System.Drawing.Point(71, 53);
            this.numericSourceRow.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericSourceRow.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceRow.Name = "numericSourceRow";
            this.numericSourceRow.Size = new System.Drawing.Size(120, 20);
            this.numericSourceRow.TabIndex = 3;
            this.numericSourceRow.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceRow.ValueChanged += new System.EventHandler(this.numericSourceRow_ValueChanged);
            // 
            // numericSourceCol
            // 
            this.numericSourceCol.Location = new System.Drawing.Point(72, 27);
            this.numericSourceCol.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericSourceCol.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceCol.Name = "numericSourceCol";
            this.numericSourceCol.Size = new System.Drawing.Size(119, 20);
            this.numericSourceCol.TabIndex = 2;
            this.numericSourceCol.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericSourceCol.ValueChanged += new System.EventHandler(this.numericSourceCol_ValueChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(31, 55);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(34, 13);
            this.label3.TabIndex = 1;
            this.label3.Text = "Rows";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(19, 29);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Columns";
            // 
            // tabPageBackground
            // 
            this.tabPageBackground.Controls.Add(this.splitContainerBackground);
            this.tabPageBackground.Location = new System.Drawing.Point(4, 22);
            this.tabPageBackground.Name = "tabPageBackground";
            this.tabPageBackground.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageBackground.Size = new System.Drawing.Size(865, 522);
            this.tabPageBackground.TabIndex = 2;
            this.tabPageBackground.Text = "Background";
            this.tabPageBackground.UseVisualStyleBackColor = true;
            // 
            // splitContainerBackground
            // 
            this.splitContainerBackground.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerBackground.IsSplitterFixed = true;
            this.splitContainerBackground.Location = new System.Drawing.Point(3, 3);
            this.splitContainerBackground.Name = "splitContainerBackground";
            // 
            // splitContainerBackground.Panel1
            // 
            this.splitContainerBackground.Panel1.AutoScroll = true;
            this.splitContainerBackground.Panel1.Controls.Add(this.splitContainerTiles);
            // 
            // splitContainerBackground.Panel2
            // 
            this.splitContainerBackground.Panel2.AutoScroll = true;
            this.splitContainerBackground.Panel2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.splitContainerBackground_Panel2_MouseMove);
            this.splitContainerBackground.Panel2.Scroll += new System.Windows.Forms.ScrollEventHandler(this.splitContainerBackground_Panel2_Scroll);
            this.splitContainerBackground.Panel2.MouseClick += new System.Windows.Forms.MouseEventHandler(this.splitContainerBackground_Panel2_MouseClick);
            this.splitContainerBackground.Panel2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.splitContainerBackground_Panel2_MouseDown);
            this.splitContainerBackground.Panel2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.splitContainerBackground_Panel2_MouseUp);
            this.splitContainerBackground.Panel2.ClientSizeChanged += new System.EventHandler(this.splitContainerBackground_Panel2_ClientSizeChanged);
            this.splitContainerBackground.Size = new System.Drawing.Size(859, 516);
            this.splitContainerBackground.SplitterDistance = 299;
            this.splitContainerBackground.TabIndex = 0;
            // 
            // splitContainerTiles
            // 
            this.splitContainerTiles.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerTiles.IsSplitterFixed = true;
            this.splitContainerTiles.Location = new System.Drawing.Point(0, 0);
            this.splitContainerTiles.Name = "splitContainerTiles";
            this.splitContainerTiles.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainerTiles.Panel1
            // 
            this.splitContainerTiles.Panel1.AutoScroll = true;
            this.splitContainerTiles.Panel1.Scroll += new System.Windows.Forms.ScrollEventHandler(this.splitContainerTiles_Panel1_Scroll);
            this.splitContainerTiles.Panel1.MouseClick += new System.Windows.Forms.MouseEventHandler(this.splitContainerTiles_Panel1_MouseClick);
            this.splitContainerTiles.Panel1.ClientSizeChanged += new System.EventHandler(this.splitContainerTiles_Panel1_ClientSizeChanged);
            // 
            // splitContainerTiles.Panel2
            // 
            this.splitContainerTiles.Panel2.AutoScroll = true;
            this.splitContainerTiles.Panel2.Controls.Add(this.buttonWork);
            this.splitContainerTiles.Panel2.Controls.Add(this.buttonTileset);
            this.splitContainerTiles.Size = new System.Drawing.Size(299, 516);
            this.splitContainerTiles.SplitterDistance = 264;
            this.splitContainerTiles.TabIndex = 0;
            // 
            // buttonWork
            // 
            this.buttonWork.Location = new System.Drawing.Point(14, 89);
            this.buttonWork.Name = "buttonWork";
            this.buttonWork.Size = new System.Drawing.Size(267, 51);
            this.buttonWork.TabIndex = 1;
            this.buttonWork.Text = "Change Working Directory";
            this.buttonWork.UseVisualStyleBackColor = true;
            this.buttonWork.Click += new System.EventHandler(this.buttonWork_Click);
            // 
            // buttonTileset
            // 
            this.buttonTileset.Location = new System.Drawing.Point(14, 17);
            this.buttonTileset.Name = "buttonTileset";
            this.buttonTileset.Size = new System.Drawing.Size(267, 51);
            this.buttonTileset.TabIndex = 0;
            this.buttonTileset.Text = "Load Tileset";
            this.buttonTileset.UseVisualStyleBackColor = true;
            this.buttonTileset.Click += new System.EventHandler(this.buttonTileset_Click);
            // 
            // tabPageCollision
            // 
            this.tabPageCollision.Controls.Add(this.splitContainerCollision);
            this.tabPageCollision.Location = new System.Drawing.Point(4, 22);
            this.tabPageCollision.Name = "tabPageCollision";
            this.tabPageCollision.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageCollision.Size = new System.Drawing.Size(865, 522);
            this.tabPageCollision.TabIndex = 3;
            this.tabPageCollision.Text = "Collision";
            this.tabPageCollision.UseVisualStyleBackColor = true;
            // 
            // splitContainerCollision
            // 
            this.splitContainerCollision.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainerCollision.IsSplitterFixed = true;
            this.splitContainerCollision.Location = new System.Drawing.Point(3, 3);
            this.splitContainerCollision.Name = "splitContainerCollision";
            // 
            // splitContainerCollision.Panel1
            // 
            this.splitContainerCollision.Panel1.AutoScroll = true;
            this.splitContainerCollision.Panel1.Controls.Add(this.groupBox7);
            // 
            // splitContainerCollision.Panel2
            // 
            this.splitContainerCollision.Panel2.AutoScroll = true;
            this.splitContainerCollision.Panel2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.splitContainerCollision_Panel2_MouseMove);
            this.splitContainerCollision.Panel2.Scroll += new System.Windows.Forms.ScrollEventHandler(this.splitContainerCollision_Panel2_Scroll);
            this.splitContainerCollision.Panel2.MouseClick += new System.Windows.Forms.MouseEventHandler(this.splitContainerCollision_Panel2_MouseClick);
            this.splitContainerCollision.Panel2.ClientSizeChanged += new System.EventHandler(this.splitContainerCollision_Panel2_ClientSizeChanged);
            this.splitContainerCollision.Size = new System.Drawing.Size(859, 516);
            this.splitContainerCollision.SplitterDistance = 237;
            this.splitContainerCollision.TabIndex = 0;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.label9);
            this.groupBox7.Controls.Add(this.buttonSpawner);
            this.groupBox7.Controls.Add(this.comboCollision);
            this.groupBox7.Controls.Add(this.buttonCollision);
            this.groupBox7.Controls.Add(this.label7);
            this.groupBox7.Controls.Add(this.buttonEnemy);
            this.groupBox7.Controls.Add(this.comboSpawn);
            this.groupBox7.Controls.Add(this.label8);
            this.groupBox7.Controls.Add(this.comboEnemy);
            this.groupBox7.Location = new System.Drawing.Point(13, 17);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(212, 277);
            this.groupBox7.TabIndex = 0;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Tile Types";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(6, 16);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(69, 13);
            this.label9.TabIndex = 5;
            this.label9.Text = "Enemy Type:";
            // 
            // buttonSpawner
            // 
            this.buttonSpawner.Location = new System.Drawing.Point(9, 226);
            this.buttonSpawner.Name = "buttonSpawner";
            this.buttonSpawner.Size = new System.Drawing.Size(193, 41);
            this.buttonSpawner.TabIndex = 8;
            this.buttonSpawner.Text = "Spawner";
            this.buttonSpawner.UseVisualStyleBackColor = true;
            this.buttonSpawner.Click += new System.EventHandler(this.buttonSpawner_Click);
            // 
            // comboCollision
            // 
            this.comboCollision.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboCollision.FormattingEnabled = true;
            this.comboCollision.Items.AddRange(new object[] {
            "Solid",
            "Partial Solid",
            "Moving",
            "Unstable",
            "Trap"});
            this.comboCollision.Location = new System.Drawing.Point(81, 105);
            this.comboCollision.Name = "comboCollision";
            this.comboCollision.Size = new System.Drawing.Size(121, 21);
            this.comboCollision.TabIndex = 0;
            // 
            // buttonCollision
            // 
            this.buttonCollision.Location = new System.Drawing.Point(9, 132);
            this.buttonCollision.Name = "buttonCollision";
            this.buttonCollision.Size = new System.Drawing.Size(193, 41);
            this.buttonCollision.TabIndex = 7;
            this.buttonCollision.Text = "Collision";
            this.buttonCollision.UseVisualStyleBackColor = true;
            this.buttonCollision.Click += new System.EventHandler(this.buttonCollision_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(0, 108);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(75, 13);
            this.label7.TabIndex = 1;
            this.label7.Text = "Collision Type:";
            // 
            // buttonEnemy
            // 
            this.buttonEnemy.Location = new System.Drawing.Point(9, 40);
            this.buttonEnemy.Name = "buttonEnemy";
            this.buttonEnemy.Size = new System.Drawing.Size(193, 41);
            this.buttonEnemy.TabIndex = 6;
            this.buttonEnemy.Text = "Enemy";
            this.buttonEnemy.UseVisualStyleBackColor = true;
            this.buttonEnemy.Click += new System.EventHandler(this.buttonEnemy_Click);
            // 
            // comboSpawn
            // 
            this.comboSpawn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboSpawn.FormattingEnabled = true;
            this.comboSpawn.Items.AddRange(new object[] {
            "Turret",
            "Air",
            "Ground",
            "Boss"});
            this.comboSpawn.Location = new System.Drawing.Point(81, 199);
            this.comboSpawn.Name = "comboSpawn";
            this.comboSpawn.Size = new System.Drawing.Size(121, 21);
            this.comboSpawn.TabIndex = 2;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(1, 202);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(79, 13);
            this.label8.TabIndex = 3;
            this.label8.Text = "Spawner Type:";
            // 
            // comboEnemy
            // 
            this.comboEnemy.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboEnemy.FormattingEnabled = true;
            this.comboEnemy.Items.AddRange(new object[] {
            "Gun Turret",
            "Frost Turret",
            "Fire Turret",
            "Multi-Fire Turret",
            "Attack Drone",
            "Seeker Drone",
            "Heavy Attack Drone",
            "Mech",
            "Siege Walker",
            "FLCL Mech",
            "Apple Mech",
            "Pimp Striker",
            "Death Pirate"});
            this.comboEnemy.Location = new System.Drawing.Point(81, 13);
            this.comboEnemy.Name = "comboEnemy";
            this.comboEnemy.Size = new System.Drawing.Size(121, 21);
            this.comboEnemy.TabIndex = 4;
            // 
            // tabPageHelp
            // 
            this.tabPageHelp.Controls.Add(this.webBrowser);
            this.tabPageHelp.Location = new System.Drawing.Point(4, 22);
            this.tabPageHelp.Name = "tabPageHelp";
            this.tabPageHelp.Padding = new System.Windows.Forms.Padding(3);
            this.tabPageHelp.Size = new System.Drawing.Size(865, 522);
            this.tabPageHelp.TabIndex = 1;
            this.tabPageHelp.Text = "Help";
            this.tabPageHelp.UseVisualStyleBackColor = true;
            // 
            // webBrowser
            // 
            this.webBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.webBrowser.Location = new System.Drawing.Point(3, 3);
            this.webBrowser.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser.Name = "webBrowser";
            this.webBrowser.Size = new System.Drawing.Size(859, 516);
            this.webBrowser.TabIndex = 0;
            this.webBrowser.Url = new System.Uri("http://www.google.com/", System.UriKind.Absolute);
            // 
            // Map_Editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(873, 594);
            this.Controls.Add(this.tabControlMain);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.menuStrip1);
            this.Name = "Map_Editor";
            this.Text = "Map Editor";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Map_Editor_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.tabControlMain.ResumeLayout(false);
            this.tabPageMap.ResumeLayout(false);
            this.splitContainerMap.Panel1.ResumeLayout(false);
            this.splitContainerMap.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridCol)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridTileHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericGridTileWidth)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceTileHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceTileWidth)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceRow)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericSourceCol)).EndInit();
            this.tabPageBackground.ResumeLayout(false);
            this.splitContainerBackground.Panel1.ResumeLayout(false);
            this.splitContainerBackground.ResumeLayout(false);
            this.splitContainerTiles.Panel2.ResumeLayout(false);
            this.splitContainerTiles.ResumeLayout(false);
            this.tabPageCollision.ResumeLayout(false);
            this.splitContainerCollision.Panel1.ResumeLayout(false);
            this.splitContainerCollision.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            this.tabPageHelp.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.TabControl tabControlMain;
        private System.Windows.Forms.TabPage tabPageMap;
        private System.Windows.Forms.TabPage tabPageHelp;
        private System.Windows.Forms.TabPage tabPageBackground;
        private System.Windows.Forms.TabPage tabPageCollision;
        private System.Windows.Forms.WebBrowser webBrowser;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusX;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusXValue;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusY;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusYValue;
        private System.Windows.Forms.SplitContainer splitContainerMap;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.NumericUpDown numericGridRow;
        private System.Windows.Forms.NumericUpDown numericGridCol;
        private System.Windows.Forms.Label labelRow;
        private System.Windows.Forms.Label labelCol;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NumericUpDown numericSourceRow;
        private System.Windows.Forms.NumericUpDown numericSourceCol;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.NumericUpDown numericGridTileHeight;
        private System.Windows.Forms.NumericUpDown numericGridTileWidth;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.NumericUpDown numericSourceTileHeight;
        private System.Windows.Forms.NumericUpDown numericSourceTileWidth;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.SplitContainer splitContainerCollision;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.ComboBox comboCollision;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.ComboBox comboEnemy;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox comboSpawn;
        private System.Windows.Forms.Button buttonSpawner;
        private System.Windows.Forms.Button buttonCollision;
        private System.Windows.Forms.Button buttonEnemy;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.SplitContainer splitContainerBackground;
        private System.Windows.Forms.SplitContainer splitContainerTiles;
        private System.Windows.Forms.Button buttonTileset;
        private System.Windows.Forms.Button buttonWork;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;

    }
}

