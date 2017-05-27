const fs = require('fs');
const mic = require('mic');
const spawn = require('child_process').spawn;
const execSync = require('child_process').execSync;
const EventEmitter = require('events');
const SpeechToTextV1 = require('watson-developer-cloud/speech-to-text/v1');

let speechToTextConfiguration = {
    "url": "https://stream.watsonplatform.net/speech-to-text/api",
    "username": "21d51917-0097-4356-8a58-331d4c9ac558",
    "password": "xLB54SZKHpVl"
};

let SpeechRecognition = function () {
    let that = this;
    
    let speechStream = new EventEmitter();

    // Offline speech recognition.
    let offlineRecognitionSucceded = true;
    let speechRecognitionProcess = null;

    // Remote speech recognition.
    let sending = false;
    let microphoneBuffer = new Uint8Array(0);
    let microphoneIstance = null;
    let microphoneInputStream = null;
    let SpeechToTextService = new SpeechToTextV1(speechToTextConfiguration);

    // Starts Speech Recognition process.
    that.start = (options) => {
        options = options || {};
        options.online = options.online || true;

        speechRecognitionProcess = spawn('SpeechRecognition');
        speechRecognitionProcess.stdout.on('data', onSpeechRecognized);

        // Start microphone recognition by Sox to send it to the remote service.
        if (options.online === true) {
            setupMicrophone();
            microphoneIstance.start();
        }
    }

    // Stops Speech Recognition process.
    that.stop = () => {
        if (speechRecognitionProcess !== null) {
            // Stop the process.
            speechRecognitionProcess.kill('SIGTERM');
            // Clear local properties.
            speechRecognitionProcess = null;
        }

        if (microphoneIstance !== null) {
            // Stop the process.
            microphoneIstance.stop();
            // Clear local properties.
            microphoneBuffer = new Uint8Array(0);
            microphoneIstance = null;
            microphoneInputStream = null;
        }
    }

    // Gets speech stream.
    that.getSpeechStream = () => {
        return speechStream;
    }

    function appendBuffer(buffer1, buffer2) {
        var tmp = new Uint8Array(buffer1.byteLength + buffer2.byteLength);
        tmp.set(new Uint8Array(buffer1), 0);
        tmp.set(new Uint8Array(buffer2), buffer1.byteLength);
        return tmp.buffer;
    };

    function setupMicrophone() {
        microphoneIstance = mic({ rate: '16000', channels: '1', debug: false, exitOnSilence: 2 });
        microphoneInputStream = microphoneIstance.getAudioStream();
        microphoneInputStream.on('data', onMicrophoneData);
        microphoneInputStream.on('silence', onMicrophoneSilence);
        microphoneInputStream.on('error', onMicrophoneError);
    }

    function onMicrophoneData(data) {
        microphoneBuffer = appendBuffer(microphoneBuffer, data);
    }

    function onMicrophoneSilence(data) {

        let stream = fs.createWriteStream('audio.raw');

        if (offlineRecognitionSucceded) {
            resetMicrophoneBuffer();
            return;
        }

        // Write the audio segment temp file. The write callback is used to clear the buffer when the operation will be finished.
        stream.write(Buffer.from(microphoneBuffer), null, () => {
            // Reset the microphone buffer preserving initial data.
            // The initial datas contain information about the file, so if you remove them, the file will result corrupted.
            resetMicrophoneBuffer();
            sendAudioToRemoteService();
        });
    }

    function onMicrophoneError(error) {
        console.log("Something was wrong with the microphone input: " + error);
    }

    function resetMicrophoneBuffer() {
        microphoneBuffer = new Uint8Array(microphoneBuffer, 0, 4096);
    }

    function sendAudioToRemoteService(file) {
        let fileName = file || 'audio.raw';
        let fileNameConverted = 'audio.ogg';
        // Convert the raw audio to .ogg file: the size reduction is sometimes over than 10 times.
        execSync('sox ' + fileName + ' ' + fileNameConverted);
        // execSync('sox test.raw audio.ogg silence 1 0.2 0.3%'); could trim the audio to avoid to send long silences: it must be tuned because sometimes the cut is too strong.

        if (offlineRecognitionSucceded) {
            resetMicrophoneBuffer();
            return;
        }

        let params = {
            audio: fs.createReadStream(fileNameConverted),
            content_type: 'audio/ogg;codecs=opus',
            timestamps: false,
            continuous: false,
            word_alternatives_threshold: 0.9,
        };

        sending = true;
        SpeechToTextService.recognize(params, onSpeechToTextServiceResponse);
    }

    function onSpeechToTextServiceResponse(error, response) {
        // Sending finished.
        sending = false;

        if (error) {
            console.log('Error from SpeechToTextService: ', error);
            return;
        }

        if (!response || !response.results || !response.results.length) {
            return;
        }

        let firstTranscriptionResult = response.results[0];
        let firstAlternative = firstTranscriptionResult.alternatives[0].transcript;

        if (offlineRecognitionSucceded) {
            resetMicrophoneBuffer();
            return;
        }

        speechStream.emit('recognized', firstAlternative);
    }

    // Listen for new data from SpeechRecognition process.
    function onSpeechRecognized(binaryText) {
        let text = String(binaryText);

        if (text == '$$unknown$$') {
            offlineRecognitionSucceded = false;
            return;
        }
        
        offlineRecognitionSucceded = true;
        speechStream.emit('recognized', text);
    }
}

module.exports = SpeechRecognition;