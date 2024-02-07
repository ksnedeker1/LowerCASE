#pragma once

#include <string>
#include "ChainController.h"
#include "CRUDHandler.h"
#include "ActionHandler.h"

/*------------------------------------------------------------------------------------\
| Dispatches a message to the appropriate handler
\------------------------------------------------------------------------------------*/
class Dispatcher {
public:
    //-----------------------------------------------------------------------------------//

    Dispatcher(CRUDHandler* ch, ActionHandler* ah);
    ~Dispatcher();

    //-----------------------------------------------------------------------------------//

    void dispatch(const std::string& message, ChainController& chainController);

private:
    //-----------------------------------------------------------------------------------//

    CRUDHandler* crudHandler;
    ActionHandler* actionHandler;
};