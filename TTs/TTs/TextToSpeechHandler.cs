using System;
using System.Collections;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Speech.Synthesis;

namespace TTS
{

    public class TextToSpeechHandler
    {

        public static String TextToSpeech(String text, int voiceIndex = 0, bool voiceIntroduce = false)
        {
            // Initialize a new instance of the SpeechSynthesizer.

            SpeechSynthesizer synth = new SpeechSynthesizer();

            synth.SetOutputToWaveFile("msg.wav");

            

            ArrayList voices = new ArrayList();

            foreach (InstalledVoice voice in synth.GetInstalledVoices())
            {
                VoiceInfo info = voice.VoiceInfo;

                voices.Add(info.Name);
            }

            if (voiceIndex > synth.GetInstalledVoices().Count)
            {
                voiceIndex = synth.GetInstalledVoices().Count;
            }

            synth.SelectVoice(voices[voiceIndex].ToString());

            String msg = text;

            if (voiceIntroduce)
            {
                msg = "Hi, my name is " + voices[voiceIndex];
            }

            synth.Speak(msg);
            synth.Dispose();

            return Path.GetFullPath("msg.wav");
        }

        public static String TextToSpeech(String text, int voiceIndex = 0)
        {
            // Initialize a new instance of the SpeechSynthesizer.

            SpeechSynthesizer synth = new SpeechSynthesizer();

            synth.SetOutputToWaveFile("msg.wav");



            ArrayList voices = new ArrayList();

            foreach (InstalledVoice voice in synth.GetInstalledVoices())
            {
                VoiceInfo info = voice.VoiceInfo;

                voices.Add(info.Name);
            }

            if (voiceIndex > synth.GetInstalledVoices().Count)
            {
                voiceIndex = synth.GetInstalledVoices().Count;
            }

            synth.SelectVoice(voices[voiceIndex].ToString());

            String msg = text;

            synth.Speak(msg);
            synth.Dispose();

            return Path.GetFullPath("msg.wav");
        }

        public static String TextToSpeech(String text)
        {
            // Initialize a new instance of the SpeechSynthesizer.

            SpeechSynthesizer synth = new SpeechSynthesizer();

            synth.SetOutputToWaveFile("msg.wav");



            ArrayList voices = new ArrayList();

            foreach (InstalledVoice voice in synth.GetInstalledVoices())
            {
                VoiceInfo info = voice.VoiceInfo;

                voices.Add(info.Name);
            }


            synth.SelectVoice(voices[0].ToString());

            String msg = text;

            synth.Speak(msg);
            synth.Dispose();

            return Path.GetFullPath("msg.wav");
        }
    }
}
