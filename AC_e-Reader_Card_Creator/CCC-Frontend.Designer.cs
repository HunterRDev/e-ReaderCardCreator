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
            comboBox_ItemName = new ComboBox();
            textBox_ItemID = new TextBox();
            header_ItemID = new Label();
            header_ItemName = new Label();
            header_Gift = new Label();
            header_LetterText = new Label();
            menuStrip1 = new MenuStrip();
            fileToolStripMenuItem1 = new ToolStripMenuItem();
            menu_FileNew = new ToolStripMenuItem();
            menu_NewCharCard = new ToolStripMenuItem();
            menu_FileOpen = new ToolStripMenuItem();
            menu_OpenCharCard = new ToolStripMenuItem();
            editToolStripMenuItem1 = new ToolStripMenuItem();
            menu_ClearInputs = new ToolStripMenuItem();
            menu_OpenFileDir = new ToolStripMenuItem();
            menu_View = new ToolStripMenuItem();
            menu_ToggleDarkMode = new ToolStripMenuItem();
            menu_Help = new ToolStripMenuItem();
            menu_GitRepo = new ToolStripMenuItem();
            menu_About = new ToolStripMenuItem();
            fileToolStripMenuItem = new ToolStripMenuItem();
            saveToolStripMenuItem = new ToolStripMenuItem();
            openToolStripMenuItem = new ToolStripMenuItem();
            saveToolStripMenuItem1 = new ToolStripMenuItem();
            saveAsToolStripMenuItem = new ToolStripMenuItem();
            editToolStripMenuItem = new ToolStripMenuItem();
            settingsToolStripMenuItem = new ToolStripMenuItem();
            resetToolStripMenuItem = new ToolStripMenuItem();
            helpToolStripMenuItem = new ToolStripMenuItem();
            tutorialToolStripMenuItem = new ToolStripMenuItem();
            videoTutorialToolStripMenuItem = new ToolStripMenuItem();
            gitHubRepositoryToolStripMenuItem = new ToolStripMenuItem();
            label_Version = new Label();
            header_Stationery = new Label();
            comboBox_Stationery = new ComboBox();
            header_Sender = new Label();
            comboBox_Sender = new ComboBox();
            label5 = new Label();
            btn_SaveRAW = new Button();
            btn_GenDotCode = new Button();
            header_Greeting = new Label();
            header_Body = new Label();
            textBox_Body = new TextBox();
            header_Closing = new Label();
            textBox_Closing = new TextBox();
            openFileDialog1 = new OpenFileDialog();
            label9 = new Label();
            label_Greeting = new Label();
            pictureBox_Stationery = new PictureBox();
            label_Line1 = new Label();
            label_Line2 = new Label();
            label_Line3 = new Label();
            label_Line4 = new Label();
            label_Line5 = new Label();
            label_Line6 = new Label();
            label_Closing = new Label();
            comboBox_Greeting = new ComboBox();
            menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox_Stationery).BeginInit();
            SuspendLayout();
            // 
            // comboBox_ItemName
            // 
            comboBox_ItemName.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            comboBox_ItemName.AutoCompleteSource = AutoCompleteSource.ListItems;
            comboBox_ItemName.FormattingEnabled = true;
            comboBox_ItemName.Location = new System.Drawing.Point(116, 551);
            comboBox_ItemName.Margin = new Padding(2);
            comboBox_ItemName.Name = "comboBox_ItemName";
            comboBox_ItemName.Size = new System.Drawing.Size(152, 23);
            comboBox_ItemName.TabIndex = 0;
            comboBox_ItemName.SelectedIndexChanged += ItemNameChanged;
            // 
            // textBox_ItemID
            // 
            textBox_ItemID.CharacterCasing = CharacterCasing.Upper;
            textBox_ItemID.Location = new System.Drawing.Point(43, 551);
            textBox_ItemID.Margin = new Padding(2);
            textBox_ItemID.MaxLength = 4;
            textBox_ItemID.Name = "textBox_ItemID";
            textBox_ItemID.Size = new System.Drawing.Size(69, 23);
            textBox_ItemID.TabIndex = 1;
            textBox_ItemID.TextChanged += ItemIDChanged;
            textBox_ItemID.KeyPress += ItemIDKeyPress;
            textBox_ItemID.KeyUp += ItemIDKeyUp;
            // 
            // header_ItemID
            // 
            header_ItemID.AutoSize = true;
            header_ItemID.Location = new System.Drawing.Point(22, 534);
            header_ItemID.Margin = new Padding(2, 0, 2, 0);
            header_ItemID.Name = "header_ItemID";
            header_ItemID.Size = new System.Drawing.Size(45, 15);
            header_ItemID.TabIndex = 2;
            header_ItemID.Text = "Item ID";
            // 
            // header_ItemName
            // 
            header_ItemName.AutoSize = true;
            header_ItemName.Location = new System.Drawing.Point(113, 534);
            header_ItemName.Margin = new Padding(2, 0, 2, 0);
            header_ItemName.Name = "header_ItemName";
            header_ItemName.Size = new System.Drawing.Size(66, 15);
            header_ItemName.TabIndex = 3;
            header_ItemName.Text = "Item Name";
            // 
            // header_Gift
            // 
            header_Gift.AutoSize = true;
            header_Gift.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            header_Gift.Location = new System.Drawing.Point(19, 505);
            header_Gift.Margin = new Padding(2, 0, 2, 0);
            header_Gift.Name = "header_Gift";
            header_Gift.Size = new System.Drawing.Size(107, 19);
            header_Gift.TabIndex = 4;
            header_Gift.Text = "Attached Gift";
            // 
            // header_LetterText
            // 
            header_LetterText.AutoSize = true;
            header_LetterText.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            header_LetterText.Location = new System.Drawing.Point(19, 44);
            header_LetterText.Margin = new Padding(2, 0, 2, 0);
            header_LetterText.Name = "header_LetterText";
            header_LetterText.Size = new System.Drawing.Size(89, 19);
            header_LetterText.TabIndex = 6;
            header_LetterText.Text = "Letter Text";
            // 
            // menuStrip1
            // 
            menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            menuStrip1.Items.AddRange(new ToolStripItem[] { fileToolStripMenuItem1, editToolStripMenuItem1, menu_View, menu_Help });
            menuStrip1.Location = new System.Drawing.Point(0, 0);
            menuStrip1.Name = "menuStrip1";
            menuStrip1.Padding = new Padding(5, 2, 0, 2);
            menuStrip1.Size = new System.Drawing.Size(869, 24);
            menuStrip1.TabIndex = 8;
            menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem1
            // 
            fileToolStripMenuItem1.DropDownItems.AddRange(new ToolStripItem[] { menu_FileNew, menu_FileOpen });
            fileToolStripMenuItem1.Name = "fileToolStripMenuItem1";
            fileToolStripMenuItem1.Size = new System.Drawing.Size(37, 20);
            fileToolStripMenuItem1.Text = "File";
            // 
            // menu_FileNew
            // 
            menu_FileNew.DropDownItems.AddRange(new ToolStripItem[] { menu_NewCharCard });
            menu_FileNew.Name = "menu_FileNew";
            menu_FileNew.Size = new System.Drawing.Size(103, 22);
            menu_FileNew.Text = "New";
            // 
            // menu_NewCharCard
            // 
            menu_NewCharCard.Name = "menu_NewCharCard";
            menu_NewCharCard.Size = new System.Drawing.Size(180, 22);
            menu_NewCharCard.Text = "Character Card (AC)";
            menu_NewCharCard.Click += MenuNewCard;
            // 
            // menu_FileOpen
            // 
            menu_FileOpen.DropDownItems.AddRange(new ToolStripItem[] { menu_OpenCharCard });
            menu_FileOpen.Name = "menu_FileOpen";
            menu_FileOpen.Size = new System.Drawing.Size(103, 22);
            menu_FileOpen.Text = "Open";
            // 
            // menu_OpenCharCard
            // 
            menu_OpenCharCard.Name = "menu_OpenCharCard";
            menu_OpenCharCard.Size = new System.Drawing.Size(180, 22);
            menu_OpenCharCard.Text = "Character Card (AC)";
            menu_OpenCharCard.Click += OpenCharCard;
            // 
            // editToolStripMenuItem1
            // 
            editToolStripMenuItem1.DropDownItems.AddRange(new ToolStripItem[] { menu_ClearInputs, menu_OpenFileDir });
            editToolStripMenuItem1.Name = "editToolStripMenuItem1";
            editToolStripMenuItem1.Size = new System.Drawing.Size(39, 20);
            editToolStripMenuItem1.Text = "Edit";
            // 
            // menu_ClearInputs
            // 
            menu_ClearInputs.Name = "menu_ClearInputs";
            menu_ClearInputs.Size = new System.Drawing.Size(175, 22);
            menu_ClearInputs.Text = "Clear Inputs";
            menu_ClearInputs.Click += MenuClearValues;
            // 
            // menu_OpenFileDir
            // 
            menu_OpenFileDir.Name = "menu_OpenFileDir";
            menu_OpenFileDir.Size = new System.Drawing.Size(175, 22);
            menu_OpenFileDir.Text = "Open File Directory";
            menu_OpenFileDir.Click += OpenDecompressedFileDir;
            // 
            // menu_View
            // 
            menu_View.DropDownItems.AddRange(new ToolStripItem[] { menu_ToggleDarkMode });
            menu_View.Name = "menu_View";
            menu_View.Size = new System.Drawing.Size(44, 20);
            menu_View.Text = "View";
            // 
            // menu_ToggleDarkMode
            // 
            menu_ToggleDarkMode.Name = "menu_ToggleDarkMode";
            menu_ToggleDarkMode.Size = new System.Drawing.Size(170, 22);
            menu_ToggleDarkMode.Text = "Toggle Dark Mode";
            menu_ToggleDarkMode.Click += ToggleDarkMode;
            // 
            // menu_Help
            // 
            menu_Help.DropDownItems.AddRange(new ToolStripItem[] { menu_GitRepo, menu_About });
            menu_Help.Name = "menu_Help";
            menu_Help.Size = new System.Drawing.Size(44, 20);
            menu_Help.Text = "Help";
            // 
            // menu_GitRepo
            // 
            menu_GitRepo.Name = "menu_GitRepo";
            menu_GitRepo.Size = new System.Drawing.Size(142, 22);
            menu_GitRepo.Text = "GitHub Repo";
            menu_GitRepo.Click += GitRepoClick;
            // 
            // menu_About
            // 
            menu_About.Name = "menu_About";
            menu_About.Size = new System.Drawing.Size(142, 22);
            menu_About.Text = "About";
            menu_About.Click += AboutClick;
            // 
            // fileToolStripMenuItem
            // 
            fileToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { saveToolStripMenuItem, openToolStripMenuItem, saveToolStripMenuItem1, saveAsToolStripMenuItem });
            fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            fileToolStripMenuItem.Size = new System.Drawing.Size(54, 29);
            fileToolStripMenuItem.Text = "File";
            // 
            // saveToolStripMenuItem
            // 
            saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            saveToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            saveToolStripMenuItem.Text = "New";
            // 
            // openToolStripMenuItem
            // 
            openToolStripMenuItem.Name = "openToolStripMenuItem";
            openToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            openToolStripMenuItem.Text = "Open";
            // 
            // saveToolStripMenuItem1
            // 
            saveToolStripMenuItem1.Name = "saveToolStripMenuItem1";
            saveToolStripMenuItem1.Size = new System.Drawing.Size(123, 22);
            saveToolStripMenuItem1.Text = "Save";
            // 
            // saveAsToolStripMenuItem
            // 
            saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            saveAsToolStripMenuItem.Size = new System.Drawing.Size(123, 22);
            saveAsToolStripMenuItem.Text = "Save As...";
            // 
            // editToolStripMenuItem
            // 
            editToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { settingsToolStripMenuItem, resetToolStripMenuItem });
            editToolStripMenuItem.Name = "editToolStripMenuItem";
            editToolStripMenuItem.Size = new System.Drawing.Size(58, 29);
            editToolStripMenuItem.Text = "Edit";
            // 
            // settingsToolStripMenuItem
            // 
            settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";
            settingsToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            settingsToolStripMenuItem.Text = "Settings";
            // 
            // resetToolStripMenuItem
            // 
            resetToolStripMenuItem.Name = "resetToolStripMenuItem";
            resetToolStripMenuItem.Size = new System.Drawing.Size(130, 22);
            resetToolStripMenuItem.Text = "Reset Card";
            // 
            // helpToolStripMenuItem
            // 
            helpToolStripMenuItem.DropDownItems.AddRange(new ToolStripItem[] { tutorialToolStripMenuItem, videoTutorialToolStripMenuItem, gitHubRepositoryToolStripMenuItem });
            helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            helpToolStripMenuItem.Size = new System.Drawing.Size(65, 29);
            helpToolStripMenuItem.Text = "Help";
            // 
            // tutorialToolStripMenuItem
            // 
            tutorialToolStripMenuItem.Name = "tutorialToolStripMenuItem";
            tutorialToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            tutorialToolStripMenuItem.Text = "Tutorial";
            // 
            // videoTutorialToolStripMenuItem
            // 
            videoTutorialToolStripMenuItem.Name = "videoTutorialToolStripMenuItem";
            videoTutorialToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            videoTutorialToolStripMenuItem.Text = "Video Tutorial";
            // 
            // gitHubRepositoryToolStripMenuItem
            // 
            gitHubRepositoryToolStripMenuItem.Name = "gitHubRepositoryToolStripMenuItem";
            gitHubRepositoryToolStripMenuItem.Size = new System.Drawing.Size(171, 22);
            gitHubRepositoryToolStripMenuItem.Text = "GitHub Repository";
            // 
            // label_Version
            // 
            label_Version.Anchor = AnchorStyles.Bottom | AnchorStyles.Right;
            label_Version.AutoSize = true;
            label_Version.Font = new System.Drawing.Font("Courier New", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Version.Location = new System.Drawing.Point(791, 752);
            label_Version.Margin = new Padding(2, 0, 2, 0);
            label_Version.Name = "label_Version";
            label_Version.Size = new System.Drawing.Size(0, 14);
            label_Version.TabIndex = 9;
            // 
            // header_Stationery
            // 
            header_Stationery.AutoSize = true;
            header_Stationery.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            header_Stationery.Location = new System.Drawing.Point(19, 358);
            header_Stationery.Margin = new Padding(2, 0, 2, 0);
            header_Stationery.Name = "header_Stationery";
            header_Stationery.Size = new System.Drawing.Size(87, 19);
            header_Stationery.TabIndex = 12;
            header_Stationery.Text = "Stationery";
            // 
            // comboBox_Stationery
            // 
            comboBox_Stationery.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            comboBox_Stationery.AutoCompleteSource = AutoCompleteSource.ListItems;
            comboBox_Stationery.FormattingEnabled = true;
            comboBox_Stationery.Location = new System.Drawing.Point(24, 382);
            comboBox_Stationery.Margin = new Padding(2);
            comboBox_Stationery.Name = "comboBox_Stationery";
            comboBox_Stationery.Size = new System.Drawing.Size(244, 23);
            comboBox_Stationery.TabIndex = 13;
            comboBox_Stationery.SelectedIndexChanged += StationeryChanged;
            // 
            // header_Sender
            // 
            header_Sender.AutoSize = true;
            header_Sender.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            header_Sender.Location = new System.Drawing.Point(19, 427);
            header_Sender.Margin = new Padding(2, 0, 2, 0);
            header_Sender.Name = "header_Sender";
            header_Sender.Size = new System.Drawing.Size(64, 19);
            header_Sender.TabIndex = 14;
            header_Sender.Text = "Sender";
            // 
            // comboBox_Sender
            // 
            comboBox_Sender.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            comboBox_Sender.AutoCompleteSource = AutoCompleteSource.ListItems;
            comboBox_Sender.FormattingEnabled = true;
            comboBox_Sender.Location = new System.Drawing.Point(24, 452);
            comboBox_Sender.Margin = new Padding(2);
            comboBox_Sender.Name = "comboBox_Sender";
            comboBox_Sender.Size = new System.Drawing.Size(244, 23);
            comboBox_Sender.TabIndex = 15;
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Font = new System.Drawing.Font("Arial", 12F, System.Drawing.FontStyle.Bold);
            label5.Location = new System.Drawing.Point(299, 44);
            label5.Margin = new Padding(2, 0, 2, 0);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(70, 19);
            label5.TabIndex = 16;
            label5.Text = "Preview";
            // 
            // btn_SaveRAW
            // 
            btn_SaveRAW.BackColor = System.Drawing.Color.White;
            btn_SaveRAW.Location = new System.Drawing.Point(735, 543);
            btn_SaveRAW.Margin = new Padding(2);
            btn_SaveRAW.Name = "btn_SaveRAW";
            btn_SaveRAW.Size = new System.Drawing.Size(107, 28);
            btn_SaveRAW.TabIndex = 17;
            btn_SaveRAW.Text = "Save .RAW";
            btn_SaveRAW.UseVisualStyleBackColor = false;
            btn_SaveRAW.Click += SaveRAW;
            // 
            // btn_GenDotCode
            // 
            btn_GenDotCode.BackColor = System.Drawing.Color.White;
            btn_GenDotCode.Location = new System.Drawing.Point(580, 543);
            btn_GenDotCode.Margin = new Padding(2);
            btn_GenDotCode.Name = "btn_GenDotCode";
            btn_GenDotCode.Size = new System.Drawing.Size(150, 28);
            btn_GenDotCode.TabIndex = 18;
            btn_GenDotCode.Text = "Generate Dot Code";
            btn_GenDotCode.UseVisualStyleBackColor = false;
            btn_GenDotCode.Click += GenerateDotCodeClick;
            // 
            // header_Greeting
            // 
            header_Greeting.AutoSize = true;
            header_Greeting.Location = new System.Drawing.Point(20, 74);
            header_Greeting.Margin = new Padding(2, 0, 2, 0);
            header_Greeting.Name = "header_Greeting";
            header_Greeting.Size = new System.Drawing.Size(52, 15);
            header_Greeting.TabIndex = 19;
            header_Greeting.Text = "Greeting";
            // 
            // header_Body
            // 
            header_Body.AutoSize = true;
            header_Body.Location = new System.Drawing.Point(20, 128);
            header_Body.Margin = new Padding(2, 0, 2, 0);
            header_Body.Name = "header_Body";
            header_Body.Size = new System.Drawing.Size(34, 15);
            header_Body.TabIndex = 20;
            header_Body.Text = "Body";
            // 
            // textBox_Body
            // 
            textBox_Body.Location = new System.Drawing.Point(23, 145);
            textBox_Body.Margin = new Padding(2);
            textBox_Body.MaxLength = 180;
            textBox_Body.Multiline = true;
            textBox_Body.Name = "textBox_Body";
            textBox_Body.Size = new System.Drawing.Size(244, 142);
            textBox_Body.TabIndex = 21;
            textBox_Body.TextChanged += LetterBodyChanged;
            textBox_Body.KeyPress += LetterBodyKeyPress;
            // 
            // header_Closing
            // 
            header_Closing.AutoSize = true;
            header_Closing.Location = new System.Drawing.Point(22, 299);
            header_Closing.Margin = new Padding(2, 0, 2, 0);
            header_Closing.Name = "header_Closing";
            header_Closing.Size = new System.Drawing.Size(47, 15);
            header_Closing.TabIndex = 22;
            header_Closing.Text = "Closing";
            // 
            // textBox_Closing
            // 
            textBox_Closing.Location = new System.Drawing.Point(24, 316);
            textBox_Closing.Margin = new Padding(2);
            textBox_Closing.Name = "textBox_Closing";
            textBox_Closing.Size = new System.Drawing.Size(244, 23);
            textBox_Closing.TabIndex = 23;
            textBox_Closing.TextChanged += ClosingChanged;
            // 
            // openFileDialog1
            // 
            openFileDialog1.Filter = "RAW Files (*raw)|*raw";
            openFileDialog1.Title = "Open e-Reader Card RAW File";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new System.Drawing.Point(22, 556);
            label9.Margin = new Padding(2, 0, 2, 0);
            label9.Name = "label9";
            label9.Size = new System.Drawing.Size(19, 15);
            label9.TabIndex = 25;
            label9.Text = "0x";
            // 
            // label_Greeting
            // 
            label_Greeting.AutoSize = true;
            label_Greeting.BackColor = System.Drawing.Color.Transparent;
            label_Greeting.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Greeting.Location = new System.Drawing.Point(60, 43);
            label_Greeting.Margin = new Padding(2, 0, 2, 0);
            label_Greeting.Name = "label_Greeting";
            label_Greeting.Size = new System.Drawing.Size(0, 24);
            label_Greeting.TabIndex = 27;
            // 
            // pictureBox_Stationery
            // 
            pictureBox_Stationery.BackgroundImage = Properties.Resources.Airmail_Paper_PG;
            pictureBox_Stationery.BackgroundImageLayout = ImageLayout.Stretch;
            pictureBox_Stationery.Location = new System.Drawing.Point(303, 73);
            pictureBox_Stationery.Margin = new Padding(2);
            pictureBox_Stationery.Name = "pictureBox_Stationery";
            pictureBox_Stationery.Size = new System.Drawing.Size(540, 454);
            pictureBox_Stationery.TabIndex = 10;
            pictureBox_Stationery.TabStop = false;
            // 
            // label_Line1
            // 
            label_Line1.BackColor = System.Drawing.Color.Transparent;
            label_Line1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line1.Location = new System.Drawing.Point(60, 114);
            label_Line1.Margin = new Padding(2, 0, 2, 0);
            label_Line1.Name = "label_Line1";
            label_Line1.Size = new System.Drawing.Size(414, 40);
            label_Line1.TabIndex = 28;
            // 
            // label_Line2
            // 
            label_Line2.BackColor = System.Drawing.Color.Transparent;
            label_Line2.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line2.Location = new System.Drawing.Point(60, 154);
            label_Line2.Margin = new Padding(2, 0, 2, 0);
            label_Line2.Name = "label_Line2";
            label_Line2.Size = new System.Drawing.Size(414, 42);
            label_Line2.TabIndex = 29;
            // 
            // label_Line3
            // 
            label_Line3.BackColor = System.Drawing.Color.Transparent;
            label_Line3.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line3.Location = new System.Drawing.Point(60, 196);
            label_Line3.Margin = new Padding(2, 0, 2, 0);
            label_Line3.Name = "label_Line3";
            label_Line3.Size = new System.Drawing.Size(414, 38);
            label_Line3.TabIndex = 30;
            // 
            // label_Line4
            // 
            label_Line4.BackColor = System.Drawing.Color.Transparent;
            label_Line4.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line4.Location = new System.Drawing.Point(60, 234);
            label_Line4.Margin = new Padding(2, 0, 2, 0);
            label_Line4.Name = "label_Line4";
            label_Line4.Size = new System.Drawing.Size(414, 40);
            label_Line4.TabIndex = 31;
            // 
            // label_Line5
            // 
            label_Line5.BackColor = System.Drawing.Color.Transparent;
            label_Line5.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line5.Location = new System.Drawing.Point(60, 274);
            label_Line5.Margin = new Padding(2, 0, 2, 0);
            label_Line5.Name = "label_Line5";
            label_Line5.Size = new System.Drawing.Size(414, 38);
            label_Line5.TabIndex = 32;
            // 
            // label_Line6
            // 
            label_Line6.BackColor = System.Drawing.Color.Transparent;
            label_Line6.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Line6.Location = new System.Drawing.Point(60, 316);
            label_Line6.Margin = new Padding(2, 0, 2, 0);
            label_Line6.Name = "label_Line6";
            label_Line6.Size = new System.Drawing.Size(416, 46);
            label_Line6.TabIndex = 33;
            // 
            // label_Closing
            // 
            label_Closing.BackColor = System.Drawing.Color.Transparent;
            label_Closing.Font = new System.Drawing.Font("Microsoft Sans Serif", 14F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            label_Closing.Location = new System.Drawing.Point(60, 362);
            label_Closing.Margin = new Padding(2, 0, 2, 0);
            label_Closing.Name = "label_Closing";
            label_Closing.Size = new System.Drawing.Size(419, 46);
            label_Closing.TabIndex = 34;
            label_Closing.TextAlign = System.Drawing.ContentAlignment.BottomRight;
            // 
            // comboBox_Greeting
            // 
            comboBox_Greeting.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            comboBox_Greeting.AutoCompleteSource = AutoCompleteSource.ListItems;
            comboBox_Greeting.FormattingEnabled = true;
            comboBox_Greeting.Location = new System.Drawing.Point(23, 91);
            comboBox_Greeting.Margin = new Padding(2);
            comboBox_Greeting.Name = "comboBox_Greeting";
            comboBox_Greeting.Size = new System.Drawing.Size(244, 23);
            comboBox_Greeting.TabIndex = 35;
            comboBox_Greeting.SelectedIndexChanged += GreetingChanged;
            // 
            // eReaderCCC
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(869, 583);
            Controls.Add(label_Line3);
            Controls.Add(label_Line1);
            Controls.Add(comboBox_Greeting);
            Controls.Add(label_Closing);
            Controls.Add(label_Line6);
            Controls.Add(label_Line5);
            Controls.Add(label_Line4);
            Controls.Add(label_Line2);
            Controls.Add(label_Greeting);
            Controls.Add(label9);
            Controls.Add(textBox_Closing);
            Controls.Add(header_Closing);
            Controls.Add(textBox_Body);
            Controls.Add(header_Body);
            Controls.Add(header_Greeting);
            Controls.Add(btn_GenDotCode);
            Controls.Add(btn_SaveRAW);
            Controls.Add(label5);
            Controls.Add(comboBox_Sender);
            Controls.Add(header_Sender);
            Controls.Add(comboBox_Stationery);
            Controls.Add(header_Stationery);
            Controls.Add(label_Version);
            Controls.Add(header_LetterText);
            Controls.Add(header_Gift);
            Controls.Add(header_ItemName);
            Controls.Add(header_ItemID);
            Controls.Add(textBox_ItemID);
            Controls.Add(comboBox_ItemName);
            Controls.Add(menuStrip1);
            Controls.Add(pictureBox_Stationery);
            FormBorderStyle = FormBorderStyle.Fixed3D;
            Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
            MainMenuStrip = menuStrip1;
            Margin = new Padding(2);
            Name = "eReaderCCC";
            Text = "AC e-Reader Character Card Creator";
            menuStrip1.ResumeLayout(false);
            menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)pictureBox_Stationery).EndInit();
            ResumeLayout(false);
            PerformLayout();
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
        private ToolStripMenuItem menu_Help;
        private ToolStripMenuItem menu_GitRepo;
        private ToolStripMenuItem menu_About;
        private ToolStripMenuItem menu_View;
        private ToolStripMenuItem menu_ToggleDarkMode;
    }
}
