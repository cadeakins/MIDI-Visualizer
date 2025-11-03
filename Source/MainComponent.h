#pragma once

#include <JuceHeader.h>
#include <memory>
#include "PianoKeyboard.h"
#include "Toolbar.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component, public juce::MidiInputCallback
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override; //The callback for incoming MIDI messages
    juce::Array<juce::MidiDeviceInfo> getMidiInputs(); //List of available MIDI input devices
private:
    //==============================================================================
    PianoKeyboard pianoKeyboard; //Instance of the PianoKeyboard component
    Toolbar toolbar; //Instance of the Toolbar component

	std::unique_ptr<juce::MidiInput> midiInputDevice; //Pointer to the MIDI input device
    juce::String midiDeviceName;    //Name of connected MIDI device

    //Helper functions
	void openMidiInputByIndex(int index); //Open MIDI input device by index
	void closeCurrentMidiInput(); //Close the current MIDI input device

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
