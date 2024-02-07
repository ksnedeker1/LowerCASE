#include "Dispatcher.h"

//-----------------------------------------------------------------------------------//

Dispatcher::Dispatcher(CRUDHandler* ch, ActionHandler* ah) : crudHandler(ch), actionHandler(ah) {}

Dispatcher::~Dispatcher() {}

//-----------------------------------------------------------------------------------//
// Pass message to a handler according to prefix

void Dispatcher::dispatch(const std::string& message, ChainController& chainController) 
{
    if (message.find("CRUD ") == 0) 
    {
        std::string remainingMessage = message.substr(5);
        crudHandler->handle(remainingMessage, chainController);
    }
    else if (message.find("ACTN ") == 0) 
    {
        std::string remainingMessage = message.substr(5);
        actionHandler->handle(remainingMessage, chainController);
    }
}
