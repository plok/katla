#include "error.h"

Error::Error(const std::error_code errorCode) :
    m_errorCode(std::move(errorCode))
{}

Error::Error(const std::error_code errorCode, std::string description, std::string info) :
    m_errorCode(errorCode),
    m_description(description),
    m_info(info)
{}
