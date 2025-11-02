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
#include <map>

class PianoKeyboard : public juce::Component
{
public:
    PianoKeyboard();
    ~PianoKeyboard() override;
    //==============================================================================
    void paint(juce::Graphics&) override;
    void setNotePressed(int noteNumber, bool isPressed, int velocity = 64);  //Default value of 64 for backwards compatibility


private:
    // Ordered map of notes being pushed down
    std::map<int, int> activeNotes;
    // Key colors
    const juce::Colour whiteKeyColor{ 0xFFF3F3F3 };
    const juce::Colour blackKeyColor{ 0xFF303030 };
    const juce::Colour borderColor{ 0xFF1A1A1A };

    // Velocity gradient colors
    const juce::Colour velocityGreen{ 0xFF6BCF7F };
    const juce::Colour velocityYellow{ 0xFFF8F09B };
    const juce::Colour velocityRed{ 0xFFFF6B6B };

    // Private functions
    bool isWhiteKey(int midiNoteNumber);
    juce::Colour getVelocityColor(int noteNumber);
    
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoKeyboard)
};