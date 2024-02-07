#include "ActionHandler.h"

//-----------------------------------------------------------------------------------//

void ActionHandler::handle(const std::string& message, ChainController& chainController) 
{
    if (message == "TOGGLE_AUDIO") {
        if (listener) {
            listener->toggleAudio();
        }
    }
}