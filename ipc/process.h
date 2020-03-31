#ifndef KATLA_PROCESS_H
#define KATLA_PROCESS_H

#include "katla-ipckit.h"

#include "common/error.h"
#include <memory>

class CoreApplication;
class RenderView;

class KATLA_IPCKIT_DECLSPEC Process {
public:
    explicit Process(CoreApplicationUv& coreApp);
    virtual ~Process();
    
private:
    CoreApplicationUv* _coreApp;

};

#endif