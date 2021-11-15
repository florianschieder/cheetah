#ifndef __CHEETAH_LOGGING
#define __CHEETAH_LOGGING

#include <memory>
#include <string>

namespace cheetah
{
    enum LoggerLevel
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        CRITICAL = 3,
    };

    enum LoggerType
    {
        STD_LOGGER = 0,
        MSVC_DEBUG_LOGGER = 1,
    };

    class Logger
    {
    public:
        Logger(std::wstring componentName);
        virtual void log(std::wstring msg, LoggerLevel level) = 0;

    protected:
        std::wstring componentName;
    };

    /*
    Loggs messages on stdout or stderr.
     */
    class StdLogger : public Logger
    {
    public:
        StdLogger(std::wstring label);
        void log(std::wstring msg, LoggerLevel level);
    };

    /**
    Useful for debugging within Visual Studio.
    Uses the CRT functionality regardless of the logger level so the output
    shows up in the output console.
     */
    class MSVCDebugLogger : public Logger
    {
    public:
        MSVCDebugLogger(std::wstring label);
        void log(std::wstring msg, LoggerLevel level);
    };
}

#endif
