# MIDI Keyboard Visualizer

Real-time MIDI keyboard visualization built with the JUCE framework and C++.

<p align="center">
  <img width="1213" height="681" alt="image" src="https://github.com/user-attachments/assets/760648d3-f343-4f49-b356-2b2adc4b5758" />
  <p4> Wireframe for final product design made with Figma </p4>
</p>

# Current Status
In development. Working demo is able to connect to MIDI device, display MIDI input onto an 88-key piano roll with velocity-based color coding. 

# Planned Features
✓ Real-time input from MIDI keyboard
✓ Piano keyboard visualization
✓ Velocity-based color coding
✓ Midi Device selection
- Open and visualize MIDI files
- Create falling animation for notes
- Volume slider
- Tempo slider
- Outputs sound
- Handle more MIDI messages such as pitchbend, aftertouch, and control change (mainly sustain pedal support)

# Using
- JUCE Framework 8.0.10 
- C++23
- Visual Studio 2022

# Build Instructions
1. Install JUCE from https://juce.com/get-juce
2. Open 'MIDIVisualizer.jucer' in Projucer
3. Click "Save Project and Open in IDE" (The VS button)
4. Build in Visual Studio
5. Run 

# Demo Video

https://youtu.be/cH4k4l8_GFI
