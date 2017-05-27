using System;
using System.Speech.Recognition;
using System.IO;

namespace SpeechRecognition
{
    class Program
    {
        static void Main(string[] args)
        {
            string keyphrasesFile = "keyphrases.txt";
            if(args.Length != 0)
            {
                keyphrasesFile = args[0];
            }

            using (SpeechRecognitionEngine recognizer = new SpeechRecognitionEngine(new System.Globalization.CultureInfo("en-US")))
            {
                Choices choices = new Choices();
                choices.Add(getChoices(keyphrasesFile));

                GrammarBuilder gb = new GrammarBuilder();
                gb.Append(choices);

                // Create the Grammar instance.
                Grammar g = new Grammar(gb);

                DictationGrammar dg = new DictationGrammar();
                recognizer.LoadGrammar(g);
                recognizer.SpeechRecognized += new EventHandler<SpeechRecognizedEventArgs>(recognizer_SpeechRecognized);
                recognizer.SpeechRecognitionRejected += new EventHandler<SpeechRecognitionRejectedEventArgs>(recognizer_SpeechNotRecognized);
                recognizer.SetInputToDefaultAudioDevice();
                recognizer.RecognizeAsync(RecognizeMode.Multiple);
                while (true)
                {
                    Console.ReadLine();
                }
            }
        }

        static string[] getChoices(string fileName)
        {
            try
            {
                return File.ReadAllLines(fileName);
            }
            catch (Exception)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("No " + fileName + " found.");
                Environment.Exit(1);
                return new string[] { };
            }
        }

        static void recognizer_SpeechRecognized(object sender, SpeechRecognizedEventArgs e)
        {
            Console.Write(e.Result.Text);
        }

        static void recognizer_SpeechNotRecognized(object sender, SpeechRecognitionRejectedEventArgs e)
        {
            Console.Write("$$unknown$$");
        }
    }
}
