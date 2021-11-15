#include <Lock.h>
#include <Logging.h>

#include <exception>
#include <string>

namespace cheetah
{
    Lock::Lock(std::wstring label)
    {
        this->label = label;
        this->logger = std::make_shared<MSVCDebugLogger>(label);

        memset(&this->_criticalSection, 0x0, sizeof(CRITICAL_SECTION));
        InitializeCriticalSection(&this->_criticalSection);

        int err_code = GetLastError();
        if (err_code != ERROR_SUCCESS) {
            this->logger->log(L"failed to create lock.",
                LoggerLevel::CRITICAL);
            throw std::exception(
                "failed to instantiate critical section object");
        }

        this->logger->log(L"created new lock.", LoggerLevel::DEBUG);
    }
    Lock::~Lock()
    {
        this->logger->log(L"destroying lock.", LoggerLevel::DEBUG);
        DeleteCriticalSection(&this->_criticalSection);
    }

    void Lock::acquire()
    {
        this->logger->log(L"acquired lock.", LoggerLevel::DEBUG);
        EnterCriticalSection(&this->_criticalSection);
    }

    void Lock::free()
    {
        this->logger->log(L"freed lock.", LoggerLevel::DEBUG);
        LeaveCriticalSection(&this->_criticalSection);
    }
}