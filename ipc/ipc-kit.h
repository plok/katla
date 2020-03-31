#ifndef KATLA_IPC_KIT_H
#define KATLA_IPC_KIT_H

#include "katla-ipckit.h"

#include "common/error.h"
#include <memory>

class UvCoreApplication;
class RenderView;

class KATLA_IPCKIT_DECLSPEC IpcKit {
public:
    explicit IpcKit(UvCoreApplication& coreApp);
    virtual ~IpcKit();

    ErrorPtr init();
    
private:
    UvCoreApplication* _coreApp;

};

#endif
