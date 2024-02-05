using System.Windows.Forms;

namespace AC_e_Reader_Card_Creator
{
    partial class eReaderCCC
    {
        // TODO: clean up this shit - lots of unnecessary components from testing

        private System.ComponentModel.IContainer components = null;
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(eReaderCCC));
            this.comboBox_ItemName = new System.Windows.Forms.ComboBox();
            this.textBox_ItemID = new System.Windows.Forms.TextBox();
            this.header_ItemID = new System.Windows.Forms.Label();
            this.header_ItemName = new System.Windows.Forms.Label();
            this.header_Gift = new System.Windows.Forms.Label();
            this.header_LetterText = new System.Windows.Forms.Label();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_FileNew = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_NewCharCard = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_FileOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_OpenCharCard = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_ClearInputs = new System.Windows.Forms.ToolStripMenuItem();
            this.menu_OpenFileDir = new System.Windows.Forms.ToolStripMenuItem();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tutorialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.videoTutorialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gitHubRepositoryToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label_Version = new System.Windows.Forms.Label();
            this.header_Stationery = new System.Windows.Forms.Label();
            this.comboBox_Stationery = new System.Windows.Forms.ComboBox();
            this.header_Sender = new System.Windows.Forms.Label();
            this.comboBox_Sender = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.btn_SaveRAW = new System.Windows.Forms.Button();
            this.btn_GenDotCode = new System.Windows.Forms.Button();
            this.header_Greeting = new System.Windows.Forms.Label();
            this.header_Body = new System.Windows.Forms.Label();
            this.textBox_Body = new System.Windows.Forms.TextBox();
            this.header_Closing = new System.Windows.Forms.Label();
            this.textBox_Closing = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.label9 = new System.Windows.Forms.Label();
            this.label_Greeting = new System.Windows.Forms.Label();
            this.pictureBox_Stationery = new System.Windows.Forms.PictureBox();
            this.label_Line1 = new System.Windows.Forms.Label();
            this.label_Line2 = new System.Windows.Forms.Label();
            this.label_Line3 = new System.Windows.Forms.Label();
            this.label_Line4 = new System.Windows.Forms.Label();
            this.label_Line5 = new System.Windows.Forms.Label();
            this.label_Line6 = new System.Windows.Forms.Label();
            this.label_Closing = new System.Windows.Forms.Label();
            this.comboBox_Greeting = new System.Windows.Forms.ComboBox();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Stationery)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBox_ItemName
            // 
            this.comboBox_ItemName.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBox_ItemName.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBox_ItemName.FormattingEnabled = true;
            this.comboBox_ItemName.Location = new System.Drawing.Point(149, 735);
            this.comboBox_ItemName.Name = "comboBox_ItemName";
            this.comboBox_ItemName.Size = new System.Drawing.Size(194, 28);
            this.comboBox_ItemName.TabIndex = 0;
            this.comboBox_ItemName.SelectedIndexChanged += new System.EventHandler(this.ItemNameChanged);
            // 
            // textBox_ItemID
            // 
            this.textBox_ItemID.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.textBox_ItemID.Location = new System.Drawing.Point(55, 735);
            this.textBox_ItemID.MaxLength = 4;
            this.textBox_ItemID.Name = "textBox_ItemID";
            this.textBox_ItemID.Size = new System.Drawing.Size(88, 26);
            this.textBox_ItemID.TabIndex = 1;
            this.textBox_ItemID.TextChanged += new System.EventHandler(this.ItemIDChanged);
            this.textBox_ItemID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.ItemIDKeyPress);
            this.textBox_ItemID.KeyUp += new System.Windows.Forms.KeyEventHandler(this.ItemIDKeyUp);
            // 
            // header_ItemID
            // 
            this.header_ItemID.AutoSize = true;
            this.header_ItemID.Location = new System.Drawing.Point(28, 712);
            this.header_ItemID.Name = "header_ItemID";
            this.header_ItemID.Size = new System.Drawing.Size(62, 20);
            this.header_ItemID.TabIndex = 2;
            this.header_ItemID.Text = "Item ID";
            // 
            // header_ItemName
            // 
            this.header_ItemName.AutoSize = true;
            this.header_ItemName.Location = new System.Drawing.Point(145, 712);
            this.header_ItemName.Name = "header_ItemName";
            this.header_ItemName.Size = new System.Drawing.Size(87, 20);
            this.header_ItemName.TabIndex = 3;
            this.header_ItemName.Text = "Item Name";
            // 
            // header_Gift
            // 
            this.header_Gift.AutoSize = true;
            this.header_Gift.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.header_Gift.Location = new System.Drawing.Point(25, 673);
            this.header_Gift.Name = "header_Gift";
            this.header_Gift.Size = new System.Drawing.Size(164, 29);
            this.header_Gift.TabIndex = 4;
            this.header_Gift.Text = "Attached Gift";
            // 
            // header_LetterText
            // 
            this.header_LetterText.AutoSize = true;
            this.header_LetterText.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.header_LetterText.Location = new System.Drawing.Point(25, 59);
            this.header_LetterText.Name = "header_LetterText";
            this.header_LetterText.Size = new System.Drawing.Size(133, 29);
            this.header_LetterText.TabIndex = 6;
            this.header_LetterText.Text = "Letter Text";
            // 
            // menuStrip1
            // 
            this.menuStrip1.GripMargin = new System.Windows.Forms.Padding(2, 2, 0, 2);
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem1,
            this.editToolStripMenuItem1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1117, 33);
            this.menuStrip1.TabIndex = 8;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem1
            // 
            this.fileToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_FileNew,
            this.menu_FileOpen});
            this.fileToolStripMenuItem1.Name = "fileToolStripMenuItem1";
            this.fileToolStripMenuItem1.Size = new System.Drawing.Size(54, 29);
            this.fileToolStripMenuItem1.Text = "File";
            // 
            // menu_FileNew
            // 
            this.menu_FileNew.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_NewCharCard});
            this.menu_FileNew.Name = "menu_FileNew";
            this.menu_FileNew.Size = new System.Drawing.Size(158, 34);
            this.menu_FileNew.Text = "New";
            // 
            // menu_NewCharCard
            // 
            this.menu_NewCharCard.Name = "menu_NewCharCard";
            this.menu_NewCharCard.Size = new System.Drawing.Size(268, 34);
            this.menu_NewCharCard.Text = "Character Card (AC)";
            this.menu_NewCharCard.Click += new System.EventHandler(this.MenuNewCard);
            // 
            // menu_FileOpen
            // 
            this.menu_FileOpen.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_OpenCharCard});
            this.menu_FileOpen.Name = "menu_FileOpen";
            this.menu_FileOpen.Size = new System.Drawing.Size(158, 34);
            this.menu_FileOpen.Text = "Open";
            // 
            // menu_OpenCharCard
            // 
            this.menu_OpenCharCard.Name = "menu_OpenCharCard";
            this.menu_OpenCharCard.Size = new System.Drawing.Size(268, 34);
            this.menu_OpenCharCard.Text = "Character Card (AC)";
            this.menu_OpenCharCard.Click += new System.EventHandler(this.OpenCharCard);
            // 
            // editToolStripMenuItem1
            // 
            this.editToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menu_ClearInputs,
            this.menu_OpenFileDir});
            this.editToolStripMenuItem1.Name = "editToolStripMenuItem1";
            this.editToolStripMenuItem1.Size = new System.Drawing.Size(58, 29);
            this.editToolStripMenuItem1.Text = "Edit";
            // 
            // menu_ClearInputs
            // 
            this.menu_ClearInputs.Name = "menu_ClearInputs";
            this.menu_ClearInputs.Size = new System.Drawing.Size(266, 34);
            this.menu_ClearInputs.Text = "Clear Inputs";
            this.menu_ClearInputs.Click += new System.EventHandler(this.MenuClearValues);
            // 
            // menu_OpenFileDir
            // 
            this.menu_OpenFileDir.Name = "menu_OpenFileDir";
            this.menu_OpenFileDir.Size = new System.Drawing.Size(266, 34);
            this.menu_OpenFileDir.Text = "Open File Directory";
            this.menu_OpenFileDir.Click += new System.EventHandler(this.OpenDecompressedFileDir);
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem1,
            this.saveAsToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(188, 34);
            this.saveToolStripMenuItem.Text = "New";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(188, 34);
            this.openToolStripMenuItem.Text = "Open";
            // 
            // saveToolStripMenuItem1
            // 
            this.saveToolStripMenuItem1.Name = "saveToolStripMenuItem1";
            this.saveToolStripMenuItem1.Size = new System.Drawing.Size(188, 34);
            this.saveToolStripMenuItem1.Text = "Save";
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(188, 34);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingsToolStripMenuItem,
            this.resetToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(58, 29);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // settingsToolStripMenuItem
            // 
            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(198, 34);
            this.settingsToolStripMenuItem.Text = "Settings";
            // 
            // resetToolStripMenuItem
            // 
            this.resetToolStripMenuItem.Name = "resetToolStripMenuItem";
            this.resetToolStripMenuItem.Size = new System.Drawing.Size(198, 34);
            this.resetToolStripMenuItem.Text = "Reset Card";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.tutorialToolStripMenuItem,
            this.videoTutorialToolStripMenuItem,
            this.gitHubRepositoryToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(65, 29);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // tutorialToolStripMenuItem
            // 
            this.tutorialToolStripMenuItem.Name = "tutorialToolStripMenuItem";
            this.tutorialToolStripMenuItem.Size = new System.Drawing.Size(260, 34);
            this.tutorialToolStripMenuItem.Text = "Tutorial";
            // 
            // videoTutorialToolStripMenuItem
            // 
            this.videoTutorialToolStripMenuItem.Name = "videoTutorialToolStripMenuItem";
            this.videoTutorialToolStripMenuItem.Size = new System.Drawing.Size(260, 34);
            this.videoTutorialToolStripMenuItem.Text = "Video Tutorial";
            // 
            // gitHubRepositoryToolStripMenuItem
            // 
            this.gitHubRepositoryToolStripMenuItem.Name = "gitHubRepositoryToolStripMenuItem";
            this.gitHubRepositoryToolStripMenuItem.Size = new System.Drawing.Size(260, 34);
            this.gitHubRepositoryToolStripMenuItem.Text = "GitHub Repository";
            // 
            // label_Version
            // 
            this.label_Version.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label_Version.AutoSize = true;
            this.label_Version.Font = new System.Drawing.Font("Courier New", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Version.Location = new System.Drawing.Point(1017, 782);
            this.label_Version.Name = "label_Version";
            this.label_Version.Size = new System.Drawing.Size(0, 18);
            this.label_Version.TabIndex = 9;
            // 
            // header_Stationery
            // 
            this.header_Stationery.AutoSize = true;
            this.header_Stationery.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.header_Stationery.Location = new System.Drawing.Point(25, 477);
            this.header_Stationery.Name = "header_Stationery";
            this.header_Stationery.Size = new System.Drawing.Size(130, 29);
            this.header_Stationery.TabIndex = 12;
            this.header_Stationery.Text = "Stationery";
            // 
            // comboBox_Stationery
            // 
            this.comboBox_Stationery.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBox_Stationery.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBox_Stationery.FormattingEnabled = true;
            this.comboBox_Stationery.Location = new System.Drawing.Point(31, 509);
            this.comboBox_Stationery.Name = "comboBox_Stationery";
            this.comboBox_Stationery.Size = new System.Drawing.Size(312, 28);
            this.comboBox_Stationery.TabIndex = 13;
            this.comboBox_Stationery.SelectedIndexChanged += new System.EventHandler(this.StationeryChanged);
            // 
            // header_Sender
            // 
            this.header_Sender.AutoSize = true;
            this.header_Sender.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.header_Sender.Location = new System.Drawing.Point(25, 569);
            this.header_Sender.Name = "header_Sender";
            this.header_Sender.Size = new System.Drawing.Size(94, 29);
            this.header_Sender.TabIndex = 14;
            this.header_Sender.Text = "Sender";
            // 
            // comboBox_Sender
            // 
            this.comboBox_Sender.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBox_Sender.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBox_Sender.FormattingEnabled = true;
            this.comboBox_Sender.Location = new System.Drawing.Point(31, 603);
            this.comboBox_Sender.Name = "comboBox_Sender";
            this.comboBox_Sender.Size = new System.Drawing.Size(312, 28);
            this.comboBox_Sender.TabIndex = 15;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            this.label5.Location = new System.Drawing.Point(384, 59);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(103, 29);
            this.label5.TabIndex = 16;
            this.label5.Text = "Preview";
            // 
            // btn_SaveRAW
            // 
            this.btn_SaveRAW.Location = new System.Drawing.Point(945, 724);
            this.btn_SaveRAW.Name = "btn_SaveRAW";
            this.btn_SaveRAW.Size = new System.Drawing.Size(138, 37);
            this.btn_SaveRAW.TabIndex = 17;
            this.btn_SaveRAW.Text = "Save .RAW";
            this.btn_SaveRAW.UseVisualStyleBackColor = true;
            this.btn_SaveRAW.Click += new System.EventHandler(this.SaveRAW);
            // 
            // btn_GenDotCode
            // 
            this.btn_GenDotCode.Location = new System.Drawing.Point(746, 724);
            this.btn_GenDotCode.Name = "btn_GenDotCode";
            this.btn_GenDotCode.Size = new System.Drawing.Size(193, 37);
            this.btn_GenDotCode.TabIndex = 18;
            this.btn_GenDotCode.Text = "Generate Dot Code";
            this.btn_GenDotCode.UseVisualStyleBackColor = true;
            this.btn_GenDotCode.Click += new System.EventHandler(this.GenerateDotCode);
            // 
            // header_Greeting
            // 
            this.header_Greeting.AutoSize = true;
            this.header_Greeting.Location = new System.Drawing.Point(26, 98);
            this.header_Greeting.Name = "header_Greeting";
            this.header_Greeting.Size = new System.Drawing.Size(71, 20);
            this.header_Greeting.TabIndex = 19;
            this.header_Greeting.Text = "Greeting";
            // 
            // header_Body
            // 
            this.header_Body.AutoSize = true;
            this.header_Body.Location = new System.Drawing.Point(26, 170);
            this.header_Body.Name = "header_Body";
            this.header_Body.Size = new System.Drawing.Size(45, 20);
            this.header_Body.TabIndex = 20;
            this.header_Body.Text = "Body";
            // 
            // textBox_Body
            // 
            this.textBox_Body.Location = new System.Drawing.Point(30, 193);
            this.textBox_Body.MaxLength = 180;
            this.textBox_Body.Multiline = true;
            this.textBox_Body.Name = "textBox_Body";
            this.textBox_Body.Size = new System.Drawing.Size(312, 188);
            this.textBox_Body.TabIndex = 21;
            this.textBox_Body.TextChanged += new System.EventHandler(this.LetterBodyChanged);
            this.textBox_Body.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.LetterBodyKeyPress);
            // 
            // header_Closing
            // 
            this.header_Closing.AutoSize = true;
            this.header_Closing.Location = new System.Drawing.Point(28, 399);
            this.header_Closing.Name = "header_Closing";
            this.header_Closing.Size = new System.Drawing.Size(61, 20);
            this.header_Closing.TabIndex = 22;
            this.header_Closing.Text = "Closing";
            // 
            // textBox_Closing
            // 
            this.textBox_Closing.Location = new System.Drawing.Point(31, 422);
            this.textBox_Closing.Name = "textBox_Closing";
            this.textBox_Closing.Size = new System.Drawing.Size(312, 26);
            this.textBox_Closing.TabIndex = 23;
            this.textBox_Closing.TextChanged += new System.EventHandler(this.ClosingChanged);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "RAW Files (*raw)|*raw";
            this.openFileDialog1.Title = "Open e-Reader Card RAW File";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(28, 741);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(25, 20);
            this.label9.TabIndex = 25;
            this.label9.Text = "0x";
            // 
            // label_Greeting
            // 
            this.label_Greeting.AutoSize = true;
            this.label_Greeting.BackColor = System.Drawing.Color.Transparent;
            this.label_Greeting.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Greeting.Location = new System.Drawing.Point(77, 57);
            this.label_Greeting.Name = "label_Greeting";
            this.label_Greeting.Size = new System.Drawing.Size(0, 32);
            this.label_Greeting.TabIndex = 27;
            // 
            // pictureBox_Stationery
            // 
            this.pictureBox_Stationery.BackgroundImage = global::AC_e_Reader_Card_Creator.Properties.Resources.Airmail_Paper_PG;
            this.pictureBox_Stationery.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBox_Stationery.Location = new System.Drawing.Point(389, 97);
            this.pictureBox_Stationery.Name = "pictureBox_Stationery";
            this.pictureBox_Stationery.Size = new System.Drawing.Size(694, 605);
            this.pictureBox_Stationery.TabIndex = 10;
            this.pictureBox_Stationery.TabStop = false;
            // 
            // label_Line1
            // 
            this.label_Line1.BackColor = System.Drawing.Color.Transparent;
            this.label_Line1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line1.Location = new System.Drawing.Point(77, 152);
            this.label_Line1.Name = "label_Line1";
            this.label_Line1.Size = new System.Drawing.Size(532, 53);
            this.label_Line1.TabIndex = 28;
            // 
            // label_Line2
            // 
            this.label_Line2.BackColor = System.Drawing.Color.Transparent;
            this.label_Line2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line2.Location = new System.Drawing.Point(77, 205);
            this.label_Line2.Name = "label_Line2";
            this.label_Line2.Size = new System.Drawing.Size(532, 56);
            this.label_Line2.TabIndex = 29;
            // 
            // label_Line3
            // 
            this.label_Line3.BackColor = System.Drawing.Color.Transparent;
            this.label_Line3.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line3.Location = new System.Drawing.Point(77, 261);
            this.label_Line3.Name = "label_Line3";
            this.label_Line3.Size = new System.Drawing.Size(532, 51);
            this.label_Line3.TabIndex = 30;
            // 
            // label_Line4
            // 
            this.label_Line4.BackColor = System.Drawing.Color.Transparent;
            this.label_Line4.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line4.Location = new System.Drawing.Point(77, 312);
            this.label_Line4.Name = "label_Line4";
            this.label_Line4.Size = new System.Drawing.Size(532, 53);
            this.label_Line4.TabIndex = 31;
            // 
            // label_Line5
            // 
            this.label_Line5.BackColor = System.Drawing.Color.Transparent;
            this.label_Line5.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line5.Location = new System.Drawing.Point(77, 365);
            this.label_Line5.Name = "label_Line5";
            this.label_Line5.Size = new System.Drawing.Size(532, 51);
            this.label_Line5.TabIndex = 32;
            // 
            // label_Line6
            // 
            this.label_Line6.BackColor = System.Drawing.Color.Transparent;
            this.label_Line6.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Line6.Location = new System.Drawing.Point(74, 416);
            this.label_Line6.Name = "label_Line6";
            this.label_Line6.Size = new System.Drawing.Size(535, 61);
            this.label_Line6.TabIndex = 33;
            // 
            // label_Closing
            // 
            this.label_Closing.BackColor = System.Drawing.Color.Transparent;
            this.label_Closing.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Closing.Location = new System.Drawing.Point(83, 477);
            this.label_Closing.Name = "label_Closing";
            this.label_Closing.Size = new System.Drawing.Size(539, 62);
            this.label_Closing.TabIndex = 34;
            this.label_Closing.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // comboBox_Greeting
            // 
            this.comboBox_Greeting.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.SuggestAppend;
            this.comboBox_Greeting.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.comboBox_Greeting.FormattingEnabled = true;
            this.comboBox_Greeting.Location = new System.Drawing.Point(30, 121);
            this.comboBox_Greeting.Name = "comboBox_Greeting";
            this.comboBox_Greeting.Size = new System.Drawing.Size(312, 28);
            this.comboBox_Greeting.TabIndex = 35;
            this.comboBox_Greeting.SelectedIndexChanged += new System.EventHandler(this.GreetingChanged);
            // 
            // eReaderCCC
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1117, 809);
            this.Controls.Add(this.label_Line1);
            this.Controls.Add(this.comboBox_Greeting);
            this.Controls.Add(this.label_Closing);
            this.Controls.Add(this.label_Line6);
            this.Controls.Add(this.label_Line5);
            this.Controls.Add(this.label_Line4);
            this.Controls.Add(this.label_Line3);
            this.Controls.Add(this.label_Line2);
            this.Controls.Add(this.label_Greeting);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.textBox_Closing);
            this.Controls.Add(this.header_Closing);
            this.Controls.Add(this.textBox_Body);
            this.Controls.Add(this.header_Body);
            this.Controls.Add(this.header_Greeting);
            this.Controls.Add(this.btn_GenDotCode);
            this.Controls.Add(this.btn_SaveRAW);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.comboBox_Sender);
            this.Controls.Add(this.header_Sender);
            this.Controls.Add(this.comboBox_Stationery);
            this.Controls.Add(this.header_Stationery);
            this.Controls.Add(this.pictureBox_Stationery);
            this.Controls.Add(this.label_Version);
            this.Controls.Add(this.header_LetterText);
            this.Controls.Add(this.header_Gift);
            this.Controls.Add(this.header_ItemName);
            this.Controls.Add(this.header_ItemID);
            this.Controls.Add(this.textBox_ItemID);
            this.Controls.Add(this.comboBox_ItemName);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "eReaderCCC";
            this.Text = "AC e-Reader Character Card Creator";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_Stationery)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox_ItemName;
        private TextBox textBox_ItemID;
        private Label header_ItemID;
        private Label header_ItemName;
        private Label header_Gift;
        private Label header_LetterText;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem fileToolStripMenuItem;
        private ToolStripMenuItem saveToolStripMenuItem;
        private ToolStripMenuItem openToolStripMenuItem;
        private ToolStripMenuItem saveToolStripMenuItem1;
        private ToolStripMenuItem saveAsToolStripMenuItem;
        private ToolStripMenuItem editToolStripMenuItem;
        private ToolStripMenuItem settingsToolStripMenuItem;
        private ToolStripMenuItem resetToolStripMenuItem;
        private ToolStripMenuItem helpToolStripMenuItem;
        private ToolStripMenuItem tutorialToolStripMenuItem;
        private ToolStripMenuItem videoTutorialToolStripMenuItem;
        private ToolStripMenuItem gitHubRepositoryToolStripMenuItem;
        private Label label_Version;
        private Label header_Stationery;
        private ComboBox comboBox_Stationery;
        private Label header_Sender;
        private ComboBox comboBox_Sender;
        private Label label5;
        private Button btn_SaveRAW;
        private Button btn_GenDotCode;
        private PictureBox pictureBox_Stationery;
        private Label header_Greeting;
        private Label header_Body;
        private TextBox textBox_Body;
        private Label header_Closing;
        private TextBox textBox_Closing;
        private ToolStripMenuItem fileToolStripMenuItem1;
        private ToolStripMenuItem editToolStripMenuItem1;
        private ToolStripMenuItem menu_FileOpen;
        private OpenFileDialog openFileDialog1;
        private Label label9;
        private Label label_Greeting;
        private Label label_Line1;
        private Label label_Line2;
        private Label label_Line3;
        private Label label_Line4;
        private Label label_Line5;
        private Label label_Line6;
        private Label label_Closing;
        private ComboBox comboBox_Greeting;
        private ToolStripMenuItem menu_FileNew;
        private ToolStripMenuItem menu_NewCharCard;
        private ToolStripMenuItem menu_ClearInputs;
        private ToolStripMenuItem menu_OpenCharCard;
        private ToolStripMenuItem menu_OpenFileDir;
    }
}

