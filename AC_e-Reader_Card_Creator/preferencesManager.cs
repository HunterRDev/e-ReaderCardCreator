using System;
using System.Drawing;
using System.Windows.Forms;

public class PreferencesManager
{
    private readonly string appDataFolderPath;
    private readonly string preferenceFilePath;

    // Additional color properties for dark mode
    public Color DarkModeBackColor => Color.FromArgb(40, 40, 40);
    public Color DarkModeForeColor => Color.White;
    public Color DarkModeTextBoxBackColor => Color.FromArgb(31, 31, 31);

    public PreferencesManager(string appName)
    {
        appDataFolderPath = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), appName);
        preferenceFilePath = System.IO.Path.Combine(appDataFolderPath, "config.txt");

        if (!System.IO.Directory.Exists(appDataFolderPath))
        {
            System.IO.Directory.CreateDirectory(appDataFolderPath);
        }
    }

    public void SaveDarkModePreference(bool darkModeEnabled)
    {
        string preferencesContent = $"DarkModeEnabled={darkModeEnabled}";
        System.IO.File.WriteAllText(preferenceFilePath, preferencesContent);
    }

    public bool ReadDarkModePreference()
    {
        if (System.IO.File.Exists(preferenceFilePath))
        {
            string preferencesContent = System.IO.File.ReadAllText(preferenceFilePath);

            if (bool.TryParse(GetPreferenceValue(preferencesContent, "DarkModeEnabled"), out bool darkModePreference))
            {
                return darkModePreference;
            }
        }

        return false;
    }

    public void ApplyDarkMode(Form form, bool isDarkModeEnabled)
    {
        form.BackColor = isDarkModeEnabled ? DarkModeBackColor : SystemColors.Control;
        ApplyThemeToControls(form.Controls, isDarkModeEnabled);
    }

    public void ApplyThemeToControls(Control.ControlCollection controls, bool isDarkModeEnabled)
    {
        foreach (Control control in controls)
        {
            if (control is ToolStrip)
            {
                continue;
            }

            if (isDarkModeEnabled)
            {
                control.BackColor = DarkModeBackColor;
                control.ForeColor = DarkModeForeColor;

                if (control is TextBox textBox)
                {
                    textBox.BackColor = DarkModeTextBoxBackColor;
                    textBox.BorderStyle = BorderStyle.FixedSingle;
                }
                else if (control is Button button)
                {
                    button.FlatStyle = FlatStyle.Flat;
                    button.FlatAppearance.BorderColor = DarkModeForeColor;
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
                    textBox.BackColor = SystemColors.Window;
                    textBox.BorderStyle = BorderStyle.Fixed3D;
                }
                else if (control is Button button)
                {
                    button.BackColor = SystemColors.Window;
                    button.FlatStyle = FlatStyle.Standard;
                    button.FlatAppearance.BorderColor = SystemColors.ControlText;
                }
                else if (control is ComboBox comboBox)
                {
                    comboBox.BackColor = SystemColors.Window;
                    comboBox.ForeColor = SystemColors.ControlText;
                }

                // Recursively apply theme to nested controls if any
                if (control.Controls.Count > 0)
                {
                    ApplyThemeToControls(control.Controls, isDarkModeEnabled);
                }
            }
        }
    }

    private string GetPreferenceValue(string content, string key)
    {
        string keyString = $"{key}=";
        int startIndex = content.IndexOf(keyString, StringComparison.OrdinalIgnoreCase);

        if (startIndex != -1)
        {
            int endIndex = content.IndexOf(Environment.NewLine, startIndex, StringComparison.OrdinalIgnoreCase);
            if (endIndex == -1)
            {
                endIndex = content.Length;
            }

            return content.Substring(startIndex + keyString.Length, endIndex - startIndex - keyString.Length).Trim();
        }

        return null;
    }
}