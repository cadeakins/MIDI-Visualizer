#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
    //Seeing what MIDI devices are available
    auto midiInputList = juce::MidiInput::getAvailableDevices();
    if (midiInputList.isEmpty()) {
        juce::Logger::writeToLog("No MIDI input devices found.");
    } 
    else {
        for (int i = 0; i < midiInputList.size(); i++) {
            //g.drawText(midiInputs[i].name + " is available.", getLocalBounds(), juce::Justification::centred, true);
            juce::Logger::writeToLog("MIDI Input Device " + juce::String(i) + ": " + midiInputList[i].name + " ID: " + midiInputList[i].identifier + ")");
        }
	    //MY SPECIFIC DEVICE, CHANGE TO BE DYNAMIC LATER
        midiInputDevice = juce::MidiInput::openDevice(midiInputList[4].identifier, this);
    }
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);


    //g.drawText("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}


void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) {
	// Handle incoming MIDI messages here
    int noteNumber = message.getNoteNumber();
    juce::String noteName = message.getMidiNoteName(noteNumber, true, true, 4);
    juce::uint8 noteVelocity = message.getVelocity();
    if (message.isNoteOn()) {
        juce::Logger::writeToLog(noteName + "pressed with velocity " + juce::String(noteVelocity));
    }
    else if (message.isNoteOff()) {
		juce::Logger::writeToLog(noteName + " released");
    }
    
}