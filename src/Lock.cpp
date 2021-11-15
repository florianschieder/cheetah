#include <Lock.h>
#include <Logging.h>

#include <exception>
#include <string>

namespace cheetah
{
    Lock::Lock(std::wstring label)
    {
        this->label = label;
    }
    Lock::~Lock() {}

    void Lock::acquire() {}
    void Lock::free() {}

    std::shared_ptr<Lock> Lock::createLock(std::wstring label)
    {
        #ifdef _WIN32

        return std::make_shared<_WinLock>(label);

        #else // TODO

        return std::make_shared<_PosixLock>(label);

        #endif
    }

    #ifdef _WIN32
        _WinLock::_WinLock(std::wstring label)
            : Lock::Lock(label)
        {
            // TODO prefix with lock_
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

        _WinLock::~_WinLock()
        {
            this->logger->log(L"destroying lock.", LoggerLevel::DEBUG);
            DeleteCriticalSection(&this->_criticalSection);
        }

        void _WinLock::acquire()
        {
            this->logger->log(L"acquired lock.", LoggerLevel::DEBUG);
            EnterCriticalSection(&this->_criticalSection);
        }

        void _WinLock::free()
        {
            this->logger->log(L"freed lock.", LoggerLevel::DEBUG);
            LeaveCriticalSection(&this->_criticalSection);
        }
    #else // TODO
        _PosixLock::_PosixLock(std::wstring label)
            : Lock::Lock(label)
        {
            // TODO prefix with lock_
            this->logger = std::make_shared<StdLogger>(label);

            if (pthread_mutex_init(&this->mutex, 0) != 0) {
                this->logger->log(L"failed to create lock.",
                                  LoggerLevel::CRITICAL);

                throw std::runtime_error("failed to instantiate pthread mutex.");
            }

            this->logger->log(L"created new lock.", LoggerLevel::DEBUG);
        }

        _PosixLock::~_PosixLock()
        {
            this->logger->log(L"destroying lock.", LoggerLevel::DEBUG);
            pthread_mutex_destroy(&this->mutex);
        }

        void _PosixLock::acquire()
        {
            this->logger->log(L"acquired lock.", LoggerLevel::DEBUG);
            pthread_mutex_lock(&this->mutex);
        }

        void _PosixLock::free()
        {
            this->logger->log(L"freed lock.", LoggerLevel::DEBUG);
            pthread_mutex_unlock(&this->mutex);
        }
    #endif
}
