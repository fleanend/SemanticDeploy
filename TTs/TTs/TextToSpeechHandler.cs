using System;
using System.Collections;
using System.IO;
using Microsoft.Speech.Synthesis;
using Microsoft.Speech.AudioFormat;

namespace TTS
{

    public class TextToSpeechHandler
    {

        public static MemoryStream TextToSpeech(String text, int voiceIndex = 0, bool voiceIntroduce = false)
        {
            // Initialize a new instance of the SpeechSynthesizer.

            SpeechSynthesizer synth = new SpeechSynthesizer();


            MemoryStream streamAudio = new MemoryStream();

            synth.SetOutputToAudioStream(streamAudio, new SpeechAudioFormatInfo(EncodingFormat.Pcm, 44100, 16, 1, 16000, 2, null));

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
            synth.SetOutputToNull();
            synth.Dispose();
            return streamAudio;
        }

        public static MemoryStream TextToSpeech(String text, int voiceIndex = 0)
        {
            return TextToSpeech(text, voiceIndex, false);
        }

        public static MemoryStream TextToSpeech(String text)
        {
            return TextToSpeech(text, 0, false);
        }
    }
}
