#include <JuceHeader.h>
#include "MainComponent.h"

//-----------------------------------------------------------------------------------//

class LowerCASEApplication : public juce::JUCEApplication
{
public:
    //-----------------------------------------------------------------------------------//

    LowerCASEApplication() {}

    //-----------------------------------------------------------------------------------//

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //-----------------------------------------------------------------------------------//

    void initialise(const juce::String& commandLine) override
    {
        mainComponent = std::make_unique<MainComponent>();
    }

    void shutdown() override { mainComponent = nullptr; }

    //-----------------------------------------------------------------------------------//

    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override {}

private:
    //-----------------------------------------------------------------------------------//

    std::unique_ptr<MainComponent> mainComponent;
};

//-----------------------------------------------------------------------------------//

START_JUCE_APPLICATION(LowerCASEApplication)
