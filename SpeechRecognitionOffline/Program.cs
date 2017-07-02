using System;
using System.IO;
using Microsoft.Speech.Recognition;
using Choices = Microsoft.Speech.Recognition.Choices;
using Grammar = Microsoft.Speech.Recognition.Grammar;
using GrammarBuilder = Microsoft.Speech.Recognition.GrammarBuilder;
using RecognizeMode = Microsoft.Speech.Recognition.RecognizeMode;
using SpeechRecognitionEngine = Microsoft.Speech.Recognition.SpeechRecognitionEngine;
using SpeechRecognitionRejectedEventArgs = Microsoft.Speech.Recognition.SpeechRecognitionRejectedEventArgs;
using SpeechRecognizedEventArgs = Microsoft.Speech.Recognition.SpeechRecognizedEventArgs;

namespace SpeechRecognition
{
    class Program
    {
        private static SpeechRecognitionEngine _recognizer;
        static void Main(string[] args)
        {
            var keyphrasesFile = "keyphrases.txt";
            if(args.Length != 0)
            {
                keyphrasesFile = args[0];
            }

            RecognizerInfo info = null;
            foreach(var ri in SpeechRecognitionEngine.InstalledRecognizers()) {
                if(ri.Culture.TwoLetterISOLanguageName.Equals("en")) {
                    info = ri;
                    break;
                }
            }
            if(info == null) return;

            using (_recognizer = new SpeechRecognitionEngine(info))
            {
                var keyphrases = new Choices(getChoices(keyphrasesFile));


                var gb = new GrammarBuilder(keyphrases) {Culture = info.Culture};

                // Create the Grammar instance.
                var g = new Grammar(gb) {Name = "Keyphrases"};
                
                _recognizer.RequestRecognizerUpdate();
                _recognizer.LoadGrammar(g);
                _recognizer.SpeechRecognized += recognizer_SpeechRecognized;
                _recognizer.SpeechRecognitionRejected += recognizer_SpeechNotRecognized;
                _recognizer.SetInputToDefaultAudioDevice();
                _recognizer.RecognizeAsync(RecognizeMode.Multiple);
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
