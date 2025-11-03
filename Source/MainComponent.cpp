#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent()  //Default constructor
{
    setSize(600, 400);
    //Testing PianoKeyboard component
    addAndMakeVisible(pianoKeyboard);
    addAndMakeVisible(toolbar);

    toolbar.setDeviceChangeCallback([this](int deviceIndex) {
        openMidiInputByIndex(deviceIndex);
        });
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
    g.setColour (juce::Colours::white);

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
        juce::Logger::writeToLog(noteName + "pressed with velocity " + juce::String(noteVelocity));
        
        if (message.getVelocity() > 0) {
            juce::MessageManager::callAsync([this, noteNumber, noteVelocity]() {
                pianoKeyboard.setNotePressed(noteNumber, true, noteVelocity);
            });
        }
        else {  //Since some MIDI devices send Note On with velocity 0 instead of Note Off
            juce::MessageManager::callAsync([this, noteNumber]() {
                pianoKeyboard.setNotePressed(noteNumber, false);
            });
        }
        
    }
	else if (message.isNoteOff()) { //Hop onto UI thread instead of MIDI thread
		juce::Logger::writeToLog(noteName + " released");
        juce::MessageManager::callAsync([this, noteNumber]() {
            pianoKeyboard.setNotePressed(noteNumber, false);
        });
    }
    
}


juce::Array<juce::MidiDeviceInfo> MainComponent::getMidiInputs() { //List of available MIDI input devices
    auto midiInputList = juce::MidiInput::getAvailableDevices();
    if (midiInputList.isEmpty()) {
        juce::Logger::writeToLog("No MIDI input devices found.");
    }
    else {
        for (int i = 0; i < midiInputList.size(); i++) {
            juce::Logger::writeToLog("MIDI Input Device " + juce::String(i) + ": " + midiInputList[i].name + " ID: " + midiInputList[i].identifier + ")");
        }
    }
    return midiInputList;
}


void MainComponent::openMidiInputByIndex(int index) { //Open MIDI input device by index
    auto midiInputList = getMidiInputs();
    //Check if index is valid
    if (index < 0 || index >= midiInputList.size()) {
        closeCurrentMidiInput();
        return;
    }

    //If index device is already open
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
        midiInputDevice->stop();
        midiInputDevice.reset();
    }
}