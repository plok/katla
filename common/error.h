#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <memory>
#include <optional>

class Error;
typedef std::unique_ptr<Error> ErrorPtr;

class Error
{
public:
    Error(std::string message);
    static ErrorPtr create(std::string message);
    static ErrorPtr none();

    const std::string message;
};

#endif