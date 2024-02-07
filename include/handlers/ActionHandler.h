#pragma once

#include <string.h>
#include "ChainController.h"

/*------------------------------------------------------------------------------------\
| Interface for listeners to ActionHandler
\------------------------------------------------------------------------------------*/
class IActionListener {
public:
    //-----------------------------------------------------------------------------------//

    virtual ~IActionListener() = default;

    //-----------------------------------------------------------------------------------//

    virtual void toggleAudio() = 0;
};


/*------------------------------------------------------------------------------------\
| Handles 'Action' messages
\------------------------------------------------------------------------------------*/
class ActionHandler {
public:
    //-----------------------------------------------------------------------------------//

    ActionHandler(IActionListener* listener) : listener(listener) {}
    virtual ~ActionHandler() {}

    //-----------------------------------------------------------------------------------//

    void handle(const std::string& message, ChainController& chainController);

private:
    //-----------------------------------------------------------------------------------//

    IActionListener* listener;
};