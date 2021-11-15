#include "DateTimeFormatter.h"

#include <chrono>
#include <format>

namespace cheetah
{
    CHEETAH_API std::wstring DateTimeFormatter::getCurrentTimeStamp()
    {
        auto const time = std::chrono::current_zone()
            ->to_local(std::chrono::system_clock::now());
        return std::format(L"{:%Y-%m-%d %X}", time);
    }
}
