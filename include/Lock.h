#ifndef __CHEETAH_LOCK
#define __CHEETAH_LOCK

#include <memory>
#include <string>

#include <Logging.h>

#include <common.h>

namespace cheetah
{
    class Lock
    {
    public:
        CHEETAH_API static std::shared_ptr<Lock> createLock(
            std::wstring label);

        CHEETAH_API Lock(std::wstring label);
        CHEETAH_API virtual ~Lock();
        CHEETAH_API virtual void acquire();
        CHEETAH_API virtual void free();

    protected:
        std::wstring label;
    };

    #ifdef _WIN32
    class _WinLock : public Lock
        {
        public:
            CHEETAH_API _WinLock(std::wstring label);
            CHEETAH_API ~_WinLock();

            CHEETAH_API void acquire();
            CHEETAH_API void free();

        private:
            CRITICAL_SECTION _criticalSection;
            std::shared_ptr<MSVCDebugLogger> logger;
        };
    #else // TODO more concrete!
        class _PosixLock : public Lock
        {
        public:
            CHEETAH_API _PosixLock(std::wstring label);
            CHEETAH_API ~_PosixLock();

            CHEETAH_API void acquire();
            CHEETAH_API void free();

        private:
            pthread_mutex_t mutex;
            std::shared_ptr<StdLogger> logger;
        };
    #endif
}

#endif
