/*
  ==============================================================================

    PianoKeyboard.h
    Created: 31 Oct 2025 6:18:38pm
    Author:  Cade Medearis

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> 
#include <memory>
#include <set>

class PianoKeyboard : public juce::Component
{
public:
    PianoKeyboard();
    ~PianoKeyboard() override;
    //==============================================================================
    void paint(juce::Graphics&) override;
    void setNotePressed(int noteNumber, bool isPressed);


private:
    bool isWhiteKey(int midiNoteNumber);
    std::set<int> activeNotes;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoKeyboard)
};