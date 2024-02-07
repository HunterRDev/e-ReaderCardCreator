using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Printing;
using System.Drawing.Text;
using System.IO;
using System.Windows.Forms;
using AC_e_Reader_Card_Creator.References;

namespace AC_e_Reader_Card_Creator
{
    public partial class Print_Frontend : Form
    {
        private bool isDarkModeEnabled;

        public Print_Frontend()
        {
            InitializeComponent();
            PopulatePrintersComboBox();

            comboBox_DPI.SelectedIndex = 1; // 600 DPI default

            isDarkModeEnabled = ReadDarkModePreference();
            ApplyTheme();
        }

        private void ApplyTheme()
        {
            PrintFrontend.ApplyTheme(this, isDarkModeEnabled);
        }

        private void ToggleDarkMode(object sender, EventArgs e)
        {
            isDarkModeEnabled = !isDarkModeEnabled;

            ApplyTheme();

            SaveDarkModePreference(isDarkModeEnabled);

            PrintFrontend.ApplyTheme(this, isDarkModeEnabled);
        }

        private bool ReadDarkModePreference()
        {
            string appDataFolderPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "AC e-Reader Card Creator");
            string preferenceFilePath = Path.Combine(appDataFolderPath, "DarkModePreference.txt");

            if (File.Exists(preferenceFilePath))
            {
                if (bool.TryParse(File.ReadAllText(preferenceFilePath), out bool darkModePreference))
                {
                    return darkModePreference;
                }
            }

            return false;
        }

        private void SaveDarkModePreference(bool darkModeEnabled)
        {
            string appDataFolderPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "AC e-Reader Card Creator");

            if (!Directory.Exists(appDataFolderPath))
            {
                Directory.CreateDirectory(appDataFolderPath);
            }

            string preferenceFilePath = Path.Combine(appDataFolderPath, "DarkModePreference.txt");

            File.WriteAllText(preferenceFilePath, darkModeEnabled.ToString());
        }

        public class PrintFrontend
        {
            public static void ApplyTheme(Form form, bool isDarkModeEnabled)
            {
                form.BackColor = isDarkModeEnabled ? Color.FromArgb(40, 40, 40) : SystemColors.Control;

                foreach (Control control in form.Controls)
                {
                    if (control is ToolStrip)
                    {
                        continue;
                    }

                    if (isDarkModeEnabled)
                    {
                        control.BackColor = Color.FromArgb(40, 40, 40);
                        control.ForeColor = Color.White;

                        if (control is TextBox textBox)
                        {
                            textBox.BackColor = Color.FromArgb(31, 31, 31);
                            textBox.BorderStyle = BorderStyle.FixedSingle;
                        }
                        else if (control is Button button)
                        {
                            button.FlatStyle = FlatStyle.Flat;
                            button.FlatAppearance.BorderColor = Color.White;
                        }
                        else if (control is ComboBox comboBox)
                        {
                            comboBox.BackColor = SystemColors.Window;
                            comboBox.ForeColor = SystemColors.ControlText;
                        }
                    }
                    else
                    {
                        control.BackColor = SystemColors.Control;
                        control.ForeColor = SystemColors.ControlText;

                        if (control is TextBox textBox)
                        {
                            textBox.BackColor = Color.White;
                            textBox.BorderStyle = BorderStyle.Fixed3D;
                        }
                        else if (control is Button button)
                        {
                            button.BackColor = Color.White;
                            button.FlatStyle = FlatStyle.Standard;
                            button.FlatAppearance.BorderColor = Color.White;
                        }
                        else if (control is ComboBox comboBox)
                        {
                            comboBox.BackColor = SystemColors.Window;
                            comboBox.ForeColor = SystemColors.ControlText;
                        }
                    }
                }
            }
        }

        private void PopulatePrintersComboBox()
        {
            foreach (string printerName in PrinterSettings.InstalledPrinters)
            {
                comboBox_Printer.Items.Add(printerName);
            }

            if (comboBox_Printer.Items.Count > 0)
            {
                comboBox_Printer.SelectedIndex = 0;
            }

            GetSelectedPrinterDPI(comboBox_Printer.SelectedItem.ToString());
        }

        private void SelectPrinter(object sender, EventArgs e)
        {
            GetSelectedPrinterDPI(comboBox_Printer.SelectedItem.ToString());
        }

        private void GetSelectedPrinterDPI(string printerName)
        {
            PrinterSettings ps = new()
            {
                PrinterName = printerName
            };
            PrinterResolution pr = ps.DefaultPageSettings.PrinterResolution;

            if (ps.IsValid)
            {
                if (pr.Kind == PrinterResolutionKind.Custom)
                {
                    label_PrinterDPI.Text = $"{pr.X} x {pr.Y} DPI";
                }
                else
                {
                    label_PrinterDPI.Text = $"Preset: {pr.Kind}";
                }
            }
        }

        private void SaveBMP(object sender, EventArgs e)
        {
            if (comboBox_DPI.SelectedItem == null)
            {
                MessageBox.Show("Please select a dot code DPI", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            int DPI = int.Parse(comboBox_DPI.Text[..^4]);

            SaveFileDialog saveRAWFile = new()
            {
                Filter = "Bitmap Images (*.bmp)|*.bmp",
                Title = "Save Dot Code (.bmp)",
                FileName = "dotcode"
            };

            if (saveRAWFile.ShowDialog() == DialogResult.OK)
            {
                ProcessStartInfo raw_to_bmp = new()
                {
                    FileName = Common.RAW2BMP,
                    Arguments = Common.RAW2BMP_ARGS(saveRAWFile.FileName.Replace(".bmp", ""), DPI),
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                };
                using Process process = Process.Start(raw_to_bmp);
                process.WaitForExit();
            }
        }

        private void Print(object sender, EventArgs e)
        {
            if (comboBox_Printer.SelectedItem == null)
            {
                MessageBox.Show("No valid printers found!", "Warning", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            int DPI = int.Parse(comboBox_DPI.Text[..^4]);
            MessageBox.Show($"{DPI}");

            ProcessStartInfo raw_to_bmp = new()
            {
                FileName = Common.RAW2BMP,
                Arguments = Common.RAW2BMP_ARGS(Common.BMP_DOTCODE, DPI),
                UseShellExecute = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true
            };
            using (Process process = Process.Start(raw_to_bmp)) { process.WaitForExit(); }

            PrintDotCode();
        }

        private void PrintDotCode()
        {
            string selectedPrinter = comboBox_Printer.SelectedItem.ToString();
            PrintDocument printDocument = new();
            printDocument.PrinterSettings.PrinterName = selectedPrinter;

            printDocument.PrintPage += (sender, e) =>
            {
                Image image = Image.FromFile(Common.BMP_DOTCODE+".bmp");
                Point loc = new(0, 0);
                e.Graphics.DrawImage(image, loc);
            };

            try
            {
                printDocument.Print();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"An error occurred while printing: {ex.Message}");
            }
        }
    }
}
