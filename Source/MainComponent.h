#pragma once

#include <JuceHeader.h>
#include <memory>

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
    void paint (juce::Graphics&) override;
    void resized() override;

	void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override; //The callback for incoming MIDI messages

private:
    //==============================================================================
    // Your private member variables go here...
	std::unique_ptr<juce::MidiInput> midiInputDevice; //Pointer to the MIDI input device

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
