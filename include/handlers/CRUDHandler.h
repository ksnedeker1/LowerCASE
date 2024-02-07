#pragma once

#include <string>
#include "ChainController.h"
/*------------------------------------------------------------------------------------\
| Interface for module-specific CRUD handlers
\------------------------------------------------------------------------------------*/
class IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    virtual void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) = 0;
    virtual void read(int nodeId, ChainController& chainController) = 0;
    virtual void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) = 0;
    virtual void del(int nodeId, ChainController& chainController) = 0;
    virtual ~IModuleCRUDHandler() {}
};

/*------------------------------------------------------------------------------------\
| Maps handlers to their associated moduleName strings
\------------------------------------------------------------------------------------*/
class HandlerFactory
{
public:
    //-----------------------------------------------------------------------------------//

    HandlerFactory();
    IModuleCRUDHandler* getHandler(const std::string& moduleName);

private:
    //-----------------------------------------------------------------------------------//

    std::map<std::string, std::unique_ptr<IModuleCRUDHandler>> moduleHandlers;
};

/*------------------------------------------------------------------------------------\
| Handles 'CRUD' messages
\------------------------------------------------------------------------------------*/
class CRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    virtual ~CRUDHandler() {}
    
    //-----------------------------------------------------------------------------------//

    void handle(const std::string& message, ChainController& chainController);
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to a Beeper node
\------------------------------------------------------------------------------------*/
class BeeperHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to a SimpleOsc node
\------------------------------------------------------------------------------------*/
class SimpleOscHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to a SimpleReverb node
\------------------------------------------------------------------------------------*/
class SimpleReverbHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to a Delay node
\------------------------------------------------------------------------------------*/
class DelayHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to a Gain node
\------------------------------------------------------------------------------------*/
class GainHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to an Out node
\------------------------------------------------------------------------------------*/
class OutHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};

/*------------------------------------------------------------------------------------\
| Handles CRUD related to Chains
\------------------------------------------------------------------------------------*/
class ChainHandler : public IModuleCRUDHandler
{
public:
    //-----------------------------------------------------------------------------------//

    void create(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void read(int nodeId, ChainController& chainController) override;
    void update(int nodeId, const std::vector<std::string>& params, ChainController& chainController) override;
    void del(int nodeId, ChainController& chainController) override;
};