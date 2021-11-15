#include "Logging.h"
#include "DateTimeFormatter.h"

#include <ctime>
#include <map>
#include <iostream>
#include <sstream>

#ifdef _WIN32
    #include <crtdbg.h>
#endif

// TODO see below for std::runtime_error vs. std::exception
// invent proper compiler-independent exception hierarchy.
// MSVC allows us std::exception(const char*), GCC does not
// let us do this. MSVC, on the other side, does not
// std::runtime_error.
namespace cheetah
{
    Logger::Logger(std::wstring componentName)
    {
        this->componentName = componentName;
    }

    // utility: format line

    // TODO understand what MSVC is complaining about regarding enums
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
            #ifdef _WIN32
            throw std::exception("unknown level");
            #else
            throw std::runtime_error("unknown level");
            #endif // _WIN32
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
    // concrete implementations

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

    #ifdef _WIN32 // TODO aufräumen
        MSVCDebugLogger::MSVCDebugLogger(std::wstring label)
            : Logger::Logger(label)
        {}

        void MSVCDebugLogger::log(std::wstring msg, LoggerLevel level)
        {
            OutputDebugStringW(
                formatLine(msg, this->componentName, level).c_str());
        }
    #endif
}
