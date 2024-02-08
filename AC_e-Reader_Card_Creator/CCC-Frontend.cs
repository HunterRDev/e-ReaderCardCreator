using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using System.IO;
using AC_e_Reader_Card_Creator.Decompression.Functions;
using System.Drawing.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using AC_e_Reader_Card_Creator.References;

namespace AC_e_Reader_Card_Creator
{
    public partial class eReaderCCC : Form
    {
        private readonly Dictionary<string, string> itemIDs = [];
        private readonly Dictionary<string, int[]> stationeryFontRGB = [];
        private readonly PrivateFontCollection AC_Letter_Font = new();
        private bool isDarkModeEnabled = false;
        private PreferencesManager preferencesManager = new PreferencesManager("e-ReaderCardCreator");

        public eReaderCCC()
        {
            InitializeComponent();
            LoadDirectories();
            LoadStationeryIDs();
            LoadItemIDs();
            LoadSenderIDs();
            LoadGreetings();
            HandleFonts();
            Common.LoadBlankVPKs();

            label_Version.Text = Common.VERSION;
            comboBox_Stationery.SelectedIndex = 0;
            comboBox_Greeting.SelectedIndex = 5;
            label_Greeting.Text = comboBox_Greeting.Text;

            isDarkModeEnabled = preferencesManager.ReadDarkModePreference();
            ApplyTheme();
        }

        private void LoadDirectories()
        {
            string[] directoryPath = [@"Project Files\Decompression\eCard", Common.RAW_OUTPUT, Common.BIN_OUTPUT, Common.VPK_OUTPUT, Common.DEC_OUTPUT, Common.BMP_OUTPUT];
            foreach (string dir in directoryPath)
            {
                if (!Directory.Exists(dir))
                {
                    Directory.CreateDirectory(dir);
                }
            }
        }

        private void ItemIDChanged(object sender, EventArgs e)
        {
            string itemID = "0x" + textBox_ItemID.Text;

            if (itemID.Length == 6)
            {
                string item_name = Common.LookupListValue(itemID, Common.ITEM_LIST);

                if (item_name != null)
                {
                    comboBox_ItemName.SelectedItem = item_name;
                }
                else
                {
                    comboBox_ItemName.SelectedItem = "Invalid";
                }
            }
            else
            {
                comboBox_ItemName.SelectedItem = null;
            }
        }

        private void LetterBodyChanged(object sender, EventArgs e)
        {
            int currentCharCount = textBox_Body.Text.Length;
            int maxCharBody = textBox_Body.MaxLength;
            header_Body.Text = $"Body  ( {currentCharCount} / {maxCharBody} )";

            List<Label> letter_labels =
            [
                label_Greeting,
                label_Line1,
                label_Line2,
                label_Line3,
                label_Line4,
                label_Line5,
                label_Line6,
                label_Closing
            ];
            int[] fontColor = stationeryFontRGB[comboBox_Stationery.Text];
            Common.HandleLetterBody(letter_labels, textBox_Body, fontColor);
        }

        private void LetterBodyKeyPress(object sender, KeyPressEventArgs e)
        {
            if (textBox_Body.Text != null)
            {
                int newlineCount = textBox_Body.Text.Count(c => c == '\n');
                int maxNewlines = 5;

                if (newlineCount >= maxNewlines && e.KeyChar == (char)Keys.Enter)
                {
                    e.Handled = true;
                }

                List<Label> letter_labels = [
                    label_Greeting,
                    label_Line1,
                    label_Line2,
                    label_Line3,
                    label_Line4,
                    label_Line5,
                    label_Line6,
                    label_Closing
                ];
                int[] fontColor = stationeryFontRGB[comboBox_Stationery.Text];
                Common.HandleLetterBody(letter_labels, textBox_Body, fontColor);
            }
        }

        private void ItemNameChanged(object sender, EventArgs e)
        {
            if (comboBox_ItemName.SelectedIndex != -1)
            {
                string selected_name = comboBox_ItemName.SelectedItem.ToString();
                if (selected_name != "Invalid")
                {
                    if (itemIDs.TryGetValue(selected_name, out string id))
                    {
                        textBox_ItemID.Text = id[2..];
                    }
                }
            }
        }

        private void ItemIDKeyPress(object sender, KeyPressEventArgs e)
        {
            char ch = char.ToUpper(e.KeyChar);

            if (!((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || char.IsControl(e.KeyChar)))
            {
                e.Handled = true;
            }
        }

        private void ItemIDKeyUp(object sender, KeyEventArgs e)
        {
            string itemID = "0x" + textBox_ItemID.Text;

            if (itemID.Length == 6)
            {
                string item_name = Common.LookupListValue(itemID, Common.ITEM_LIST);

                if (item_name != null)
                {
                    comboBox_ItemName.SelectedItem = item_name;
                }
                else
                {
                    comboBox_ItemName.SelectedItem = "Invalid";
                }
            }
            else
            {
                comboBox_ItemName.SelectedItem = null;
            }
        }

        private void LoadStationeryIDs()
        {
            try
            {
                foreach (string line in File.ReadAllLines(Common.STATIONERY_LIST))
                {
                    string[] stationery = line.Split(',');
                    if (stationery.Length > 1)
                    {
                        string stationery_name = stationery[1].Trim();
                        comboBox_Stationery.Items.Add(stationery_name);
                        string[] stationery_RGB_s = stationery[2].Split('-');

                        List<int> stationery_RGB_i = [];

                        for (int i = 0; i < stationery_RGB_s.Length; i++)
                        {
                            stationery_RGB_i.Add(int.Parse(stationery_RGB_s[i]));
                        }

                        stationeryFontRGB[stationery_name] = [.. stationery_RGB_i];
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error reading file: " + ex.Message);
            }
        }

        private void LoadSenderIDs()
        {
            try
            {
                foreach (string line in File.ReadAllLines(Common.SENDER_LIST))
                {
                    string[] senders = line.Split(',');
                    if (senders.Length > 1)
                    {
                        string sender_name = senders[1].Trim();
                        comboBox_Sender.Items.Add(sender_name);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error reading file: " + ex.Message);
            }

        }

        private void LoadGreetings()
        {
            try
            {
                foreach (string line in File.ReadAllLines(Common.GREETING_LIST))
                {
                    comboBox_Greeting.Items.Add(line);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error reading file: " + ex.Message);
            }

        }

        private void LoadItemIDs()
        {
            try
            {
                foreach (string line in File.ReadAllLines(Common.ITEM_LIST))
                {
                    string[] ac_items = line.Split(',');
                    if (ac_items.Length > 1)
                    {
                        string id = ac_items[0].Trim();
                        string item_name = ac_items[1].Trim();

                        itemIDs[item_name] = id;
                        comboBox_ItemName.Items.Add(item_name);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error reading file: " + ex.Message);
            }
        }

        private void StationeryChanged(object sender, EventArgs e)
        {
            if (comboBox_Stationery.SelectedItem != null)
            {
                string selectedStationery = comboBox_Stationery.SelectedItem.ToString();
                string stationeryImageName = selectedStationery.Replace(" ", "_") + "_PG";

                var stationeryImage = (Image)Properties.Resources.ResourceManager.GetObject(stationeryImageName);

                if (stationeryImage != null)
                {
                    pictureBox_Stationery.BackgroundImage = stationeryImage;
                }

                List<Label> letter_labels = [
                    label_Greeting,
                    label_Line1,
                    label_Line2,
                    label_Line3,
                    label_Line4,
                    label_Line5,
                    label_Line6,
                    label_Closing
                ];
                int[] fontColor = stationeryFontRGB[comboBox_Stationery.Text];
                foreach (Label letter_line in letter_labels)
                {
                    letter_line.ForeColor = System.Drawing.Color.FromArgb(fontColor[0], fontColor[1], fontColor[2]);
                }
            }
        }

        private void GreetingChanged(object sender, EventArgs e)
        {
            label_Greeting.Text = comboBox_Greeting.Text;
        }
        private void ClosingChanged(object sender, EventArgs e)
        {
            label_Closing.Text = textBox_Closing.Text;
        }

        private void SaveRAW(object sender, EventArgs e)
        {
            if (ValidInputs())
            {
                bool customFilePath = true;
                Compress.DECtoVPK(comboBox_Greeting.Text, textBox_Body.Text, textBox_Closing.Text, comboBox_Stationery.Text, comboBox_Sender.Text, textBox_ItemID.Text);
                Compress.VPKtoBIN();
                Compress.BINtoRAW(customFilePath);
            }
        }

        private void HandleFonts()
        {
            List<Label> letter_labels =
            [
                label_Greeting,
                label_Line1,
                label_Line2,
                label_Line3,
                label_Line4,
                label_Line5,
                label_Line6,
                label_Closing
            ];

            byte[] fontData = Properties.Resources.FOT_Rodin_Pro_M;
            IntPtr fontPtr = Marshal.AllocCoTaskMem(fontData.Length);
            Marshal.Copy(fontData, 0, fontPtr, fontData.Length);
            AC_Letter_Font.AddMemoryFont(fontPtr, fontData.Length);
            Marshal.FreeCoTaskMem(fontPtr);

            Font AC_Font = new(AC_Letter_Font.Families[0], 14, FontStyle.Bold);

            for (int i = 0; i < letter_labels.Count; i++)
            {
                letter_labels[i].Parent = pictureBox_Stationery;
                letter_labels[i].Font = AC_Font;
            }
        }

        private bool ValidInputs()
        {
            if (comboBox_ItemName.Text == "Invalid" || comboBox_ItemName.Text == null)
            {
                DialogResult dialogResult = MessageBox.Show("The selected item ID is outside of Animal Crossing's valid range. Would you still like to continue?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

                if (dialogResult == DialogResult.Yes)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                TextBox[] textBoxes = [textBox_Body, textBox_Closing, textBox_ItemID];
                ComboBox[] comboBoxes = [comboBox_Greeting, comboBox_ItemName, comboBox_Sender, comboBox_Stationery];

                foreach (TextBox tb in textBoxes)
                {
                    if (string.IsNullOrWhiteSpace(tb.Text))
                    {
                        MessageBox.Show("Please fill in all text fields.", "Missing data!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        tb.Focus();
                        return false;
                    }
                }
                foreach (ComboBox cb in comboBoxes)
                {
                    if (cb.SelectedItem == null || string.IsNullOrWhiteSpace(cb.Text))
                    {
                        MessageBox.Show("Please make a selection in all dropdown menus.", "Missing data!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        cb.Focus();
                        return false;
                    }
                }

                return true;
            }
        }

        private void MenuClearValues(object sender, EventArgs e)
        {
            comboBox_Greeting.Text = null;
            textBox_Body.Text = null;
            textBox_Closing.Text = null;
            comboBox_Stationery.SelectedIndex = 0;
            comboBox_Sender.Text = null;
            comboBox_ItemName.SelectedIndex = 0;
        }

        private void MenuNewCard(object sender, EventArgs e)
        {
            comboBox_Greeting.Text = null;
            textBox_Body.Text = null;
            textBox_Closing.Text = null;
            comboBox_Stationery.SelectedIndex = 0;
            comboBox_Sender.Text = null;
            comboBox_ItemName.SelectedIndex = 0;

            ClearFiles();
            Common.LoadBlankVPKs();
        }

        private void ClearFiles()
        {
            try
            {
                string[] fileDirectories = [Common.BIN_OUTPUT, Common.VPK_OUTPUT, Common.DEC_OUTPUT];

                foreach (string dir in fileDirectories)
                {
                    if (Directory.Exists(dir))
                    {
                        string[] files = Directory.GetFiles(dir);
                        foreach (string file in files)
                        {
                            File.Delete(file);
                        }
                    }
                    else
                    {
                        MessageBox.Show($"The specified directory does not exist: {dir}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }


        private void OpenCharCard(object sender, EventArgs e)
        {
            ClearFiles();

            try
            {
                if (openFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    string opened_file_path = openFileDialog1.FileName;

                    Decompress.RAWtoBIN(opened_file_path);
                    Decompress.BINtoVPK(Common.COMPRESSED_BIN, Common.VPK_DELIMITER, Common.VPK_OUTPUT);
                    Decompress.VPK_Decompress();

                    Dictionary<string, string> letterData = Decompressed.GetData();
                    List<Label> letter_labels = [
                    label_Greeting,
                        label_Line1,
                        label_Line2,
                        label_Line3,
                        label_Line4,
                        label_Line5,
                        label_Line6,
                        label_Closing
                    ];

                    string[] delimiter = ["\r\n"];
                    string[] split_letter_body = letterData["letter_body"].Split(delimiter, StringSplitOptions.None);
                    string stationeryID = "0x" + letterData["letter_stationery"];
                    string letter_stationery = Common.LookupListValue(stationeryID, Common.STATIONERY_LIST);
                    string senderID = "0x" + letterData["letter_sender"];
                    string letter_sender = Common.LookupListValue(senderID, Common.SENDER_LIST);
                    string itemID = letterData["letter_gift"];
                    int line_index = 1;

                    comboBox_Greeting.SelectedItem = letterData["letter_greeting"];

                    // bit of a hack to appropriately display newlines without ruining the size of the letter body
                    textBox_Body.Text = letterData["letter_body"].Replace("\n", "\r\n").Trim();
                    textBox_Closing.Text = letterData["letter_closing"];
                    letter_labels[0].Text = letterData["letter_greeting"];


                    foreach (string line in split_letter_body)
                    {
                        letter_labels[line_index].Text = line;
                        line_index++;
                    }
                    letter_labels[7].Text = letterData["letter_closing"];


                    if (comboBox_Stationery.Items.Contains(letter_stationery))
                    {
                        comboBox_Stationery.SelectedItem = letter_stationery;
                    }
                    else
                    {
                        MessageBox.Show($"Stationery '{letter_stationery}' not found.");
                    }


                    if (comboBox_Sender.Items.Contains(letter_sender))
                    {
                        comboBox_Sender.SelectedItem = letter_sender;
                    }
                    else
                    {
                        MessageBox.Show($"Sender '{letter_sender}' not found.");
                    }

                    textBox_ItemID.Text = itemID;
                }
            }
            catch
            {
                MessageBox.Show("Unable to open e-Card! Please make sure this is an Animal Crossing e-Reader Character Card or Classic Game Card.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void OpenDecompressedFileDir(object sender, EventArgs e)
        {
            if (Directory.Exists(Common.DECOMPRESSED_DIR))
            {
                Process.Start("explorer.exe", Common.DECOMPRESSED_DIR);
            }
            else
            {
                MessageBox.Show($"The directory does not exist: {Common.DECOMPRESSED_DIR}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void GenerateDotCodeClick(object sender, EventArgs e)
        {
            if (ValidInputs())
            {
                bool customFilePath = false;
                Compress.DECtoVPK(comboBox_Greeting.Text, textBox_Body.Text, textBox_Closing.Text, comboBox_Stationery.Text, comboBox_Sender.Text, textBox_ItemID.Text);
                Compress.VPKtoBIN();
                Compress.BINtoRAW(customFilePath);

                Print_Frontend printer_form = new()
                {
                    StartPosition = FormStartPosition.Manual
                };

                int centerX = Location.X + (Width - printer_form.Width) / 2;
                int centerY = Location.Y + (Height - printer_form.Height) / 2;
                centerX = Math.Max(centerX, Screen.GetWorkingArea(this).Left);
                centerY = Math.Max(centerY, Screen.GetWorkingArea(this).Top);

                printer_form.Location = new Point(centerX, centerY);
                printer_form.ShowDialog();
            }
        }

        private void GitRepoClick(object sender, EventArgs e)
        {
            string URL = "https://github.com/Hunter-Raff/e-ReaderCardCreator";
            try
            {
                Process.Start(
                    new ProcessStartInfo
                    {
                        FileName = URL,
                        UseShellExecute = true
                    }
                );
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to open web browser: {ex.Message}");
            }
        }

        private void AboutClick(object sender, EventArgs e)
        {
            MessageBox.Show(Common.CREDIT, "e-Reader Character Card Creator");
        }

        private void ToggleDarkMode(object sender, EventArgs e)
        {
            isDarkModeEnabled = !isDarkModeEnabled;
            ApplyTheme();
            preferencesManager.SaveDarkModePreference(isDarkModeEnabled);
        }

        private void ApplyTheme()
        {
            BackColor = isDarkModeEnabled ? preferencesManager.DarkModeBackColor : SystemColors.Control;
            preferencesManager.ApplyThemeToControls(Controls, isDarkModeEnabled);
        }

    private void SaveDarkModePreference(bool darkModeEnabled)
        {
            preferencesManager.SaveDarkModePreference(darkModeEnabled);
        }

        private bool ReadDarkModePreference()
        {
            return preferencesManager.ReadDarkModePreference();
        }

    }
}
