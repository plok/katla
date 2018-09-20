#ifndef PHYSICAL_DEVICE_H
#define PHYSICAL_DEVICE_H

#include "common/error.h"

#include <memory>
#include <vector>

class PhysicalDevice;
typedef std::shared_ptr<PhysicalDevice> PhysicalDevicePtr;

class PhysicalDevice
{
public:    
    virtual void printInfo() = 0;
    virtual ErrorPtr validateForGraphics() = 0;
};

#endif
