#include <utility>

#include "error.h"

Error::Error(std::string message) :
    message(std::move(message))
{}

Error::Error(std::string name, std::string message) :
    name(std::move(name)),
    message(std::move(message))
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