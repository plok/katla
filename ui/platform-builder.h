#ifndef KATLA_PLATFORM_BUILDER_H
#define KATLA_PLATFORM_BUILDER_H

#include "application.h"
#include "column.h"
#include "button.h"
#include "text.h"

#include <memory>

namespace katla {

class PlatformBuilder {
public:
    PlatformBuilder() = default;
    virtual ~PlatformBuilder() = default;

    virtual std::unique_ptr<Application> createApplication(int argc, char* argv[], std::string appName) = 0;
    virtual std::unique_ptr<Column> createColumn() = 0;
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<Text> createText() = 0;
};

}

#endif
