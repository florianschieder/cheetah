#include "Logging.h"
#include "DateTimeFormatter.h"

#include <ctime>
#include <map>
#include <iostream>
#include <sstream>
#include <crtdbg.h>

namespace cheetah
{
    Logger::Logger(std::wstring componentName)
    {
        this->componentName = componentName;
    }

    // TODO replace with std::format (c++ 20 thank you ;))
    std::wstring formatLine(std::wstring msg,
                            std::wstring componentName,
                            LoggerLevel level)
    {
        auto line = std::wstringstream();

        std::wstring readableLevel;

        switch (level) {
        case LoggerLevel::DEBUG:
            readableLevel = L"[debug]";
            break;
        case LoggerLevel::INFO:
            readableLevel = L"[info]";
            break;
        case LoggerLevel::WARNING:
            readableLevel = L"[warning]";
            break;
        case LoggerLevel::CRITICAL:
            readableLevel = L"[critical]";
            break;
        default:
            // TODO
            throw std::exception("unknown level");
            break;
        }

        // e.g. <timestamp> [main] [info] Hi! :)
        line
            << DateTimeFormatter::getCurrentTimeStamp()
            << L" ["
            << componentName
            << L"] "
            << readableLevel
            << L" "
            << msg
            << std::endl;

        return line.str();
    }

    StdLogger::StdLogger(std::wstring label)
        : Logger::Logger(label)
    {}

    void StdLogger::log(std::wstring msg, LoggerLevel level)
    {
        auto stream = ((level == LoggerLevel::CRITICAL)
                       ? &std::wcerr
                       : &std::wcout);

        *stream << formatLine(msg, this->componentName, level);
    }

    MSVCDebugLogger::MSVCDebugLogger(std::wstring label)
        : Logger::Logger(label)
    {}

    void MSVCDebugLogger::log(std::wstring msg, LoggerLevel level)
    {
        OutputDebugStringW(
            formatLine(msg, this->componentName, level).c_str());
    }
}
