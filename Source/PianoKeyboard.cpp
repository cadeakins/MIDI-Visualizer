/*
  ==============================================================================

    PianoKeyboard.cpp
    Created: 31 Oct 2025 6:24:53pm
    Author:  Cade Medearis

  ==============================================================================
*/

#include "PianoKeyboard.h"

//==============================================================================

PianoKeyboard::PianoKeyboard() {}
PianoKeyboard::~PianoKeyboard() {}

//==============================================================================

void PianoKeyboard::paint(juce::Graphics& g) {  //Override the paint method to draw the piano keyboard
    int startNote = 21; //A0
	int endNote = 108; //C8
	int numWhiteKeys = 52; //There are 52 white keys on a standard piano

    float whiteKeyWidth = (float)getWidth() / numWhiteKeys;
	float blackKeyWidth = whiteKeyWidth * 0.6f; //Black keys are typically narrower than white keys
	float blackKeyHeight = getHeight() * 0.6f; //Black keys are typically shorter than white keys
    

    int whiteKeyIndex = 0; //Draw white keys
    for (int note = startNote; note <= endNote; note++) {
        if (isWhiteKey(note)) {
			bool isPressed = (activeNotes.find(note) != activeNotes.end()); //Check if the note is currently pressed
            float x = whiteKeyIndex * whiteKeyWidth;    //Position of the white key

            if (isPressed) {
                int velocity = activeNotes[note];

                juce::Colour velocityBasedColor = getVelocityColor(note);   //Get color related with velocity of note
                g.setColour(velocityBasedColor);  //Set the draw color 
                g.fillRect(x, 0.0f, whiteKeyWidth, (float)getHeight()); //Draw filled rectangle
                g.setColour(juce::Colours::lightgrey);  
                g.drawRect(x, 0.0f, whiteKeyWidth, (float)getHeight(), 0.5);    //Draw outline
            }
            else {
                g.setColour(juce::Colour(whiteKeyColor));
                g.fillRect(x, 0.0f, whiteKeyWidth, (float)getHeight());
                g.setColour(juce::Colours::lightgrey);
                g.drawRect(x, 0.0f, whiteKeyWidth, (float)getHeight(), 0.5);
            }
            whiteKeyIndex++;
        }
    }

    whiteKeyIndex = 0; //Draw black keys
    for (int note = startNote; note <= endNote; note++) {
		bool isPressed = (activeNotes.find(note) != activeNotes.end());
        if (isWhiteKey(note)) {
            whiteKeyIndex++;
        }
        else {
            if (isPressed) {    //Change color of key when pressed
                float x = (whiteKeyIndex * whiteKeyWidth) - (blackKeyWidth / 2.0f);
                int velocity = activeNotes[note];

                juce::Colour velocityBasedColor = getVelocityColor(note);
                g.setColour(velocityBasedColor);
                g.fillRect(x, 0.0f, blackKeyWidth, blackKeyHeight);
                g.setColour(juce::Colours::lightgrey);
                g.drawRect(x, 0.0f, blackKeyWidth, blackKeyHeight);

            }
            else {  //Return to default piano roll colors 
                float x = (whiteKeyIndex * whiteKeyWidth) - (blackKeyWidth / 2.0f);

                g.setColour(juce::Colour(blackKeyColor));
                g.fillRect(x, 0.0f, blackKeyWidth, blackKeyHeight);
                g.drawRect(x, 0.0f, blackKeyWidth, blackKeyHeight);
            }
        }
     }
    
}


bool PianoKeyboard::isWhiteKey(int midiNoteNumber) const {    //Determine if a MIDI note number corresponds to a white key
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




void PianoKeyboard::setNotePressed(int noteNumber, bool isPressed, int velocity) {
    if (isPressed) {
        activeNotes[noteNumber] = velocity;
		repaint(); // Trigger a repaint to show the pressed key
    }
    else {
        activeNotes.erase(noteNumber);
		repaint(); // Trigger a repaint to show the released key
    }

}


juce::Colour PianoKeyboard::getVelocityColor(int noteNumber) const {
    auto it = activeNotes.find(noteNumber);
    if (it == activeNotes.end()) {
		return juce::Colour(whiteKeyColor); //Return default white key color if note is not active
    }

    int velocity = it->second;
    float normalizedVelocity = velocity / 127.0f;    //Range of 0.0 - 1.0 for gradient

    float t;    //Float to store sub-section normalized velocity to fit the 3 ranges

    if (normalizedVelocity < 0.5f) {   //Green to yellow range
        t = normalizedVelocity / 0.5f;   //Normalize velocity again to the green-yellow range, percent varies 0.0-1.0
        return juce::Colour(velocityGreen).interpolatedWith(velocityYellow, t);
    }
    else {  //Yellow to red range
        t = (normalizedVelocity - 0.5f) / 0.5f;   //Normalize velocity again to the yellow-red range, percent varies 0.0-1.0
        return juce::Colour(velocityYellow).interpolatedWith(velocityRed, t);
    }
}