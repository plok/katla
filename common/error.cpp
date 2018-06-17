#include "error.h"

Error::Error(std::string message) :
    message(message)
{}

ErrorPtr Error::create(std::string message)
{
    return std::make_unique<Error>(message);
}

ErrorPtr Error::none()
{
    return std::unique_ptr<Error>();
}