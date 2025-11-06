#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()  //Default constructor
{
    setSize(600, 400);

    addAndMakeVisible(pianoKeyboard);
    addAndMakeVisible(toolbar);

    toolbar.setDeviceChangeCallback([this](int deviceIndex) {
        openMidiInputByIndex(deviceIndex);
    });

    toolbar.refreshDevicesList();
    previousDeviceCount = juce::MidiInput::getAvailableDevices().size();

	startTimer(2000); //Start timer to check for MIDI device changes every 2 seconds
}

MainComponent::~MainComponent() //Default destructor
{
    if (midiInputDevice) {
        midiInputDevice->stop();
    }
    
}

//==============================================================================

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour(juce::Colour(0xFF303030));
    g.fillAll();

}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
     juce::Rectangle localBounds = getLocalBounds();
     int width = getWidth();
     int height = getHeight();

    pianoKeyboard.setBounds(0,(height - 80), width, 80);
    toolbar.setBounds(0, 0, width, 50);
}


void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
	// Handle incoming MIDI messages here
    int noteNumber = message.getNoteNumber();
    juce::String noteName = message.getMidiNoteName(noteNumber, true, true, 4);
    juce::uint8 noteVelocity = message.getVelocity();
    
    if (message.isNoteOn()) {   //Hop onto UI thread instead of MIDI thread
        
        
        if (message.getVelocity() > 0) {
            juce::MessageManager::callAsync([this, noteNumber, noteVelocity, noteName]() {
                pianoKeyboard.setNotePressed(noteNumber, true, noteVelocity);
                juce::Logger::writeToLog(noteName + "pressed with velocity " + juce::String(noteVelocity));
            });
        }
        else {  //Since some MIDI devices send Note On with velocity 0 instead of Note Off
            juce::MessageManager::callAsync([this, noteNumber]() {
                pianoKeyboard.setNotePressed(noteNumber, false);
            });
        }
        
    }
	else if (message.isNoteOff()) { //Hop onto UI thread instead of MIDI thread
        juce::MessageManager::callAsync([this, noteNumber, noteName]() {
            pianoKeyboard.setNotePressed(noteNumber, false);
            juce::Logger::writeToLog(noteName + " released");
        });
    }
    
}


juce::Array<juce::MidiDeviceInfo> MainComponent::getMidiInputs() { //List of available MIDI input devices
    auto midiInputList = juce::MidiInput::getAvailableDevices();
    
#if JUCE_DEBUG  //Only runs following in debug mode
    if (midiInputList.isEmpty()) {
        juce::Logger::writeToLog("No MIDI input devices found.");
    }
    else {
        for (int i = 0; i < midiInputList.size(); i++) {
            juce::Logger::writeToLog("MIDI Input Device " + juce::String(i) + ": " + midiInputList[i].name + " ID: " + midiInputList[i].identifier + ")");
        }
    }
#endif
    return midiInputList;
}


void MainComponent::openMidiInputByIndex(int index) { //Open MIDI input device by index
    auto midiInputList = getMidiInputs();

    //Check if index is valid
    if (index < 0 || index >= midiInputList.size()) {
        closeCurrentMidiInput();
        return;
    }

    //If index device is already open, no change needed
    if (midiInputDevice && midiInputDevice->getIdentifier() == midiInputList[index].identifier) {
        return;
    }

    //If the index exists and is not the currently open device
    closeCurrentMidiInput();

    //Open new device
    midiInputDevice = juce::MidiInput::openDevice(midiInputList[index].identifier, this);

    if (midiInputDevice) {
        midiInputDevice->start();
        juce::Logger::writeToLog("MIDI input device started");
    }
    else {
        juce::Logger::writeToLog("MIDI input device failed to open");
    }

}
void MainComponent::closeCurrentMidiInput() { //Close the current MIDI input device
    if (midiInputDevice) {
        try {
            midiInputDevice->stop();
            midiInputDevice.reset();
			juce::Logger::writeToLog("MIDI input device closed");
        }
        catch (...) {
			//If stop() fails just reset the pointer, helpful if device is disconnected unexpectedly
            midiInputDevice.reset();
			juce::Logger::writeToLog("MIDI input device closed with exception");
        }
    }

    midiDeviceName.clear();
    currentDeviceIndex = -1;
}


void MainComponent::timerCallback() {
    auto midiInputList = getMidiInputs();

	//Check if device count has changed
    if (midiInputList.size() != previousDeviceCount) {
        previousDeviceCount = midiInputList.size();
    
        bool currentDeviceStillExists = false;

        if (currentDeviceIndex >= 0 && currentDeviceIndex < midiInputList.size()) {
            //Check if same device
            if (midiInputDevice) {
                juce::String currentDeviceID = midiInputDevice->getIdentifier();

                //Search for device in new list
                for (int i = 0; i < midiInputList.size(); i++) {
                    if (midiInputList[i].identifier == currentDeviceID) {
                        currentDeviceStillExists = true;
                        break;
                    }
                }
            }
        }


        //If current device no longer exists, close it
        if (!currentDeviceStillExists && midiInputDevice != nullptr) {
            juce::Logger::writeToLog("Current device disconnected.");
            closeCurrentMidiInput();
            currentDeviceIndex = -1;
        }
        toolbar.refreshDevicesList();
    }
}