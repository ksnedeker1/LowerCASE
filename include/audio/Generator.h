#pragma once

#include "BaseProcessor.h"

/*------------------------------------------------------------------------------------\
| Parent class for Generator modules
\------------------------------------------------------------------------------------*/
class Generator : public BaseProcessor
{
public:
    //-----------------------------------------------------------------------------------//

    Generator();
    virtual ~Generator() override;

    //-----------------------------------------------------------------------------------//

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;
    
    //-----------------------------------------------------------------------------------//

    const juce::String Generator::getName() const override;
};