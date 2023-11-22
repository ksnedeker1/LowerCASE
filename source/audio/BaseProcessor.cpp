#include <JuceHeader.h>
#include "BaseProcessor.h"

BaseProcessor::BaseProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

BaseProcessor::~BaseProcessor() {}

void BaseProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}

void BaseProcessor::releaseResources() {}

void BaseProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {}

void BaseProcessor::reset() {}

const juce::String BaseProcessor::getName() const { return "BaseProcessor"; }

bool BaseProcessor::acceptsMidi() const { return false; } // May need to change this or implement in child classes

bool BaseProcessor::producesMidi() const { return false; } // May need to change this or implement in child classes

bool BaseProcessor::isMidiEffect() const { return false; } // May need to change this or implement in child classes

double BaseProcessor::getTailLengthSeconds() const { return 0.0; }

juce::AudioProcessorEditor* BaseProcessor::createEditor() { return nullptr; }

bool BaseProcessor::hasEditor() const { return false; }

int BaseProcessor::getNumPrograms() { return 1; }

int BaseProcessor::getCurrentProgram() { return 0; }

void BaseProcessor::setCurrentProgram(int index) {}

const juce::String BaseProcessor::getProgramName(int index) { return {}; }

void BaseProcessor::changeProgramName(int index, const juce::String& newName) {}

void BaseProcessor::getStateInformation(juce::MemoryBlock& destData) {}

void BaseProcessor::setStateInformation(const void* data, int sizeInBytes) {}
