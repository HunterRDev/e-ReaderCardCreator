using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using AC_e_Reader_Card_Creator.References;

namespace AC_e_Reader_Card_Creator.Decompression.Functions
{
    internal class Decompressed
    {
        public static Dictionary<string, string> GetData()
        {
            byte[] GCN_Letter_Bytes = { };
            Dictionary<string, string> letterData = new Dictionary<string, string>();

            try
            {
                GCN_Letter_Bytes = File.ReadAllBytes(Common.GCN_LETTER_DATA);
            }
            catch (IOException ex)
            {
                Console.WriteLine($"An error occurred while reading the file: {ex.Message}");
            }

            string letter_contents = ConvertBytesToACAscii(GCN_Letter_Bytes);
            string full_greeting = GetGreeting(letter_contents);

            // bunch of hardcoding with the exact byte positions - source these out to variables 
            int player_index = Convert.ToInt32("0x" + GCN_Letter_Bytes[253].ToString("X2"), 16) - 1;
            string letter_greeting = InsertPlayerVar(full_greeting, player_index).Trim();
            letterData.Add("letter_greeting", letter_greeting);

            string letter_body = GetStringInRange(letter_contents, 24, 207);
            letterData.Add("letter_body", letter_body);

            string letter_closing = GetStringInRange(letter_contents, 216, 246).Trim();
            letterData.Add("letter_closing", letter_closing);

            byte[] stationeryID_bytes = { GCN_Letter_Bytes[247], GCN_Letter_Bytes[248] };
            string stationeryID = BytesToHex(stationeryID_bytes).Replace(" ", "");
            letterData.Add("letter_stationery", stationeryID);

            byte[] giftID_bytes = { GCN_Letter_Bytes[249], GCN_Letter_Bytes[250] };
            string giftID = BytesToHex(giftID_bytes).Replace(" ", "");
            letterData.Add("letter_gift", giftID);

            byte[] senderID_bytes = { GCN_Letter_Bytes[251], GCN_Letter_Bytes[252] };
            string senderID = BytesToHex(senderID_bytes).Replace(" ","");
            letterData.Add("letter_sender", senderID);

            return letterData;
        }

        public static string ConvertBytesToACAscii(byte[] byteArray)
        {
            StringBuilder result = new StringBuilder();

            foreach (byte b in byteArray)
            {
                // handles newline
                if (b == 0xCD)
                {
                    result.Append("\n");
                }
                // handles standard ASCII conversion
                else if (b >= 32 && b <= 127)
                {
                    result.Append((char)b);
                }
                // handles AC's special characters; standard ASCII char conversion is faster,
                // so only lookup special characters instead of doing this for every character
                else if (b < 32 || b > 127)
                {
                    string special_char = "0x" + b.ToString("X2");
                    special_char = Common.LookupListValue(special_char, Common.SP_CHAR_LIST);
                    result.Append(special_char);
                }
                else
                {
                    result.Append("-");
                }
            }

            return result.ToString();
        }

        public static int PlayerVarFinder(string greeting)
        {
            for (int i = greeting.Length - 2; i > 0; i--)
            {
                if (greeting[i] == ' ' && greeting[i + 1] != ' ')
                {
                    return i;
                }
            }
            return -1;
        }

        public static string GetGreeting(string letter)
        {
            if (string.IsNullOrEmpty(letter) || letter.Length < 24)
            {
                return letter;
            }

            return letter.Substring(0, 24);
        }

        public static string GetStringInRange(string letter, int startIndex, int endIndex)
        {

            if (string.IsNullOrEmpty(letter))
            {
                throw new ArgumentException("The letter is null or empty.");
            }

            if (startIndex < 0 || endIndex >= letter.Length || startIndex > endIndex)
            {
                throw new ArgumentOutOfRangeException("Start or end index is out of range.");
            }

            int length = endIndex - startIndex + 1;
            return letter.Substring(startIndex, length);
        }

        public static string InsertPlayerVar(string greeting, int index)
        {
            if (index == 0 || index < 0 || index > greeting.Length)
            {
                string revisedGreeting = greeting.Insert(0, "<Player>");
                return revisedGreeting;
            }

            return greeting.Substring(0, index) + " <Player>" + greeting.Substring(index + 1);
        }

        public static string BytesToHex(byte[] byteArray)
        {
            StringBuilder result = new StringBuilder();

            foreach (byte b in byteArray)
            {
                result.AppendFormat("{0:X2} ", b);
            }

            return result.ToString().TrimEnd();
        }
    }
}
