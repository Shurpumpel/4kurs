using System;
using System.IO;
using System.Text;

namespace LR3
{
    class Program
    {

        static int choseComand()
        {
            int code = 0;
            Console.WriteLine("Choose operation:\n");
            Console.WriteLine("0-exit from program\n1-encode file\n2-decode file\n");
            code = Convert.ToInt32(Console.ReadLine());
            return code;
        }

        /*Создание формата закодированного файла*/
        static string createFormat(int method, int sourceLength, string block, string fileName)
        {
            char[] format = new char[32];
            format[0] = 'r';
            format[1] = 'j';
            format[2] = 'k';
            format[3] = '1';
            format[4] = Convert.ToString(method)[0];
            string formatStr = Convert.ToString(sourceLength, 16);

            /*            Console.WriteLine("Length of old text: " + sourceLength);*/

            for (int i = formatStr.Length - 1; i >= 0; i--)
            {
                format[i + 5] = formatStr[i];
            }

            /*            formatStr = Convert.ToString(newLength, 16);
                        for (int i = formatStr.Length - 1; i >= 0; i--)
                        {
                            format[i + 9] = formatStr[i];
                        }*/

            int blockValue = Convert.ToInt32(block);
            formatStr = Convert.ToString(blockValue, 16);

            format[13] = formatStr[0];
            /*            Console.WriteLine("Header block - " + format[13]);*/
            for (int i = 14; i < fileName.Length + 14; i++)
            {
                format[i] = fileName[i - 14];
            }

            string newString = new string(format);
            /* Console.WriteLine("Получившийся заголовок " + newString);*/
            return newString;
        }

        /*Кодирование данных из файла*/
        static string[] encodeFileInfo(String path)
        {
            FileStream fstream = new FileStream(@$"{path}", FileMode.Open);

            byte[] array = new byte[fstream.Length];
            fstream.Read(array, 0, array.Length);
            string textFromFile = System.Text.Encoding.Default.GetString(array);

            if (textFromFile.Remove(0, 3).Equals("rjk"))
            {
                /*for (int i = 14; i < 32; i++)
                {
                    if (textFromFile[i] == '\0')
                        continue;
                    else
                    {*/
                string[] request = new string[2];
                request[0] = request[1] = "\0";
                return request;
                /*    }                        
                }*/
            }


            Random rand = new Random();
            int blockLength = rand.Next(16);
            int count = textFromFile.Length / (blockLength + 1);
            int countBlock = 0;
            string[] newText = new string[3];
            Console.WriteLine("Count - " + count);
            Console.WriteLine("Block - " + blockLength);
            Console.WriteLine("Length text - " + textFromFile.Length);
            newText[2] = Convert.ToString(textFromFile.Length);
            for (int i = 0; i < count; i++)
            /*while (textFromFile.Length != 0)*/
            {
                countBlock++;
                //fuckq1ing q2shitq3
                // удаляем с индекса blockLength оставшиеся символы в textFromFile и суммируем с newText и q{}
                if (textFromFile.Length > blockLength + 1)
                {
                    newText[0] = newText[0] + textFromFile.Remove(blockLength + 1) + $"q{countBlock}";
                    textFromFile = textFromFile.Remove(0, blockLength + 1);
                }
                else
                {
                    newText[0] = newText[0] + textFromFile;
                    textFromFile = textFromFile.Remove(0);
                }

                if (countBlock == 255)
                {
                    countBlock = 0;
                    continue;
                }
            }

            fstream.Close();

            newText[1] = blockLength.ToString();

            return newText;
        }

        /*Сбор закодированной информации и создание файла*/
        static void encoding(String path, string name, string newName)
        {
            string[] newInfo = new string[3];
            newInfo = encodeFileInfo(@$"{path}{name}");
            /*Console.WriteLine(newInfo[1]);*/
            FileInfo sourceInfo = new FileInfo(@$"{path}{name}");
            int sourceFileLength = (int)sourceInfo.Length;
            string fileFormat = createFormat((int)1, Convert.ToInt32(newInfo[2]), newInfo[1], name);

            if (newInfo[0].Equals('\0') && newInfo[1].Equals('\0'))
            {
                Console.WriteLine("This file already encoded");
            }
            else
            {

                newInfo[0] = newInfo[0].Insert(0, fileFormat);
                Console.WriteLine("Length encoded file: " + newInfo[0].Length);
                /*int i = sourceInfo.Name.Length;
                name = new;
                while (sourceInfo.Name[i - 1] != '.')
                {
                    name = sourceInfo.Name.Remove(i - 1);
                    i--;
                }
                name += "rjk";*/
                newName += ".rjk";
                FileStream newFstream = new FileStream($@"{path}{newName}", FileMode.Create);

                byte[] forWrite = System.Text.Encoding.Default.GetBytes(newInfo[0]);
                Console.WriteLine("Length of bytes: " + forWrite.Length);
                newFstream.Write(forWrite, 0, forWrite.Length);

                newFstream.Close();

                Console.WriteLine("\nFile has been encoded\n");
            }
        }
        /*Декодирование информации из файла*/
        static string[] decryptFileInfo(String path)
        {
            /*Считывание данных из файла*/
            FileStream fstream = new FileStream(@$"{path}", FileMode.Open);
            Console.WriteLine("Length bytes array: " + fstream.Length);
            byte[] array = new byte[fstream.Length];
            fstream.Read(array, 0, array.Length);
            string textFromFile = System.Text.Encoding.Default.GetString(array);

            string[] textOnly = new string[2];
            textOnly[0] = textOnly[1] = "\0";

            Console.WriteLine("Length decrypting file: " + textFromFile.Length);
            if (textFromFile.Remove(3, textFromFile.Length - 3).Equals("rjk"))
            {
                char[] text = textFromFile.ToCharArray();
                if (text[4] == '1') // Если мы добавили q{int}
                {
                    string headerInfo = "";
                    for (int i = 5; i <= 8; i++)
                    {
                        if (text[i] != '\0')
                            headerInfo += text[i];
                    }
                    int oldLength = Convert.ToInt32(headerInfo, 16);

                    headerInfo = text[13].ToString();
                    int block = Convert.ToInt32(headerInfo, 16);
                    headerInfo = "";
                    for (int i = 14; i < 32; i++)
                    {
                        if (text[i] != '\0')
                            headerInfo += text[i];
                    }
                    textOnly[1] = headerInfo;

                    textOnly[0] = new string(text);
                    textOnly[0] = textOnly[0].Remove(0, 32);

                    int count = oldLength / (block + 1);
                    int countBlock = 0;
                    int numberQ = 0;
                    string decodedText = "";
                    for (int i = 0; i < count; i++)
                    {                        
                        if (numberQ < 10)
                        {
                            if (textOnly[0].Length > block + 1)
                            {
                                decodedText += textOnly[0].Remove(block + 1, textOnly[0].Length - block - 1);
                                string test = textOnly[0].Remove(block + 1, textOnly[0].Length - block - 1);
                            }
                            else
                                decodedText += textOnly[0].Substring(0, textOnly[0].Length);

                            if (textOnly[0].Length > block + 3)
                                textOnly[0] = textOnly[0].Remove(0, block + 3);
                            else
                                textOnly[0] = textOnly[0].Remove(0, textOnly[0].Length);
                            continue;
                        }
                        if (numberQ < 100)
                        {
                            if (textOnly[0].Length > block + 1)
                            {
                                decodedText += textOnly[0].Remove(block, textOnly[0].Length - block - 1);
                                string test = textOnly[0].Remove(block, textOnly[0].Length - block - 1);
                            }
                            else
                                decodedText += textOnly[0].Substring(0, textOnly[0].Length);

                            if (textOnly[0].Length > block + 4)
                            {
                                textOnly[0] = textOnly[0].Remove(0, block + 4);
                            }
                            else
                            {
                                textOnly[0] = textOnly[0].Remove(0, textOnly[0].Length);
                            }

                            continue;
                        }
                        if (numberQ <= 255)
                        {

                            if (textOnly[0].Length > block + 1)
                            {
                                decodedText += textOnly[0].Remove(block, textOnly[0].Length - block - 1);
                                string test = textOnly[0].Remove(block, textOnly[0].Length - block - 1);
                            }
                            else
                                decodedText += textOnly[0].Substring(0, textOnly[0].Length);

                            if (textOnly[0].Length > block + 5)
                                textOnly[0] = textOnly[0].Remove(0, block + 5);
                            else
                                textOnly[0] = textOnly[0].Remove(0, textOnly[0].Length);

                        }
                        if (numberQ == 255)
                        {
                            numberQ = 0;
                        }
                    }
                    textOnly[0] = decodedText;
                    return textOnly;
                }
                else
                {
                    Console.WriteLine("\nI do not know this decode algorithm\n");
                    return textOnly;
                }

            }

            fstream.Close();

            return textOnly;
        }
        /*
            0-2     - sign
            3       - version
            4       - compression type
            5-12    - file length
            13      - block value
            14-31   - file length/reserved
        */

        /*Старт декодирования*/
        static void decoding(String path, string name)
        {
            string[] newInfo = new string[2];
            newInfo = decryptFileInfo(@$"{path}{name}");
            /*FileInfo sourceInfo = new FileInfo(@$"{path}\{name}");*/

            if (!newInfo[0].Equals("\0"))
            {
                name = newInfo[1];

                FileStream newFstream = new FileStream($@"{path}\{name}", FileMode.Create);

                byte[] forWrite = System.Text.Encoding.Default.GetBytes(newInfo[0]);
                Console.WriteLine("Length bytes decrypted " + forWrite.Length);
                newFstream.Write(forWrite, 0, forWrite.Length);

                newFstream.Close();

                Console.WriteLine("\nFile has been decrypted\n");
            }
            else
                Console.WriteLine("\nCannot read this file\n");

        }
        static void Main(string[] args)
        {
            String path = @"C:\Users\alfdt\OneDrive\Документы\УЧЕБА\Labs\4kurs\Отик\";

            while (true)
            {
                bool flag = true;
                Console.WriteLine("Write name file");
                string name = Console.ReadLine();
                switch (choseComand())
                {
                    case 0:
                        flag = false;
                        break;
                    case 1:
                        Console.WriteLine("Write new file name");
                        string newName = Console.ReadLine();
                        encoding(path, name, newName);
                        break;
                    case 2:
                        decoding(path, name);
                        break;
                    default:
                        Console.WriteLine("Command not found");
                        break;
                }
                if (!flag)
                    break;
            }
        }
    }
}
