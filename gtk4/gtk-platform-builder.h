#ifndef KATLA_GTK_PLATFORM_BUILDER_H
#define KATLA_GTK_PLATFORM_BUILDER_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "ui/platform-builder.h"
#include "ui/application.h"
#include "ui/column.h"
#include "ui/button.h"
#include "ui/text.h"

#include <memory>

namespace katla {

class GtkPlatformBuilder : public PlatformBuilder {
public:
    GtkPlatformBuilder() = default;
    virtual ~GtkPlatformBuilder() = default;

    std::unique_ptr<Application> createApplication(int argc, char* argv[], std::string appName);
    std::unique_ptr<Column> createColumn();
    std::unique_ptr<Button> createButton();
    std::unique_ptr<Text> createText();
};

}

#endif
