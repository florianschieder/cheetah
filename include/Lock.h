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
        CHEETAH_API Lock(std::wstring label);
        CHEETAH_API virtual ~Lock();
        CHEETAH_API virtual void acquire();
        CHEETAH_API virtual void free();

    private:
        CRITICAL_SECTION _criticalSection;
        std::shared_ptr<MSVCDebugLogger> logger;
        std::wstring label;
    };
}

#endif // __CHEETAH_LOCK