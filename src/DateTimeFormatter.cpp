#include "DateTimeFormatter.h"

#include <ctime>

namespace cheetah
{
    #pragma warning (disable : 4996) // TODO goal: remove it

    CHEETAH_API std::wstring DateTimeFormatter::getCurrentTimeStamp()
    {
        time_t tt;
        struct tm* ti;

        time(&tt);
        ti = localtime(&tt); // TODO why is this unsafe?

        // TODO: use safer variant. I agree with MSVC regarding the unsafety.
        char rawResult[20];
        sprintf(rawResult, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
                1900  + ti->tm_year, ti->tm_mon, ti->tm_mday,
                ti->tm_hour, ti->tm_min, ti->tm_sec);
        std::string result(rawResult);

        return std::wstring(result.begin(), result.end());
    }
}
