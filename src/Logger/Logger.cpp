#include <chrono>
#include <ctime>
#include <string>
#include "Logger.h"

namespace {
    std::string getCurrentDayAndTimeToString() {
        std::time_t time{std::time({})};
        char timeString[std::size("dd/mm/yyyy hh:mm:ss")]{};

        std::strftime(std::data(timeString), std::size(timeString), "%d/%m/%Y %X", std::localtime(&time));

        return timeString;
    }

    std::string greenColor{"\x1b[32m"};
    std::string yellowColor{"\x1b[33m"};
    std::string redColor{"\x1b[31m"};
    std::string noColor{"\x1b[0m"};

}

void Logger::Log(const std::string& message) {
    std::cout << greenColor << "INFO: [ " << getCurrentDayAndTimeToString() << " ] - " << message << noColor << '\n';
}

void Logger::Warn(const std::string& message) {
    std::cout << yellowColor << "WARNING: [ " << getCurrentDayAndTimeToString() << " ] - " << message << noColor << '\n';
}

void Logger::Err(const std::string& message) {
    std::cout << redColor << "ERROR: [ " << getCurrentDayAndTimeToString() << " ] - " << message << noColor << '\n';
}