#include "Generator.h"

Generator::Generator() {}

Generator::~Generator() {}

void Generator::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) { buffer.clear(); }

const juce::String Generator::getName() const { return "Generator"; }
