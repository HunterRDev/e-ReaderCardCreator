namespace AC_e_Reader_Card_Creator
{
    partial class Print_Frontend
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
            System.Windows.Forms.Button btn_Print;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Print_Frontend));
            this.comboBox_DPI = new System.Windows.Forms.ComboBox();
            this.label_DotCodeDPI = new System.Windows.Forms.Label();
            this.btn_SaveBMP = new System.Windows.Forms.Button();
            this.label_Printer = new System.Windows.Forms.Label();
            this.comboBox_Printer = new System.Windows.Forms.ComboBox();
            this.label_dpiPrinter = new System.Windows.Forms.Label();
            this.label_PrinterDPI = new System.Windows.Forms.Label();
            this.picture_DotCode = new System.Windows.Forms.PictureBox();
            btn_Print = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.picture_DotCode)).BeginInit();
            this.SuspendLayout();
            // 
            // btn_Print
            // 
            btn_Print.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            btn_Print.Location = new System.Drawing.Point(860, 146);
            btn_Print.Name = "btn_Print";
            btn_Print.Size = new System.Drawing.Size(162, 36);
            btn_Print.TabIndex = 5;
            btn_Print.Text = "Print\r\n";
            btn_Print.UseVisualStyleBackColor = true;
            btn_Print.Click += new System.EventHandler(this.Print);
            // 
            // comboBox_DPI
            // 
            this.comboBox_DPI.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBox_DPI.FormattingEnabled = true;
            this.comboBox_DPI.Items.AddRange(new object[] {
            "300 DPI",
            "600 DPI",
            "1200 DPI"});
            this.comboBox_DPI.Location = new System.Drawing.Point(31, 152);
            this.comboBox_DPI.Name = "comboBox_DPI";
            this.comboBox_DPI.Size = new System.Drawing.Size(134, 26);
            this.comboBox_DPI.TabIndex = 0;
            // 
            // label_DotCodeDPI
            // 
            this.label_DotCodeDPI.AutoSize = true;
            this.label_DotCodeDPI.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_DotCodeDPI.Location = new System.Drawing.Point(27, 130);
            this.label_DotCodeDPI.Name = "label_DotCodeDPI";
            this.label_DotCodeDPI.Size = new System.Drawing.Size(112, 19);
            this.label_DotCodeDPI.TabIndex = 1;
            this.label_DotCodeDPI.Text = "Dot Code DPI";
            // 
            // btn_SaveBMP
            // 
            this.btn_SaveBMP.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_SaveBMP.Location = new System.Drawing.Point(692, 146);
            this.btn_SaveBMP.Name = "btn_SaveBMP";
            this.btn_SaveBMP.Size = new System.Drawing.Size(162, 36);
            this.btn_SaveBMP.TabIndex = 4;
            this.btn_SaveBMP.Text = "Save .BMP";
            this.btn_SaveBMP.UseVisualStyleBackColor = true;
            this.btn_SaveBMP.Click += new System.EventHandler(this.SaveBMP);
            // 
            // label_Printer
            // 
            this.label_Printer.AutoSize = true;
            this.label_Printer.Location = new System.Drawing.Point(176, 130);
            this.label_Printer.Name = "label_Printer";
            this.label_Printer.Size = new System.Drawing.Size(60, 19);
            this.label_Printer.TabIndex = 6;
            this.label_Printer.Text = "Printer";
            // 
            // comboBox_Printer
            // 
            this.comboBox_Printer.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.comboBox_Printer.FormattingEnabled = true;
            this.comboBox_Printer.Location = new System.Drawing.Point(180, 151);
            this.comboBox_Printer.Name = "comboBox_Printer";
            this.comboBox_Printer.Size = new System.Drawing.Size(276, 26);
            this.comboBox_Printer.TabIndex = 7;
            this.comboBox_Printer.SelectionChangeCommitted += new System.EventHandler(this.SelectPrinter);
            // 
            // label_dpiPrinter
            // 
            this.label_dpiPrinter.AutoSize = true;
            this.label_dpiPrinter.Location = new System.Drawing.Point(471, 130);
            this.label_dpiPrinter.Name = "label_dpiPrinter";
            this.label_dpiPrinter.Size = new System.Drawing.Size(143, 19);
            this.label_dpiPrinter.TabIndex = 8;
            this.label_dpiPrinter.Text = "Printer DPI  (auto)";
            // 
            // label_PrinterDPI
            // 
            this.label_PrinterDPI.BackColor = System.Drawing.Color.White;
            this.label_PrinterDPI.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.label_PrinterDPI.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_PrinterDPI.Location = new System.Drawing.Point(475, 151);
            this.label_PrinterDPI.Name = "label_PrinterDPI";
            this.label_PrinterDPI.Size = new System.Drawing.Size(185, 27);
            this.label_PrinterDPI.TabIndex = 9;
            this.label_PrinterDPI.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // picture_DotCode
            // 
            this.picture_DotCode.Image = global::AC_e_Reader_Card_Creator.Properties.Resources.sample_dotcode;
            this.picture_DotCode.InitialImage = global::AC_e_Reader_Card_Creator.Properties.Resources.sample_dotcode;
            this.picture_DotCode.Location = new System.Drawing.Point(31, 28);
            this.picture_DotCode.Name = "picture_DotCode";
            this.picture_DotCode.Size = new System.Drawing.Size(991, 63);
            this.picture_DotCode.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picture_DotCode.TabIndex = 11;
            this.picture_DotCode.TabStop = false;
            // 
            // Print_Frontend
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(1061, 205);
            this.Controls.Add(this.picture_DotCode);
            this.Controls.Add(this.label_PrinterDPI);
            this.Controls.Add(this.label_dpiPrinter);
            this.Controls.Add(this.comboBox_Printer);
            this.Controls.Add(this.label_Printer);
            this.Controls.Add(btn_Print);
            this.Controls.Add(this.btn_SaveBMP);
            this.Controls.Add(this.label_DotCodeDPI);
            this.Controls.Add(this.comboBox_DPI);
            this.Font = new System.Drawing.Font("Arial", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimizeBox = false;
            this.Name = "Print_Frontend";
            this.Text = "Print Dot Code";
            ((System.ComponentModel.ISupportInitialize)(this.picture_DotCode)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox_DPI;
        private System.Windows.Forms.Label label_DotCodeDPI;
        private System.Windows.Forms.Button btn_SaveBMP;
        private System.Windows.Forms.Label label_Printer;
        private System.Windows.Forms.ComboBox comboBox_Printer;
        private System.Windows.Forms.Label label_dpiPrinter;
        private System.Windows.Forms.Label label_PrinterDPI;
        private System.Windows.Forms.PictureBox picture_DotCode;
    }
}