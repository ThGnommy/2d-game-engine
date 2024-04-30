#include <chrono>
#include <ctime>
#include <string>
#include "Logger.h"

namespace {
    std::string getFormattedDayAndTime() {
        std::time_t time{std::time({})};
        char timeString[std::size("dd/mm/yyyy hh:mm:ss")]{};

        std::strftime(std::data(timeString), std::size(timeString), "%d/%m/%Y %X", std::localtime(&time));

        return timeString;
    }

    std::string greenColor{"\x1b[32m"};
    std::string redColor{"\x1b[31m"};
    std::string noColor{"\x1b[0m"};

}

void Logger::Log(const std::string& message) {
    std::cout << greenColor << "LOG: [ " << getFormattedDayAndTime() << " ] - " << message << noColor << '\n';
}

void Logger::Err(const std::string& message) {
    std::cout << redColor << "LOG: [ " << getFormattedDayAndTime() << " ] - " << message << noColor << '\n';
}