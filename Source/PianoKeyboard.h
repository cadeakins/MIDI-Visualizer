/*
  ==============================================================================

    PianoKeyboard.h
    Created: 31 Oct 2025 6:18:38pm
    Author:  Cade Medearis

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> 

class PianoKeyboard : public juce::Component
{
public:
    PianoKeyboard();
    ~PianoKeyboard() override;
    //==============================================================================
    void paint(juce::Graphics&) override;


private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoKeyboard)
};