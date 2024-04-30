#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

class Logger
{
public:
    Logger();
    Logger(Logger&&) = default;
    Logger(const Logger&) = delete;
    Logger &operator=(Logger&&) = delete;
    Logger &operator=(const Logger&) = delete;
    ~Logger();

    static void Log(const std::string& message);
    static void Warn(const std::string& message);
    static void Err(const std::string& message);
};

#endif