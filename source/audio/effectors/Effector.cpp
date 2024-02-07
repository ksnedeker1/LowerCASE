#include "Effector.h"

//-----------------------------------------------------------------------------------//

Effector::Effector() {}

Effector::~Effector() {}

//-----------------------------------------------------------------------------------//

void Effector::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) { buffer.clear(); }

//-----------------------------------------------------------------------------------//

const juce::String Effector::getName() const { return "Effector"; }