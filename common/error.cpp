#include "error.h"

Error::Error(std::string message) :
    message(message)
{}

Error::Error(std::string name, std::string message) :
    name(name),
    message(message)
{}

ErrorPtr Error::create(std::string message)
{
    return std::make_unique<Error>(message);
}

ErrorPtr Error::create(std::string name, std::string message)
{
    return std::make_unique<Error>(name, message);
}

ErrorPtr Error::none()
{
    return std::unique_ptr<Error>();
}