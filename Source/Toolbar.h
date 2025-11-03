/*
  ==============================================================================

    Toolbar.h
    Created: 2 Nov 2025 2:59:25pm
    Author:  Cade Medearis

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class Toolbar : public juce::Component,
                private juce::ComboBox::Listener
{
public:
    Toolbar();  //Constructor
    ~Toolbar() override;    //Destructor

    void paint(juce::Graphics& g) override; //Paint method
    void resized() override;    //Resized method

	void refreshDevicesList(); //Refresh the list of MIDI devices
	void setDeviceChangeCallback(std::function<void(int)> callback); //Set the callback for when the device is changed

private:
	juce::Font textFont{ 12.0f }; //Font for text elements
 
    juce::ComboBox midiDropdown; //Dropdown for selecting MIDI devices
	juce::Label deviceLabel; //Label for the dropdown

	std::function<void(int)> deviceChangeCallback; //Callback function for device change

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Toolbar)
};