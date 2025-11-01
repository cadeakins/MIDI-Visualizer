/*
  ==============================================================================

    PianoKeyboard.cpp
    Created: 31 Oct 2025 6:24:53pm
    Author:  Cade Medearis

  ==============================================================================
*/

#include "PianoKeyboard.h"

PianoKeyboard::PianoKeyboard() {

}
PianoKeyboard::~PianoKeyboard() {

}

//==============================================================================
void PianoKeyboard::paint(juce::Graphics& g) {
    int startNote = 21; //A0
	int endNote = 108; //C8
	int numWhiteKeys = 52; //There are 52 white keys on a standard piano

    float whiteKeyWidth = (float)getWidth() / numWhiteKeys;
	float blackKeyWidth = whiteKeyWidth * 0.6f; //Black keys are typically narrower than white keys
	float blackKeyHeight = getHeight() * 0.6f; //Black keys are typically shorter than white keys
    
    juce::uint32 whiteKeyColor = 0xFFF3F3F3;
    juce::uint32 blackKeyColor = 0xFF303030;

    int whiteKeyIndex = 0; //Draw white keys
    for (int note = startNote; note <= endNote; note++) {
        if (isWhiteKey(note)) {
            float x = whiteKeyIndex * whiteKeyWidth;

            g.setColour(juce::Colour(whiteKeyColor));
            g.fillRect(x, 0.0f, whiteKeyWidth, (float)getHeight());
            g.setColour(juce::Colours::lightgrey);
            g.drawRect(x, 0.0f, whiteKeyWidth, (float)getHeight(), 0.5);
            whiteKeyIndex++;
        }
    }

    whiteKeyIndex = 0; //Draw black keys
    for (int note = startNote; note <= endNote; note++) {
        if (isWhiteKey(note)) {
            whiteKeyIndex++;
        }
        else {
            float x = (whiteKeyIndex * whiteKeyWidth) - (blackKeyWidth / 2.0f);

            g.setColour(juce::Colour(blackKeyColor));
            g.fillRect(x, 0.0f, blackKeyWidth, blackKeyHeight);
			g.drawRect(x, 0.0f, blackKeyWidth, blackKeyHeight);
        }
     }
    
}


bool PianoKeyboard::isWhiteKey(int midiNoteNumber) {
    switch (midiNoteNumber % 12) {
        case 0: // C
        case 2: // D
        case 4: // E
        case 5: // F
        case 7: // G
        case 9: // A
        case 11: // B
            return true;
        default:
            return false;
    }
}