/*
  ==============================================================================

    Toolbar.cpp
    Created: 2 Nov 2025 2:59:25pm
    Author:  Cade Medearis

  ==============================================================================
*/

#include "Toolbar.h"


Toolbar::Toolbar() {  //Constructor
	addAndMakeVisible(midiDropdown); //Add dropdown to the toolbar
	midiDropdown.addListener(this); //Listen for changes in the dropdown

    refreshDevicesList(); //Populate the MIDI devices list
}
Toolbar::~Toolbar() {}  //Destructor

void Toolbar::paint(juce::Graphics& g) {  //Paint method
	//Draw background with shadow effect
    g.setColour(juce::Colour(0xFF666666));  //Lighter gray for toolbar
	g.fillRect(0, 0, getWidth(), getHeight() - 7); //Fill the toolbar background
    g.drawRect(0, 0, getWidth(), getHeight() - 7); //Draw the toolbar background

    juce::ColourGradient shadowGradient(
        juce::Colour(0x70000000), //Semi-transparent black at the top
        0.0f, getHeight() - 7.0f,  //Start point (slightly above bottom)
        juce::Colour(0x00000000), //Fully transparent at the bottom
		0.0f, (float)getHeight(), //End point (bottom)
        false
    );

    g.setGradientFill(shadowGradient);
	g.fillRect(0, getHeight() - 7, getWidth(), 7); //Draw the shadow at the bottom of the toolbar
}
void Toolbar::resized() {    //Resized method
	midiDropdown.setBounds(10, 10, 200, 23); //Position the dropdown
}


void Toolbar::refreshDevicesList() { //Refresh the list of MIDI devices
    auto midiInputList = juce::MidiInput::getAvailableDevices();//Get the list of MIDI input devices

    midiDropdown.clear(juce::dontSendNotification); //Clear existing items

    for (int i = 0; i < midiInputList.size(); i++) {
        midiDropdown.addItem(midiInputList[i].name, i + 1); //Add each device to the dropdown
    }
}
    void Toolbar::setDeviceChangeCallback(std::function<void(int)> callback) { //Set the callback for when the device is changed
		deviceChangeCallback = std::move(callback);
    }



    void Toolbar::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) {
        if (comboBoxThatHasChanged == &midiDropdown) {
			int selectedIndex = midiDropdown.getSelectedId() - 1; //Get the selected device index
            if (deviceChangeCallback) {
				deviceChangeCallback(selectedIndex); //Invoke the callback with the selected index
            }
        }
    }