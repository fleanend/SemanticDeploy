'use strict';

const net = require('net');
const ConversationModule = require('watson-developer-cloud/conversation/v1');
const SpeechRecognition = require('./src/SpeechRecognition');
const util = require('util');

// Regressor fn params
const C_ANG = 0.05454;
const C = 2.336;
// Lag time
const OFFSET = 1;
const ERROR_COMP = 1;

// Dialog learning states.
const DIALOG = {
    LEARN: {
        START: 'StartLearning',
        CONFIRM_INTENT: 'ConfirmIntentName',
        CORRECT_INTENT: 'CorrectIntentName',
        WRONG_INTENT: 'WrongIntentName',
        UNKNOWN_INTENT: 'UnknownIntentNameConfirm',
        CONFIRM_EXAMPLE: 'ConfirmExample',
        CORRECT_EXAMPLE: 'CorrectExample',
        WRONG_EXAMPLE: 'WrongExample',
        UNKNWONW_EXAMPLE: 'UnknownExample',
        STOP: 'StopLearning',
    }
};

const Conversation = new ConversationModule({
    url: "https://gateway.watsonplatform.net/conversation/api",
    username: "bc315d05-5326-4ea3-86cf-372b8da46df1",
    password: "nVS1Bnq2RNh2",
    version_date: ConversationModule.VERSION_DATE_2017_02_03,
});

// Create TCP connections to communicate with Unreal Engine.
const speechClient = new net.Socket();
const botClient = new net.Socket();

speechClient.connect(9000, '127.0.0.1', () => console.log('Speech TCP client connected.'));
botClient.connect(9001, '127.0.0.1', () => console.log('Bot TCP client connected.'));


// Used to check if there is a pending request to dialog system.
let waitingDialog = false;

let speaking = false;

// Starts speech recognition.
var sr = new SpeechRecognition();
sr.start();
sr.getSpeechStream().on('recognized', (text) => {
    speechClient.write(text);
    if (!waitingDialog) {
        dialog(text);
    }
});

// EDIT: action on speech notRecognized
sr.getSpeechStream().on('notRecognized', (text) => {
    speechClient.write(text);
    if (!waitingDialog) {
        dialog(text);
    }
})

// Keep the dialog states.
let context = {};

// Dialog status name.
let status = null;

// Store a intent: used to create a new one.
let intent = {
    title: null,
    examples: [],
};
let confirmedExample = null;

function speechTimeFromString(speechText) {
    let vowelCounter = speechText.match(/[aeiouyw]/gi);
    return (OFFSET + (C_ANG * vowelCounter + C) + ERROR_COMP);
}

function dialog(speech) {
    waitingDialog = true;
    Conversation.message({
        workspace_id: '9676d0e7-f6d2-4a8d-8612-1ae60999b734',
        input: { text: speech },
        context: context,
    }, (serviceError, serviceResponse) => {
        waitingDialog = false;
        if (serviceError) {
            console.log('Something was wrong with Dialog Service: ' + serviceError);
            return;
        }

        let outputText = null;
        let inputText = null;

        context = serviceResponse.context;

        if (serviceResponse.output.nodes_visited[0]) {
            status = serviceResponse.output.nodes_visited[0];
        }

        // The input text sent to the bot.
        if (serviceResponse.input.text) {
            inputText = serviceResponse.input.text;
        }

        // The chatbot text response.
        if (serviceResponse.output.text[0]) {
            /*
            if (!speaking) {
                speaking = true;
                outputText = serviceResponse.output.text[0];
                let timerSpeaking = setTimeout(function () {
                    speaking = false;
                }, speechTimeFromString(outputText));
            }
            */
            outputText = serviceResponse.output.text[0];
        }

        // Define behaviors for leaning mode.
        switch (status) {
            case DIALOG.LEARN.CONFIRM_INTENT:
                outputText = util.format(outputText, inputText);
                intent.title = inputText.replace(/[\s]+/, '_');
                break;
            case DIALOG.LEARN.CORRECT_INTENT:
                outputText = util.format(outputText, intent.title);
                break;
            case DIALOG.LEARN.WRONG_INTENT:
                intent.title = null;
                break;
            case DIALOG.LEARN.CONFIRM_EXAMPLE:
                outputText = util.format(outputText, inputText);
                confirmedExample = inputText;
                break;
            case DIALOG.LEARN.CORRECT_EXAMPLE:
                intent.examples.push({ text: confirmedExample });
                break;
            case DIALOG.LEARN.STOP:
                context = {};
                Conversation.createIntent({
                    workspace_id: '9676d0e7-f6d2-4a8d-8612-1ae60999b734',
                    intent: intent.title,
                    examples: intent.examples,
                }, (s, e) => { console.log(s, e); });
                break;
        }

        botClient.write(outputText);
    });
}