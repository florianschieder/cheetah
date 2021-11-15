#include <gtest/gtest.h>
#include <regex>
#include <DateTimeFormatter.h>

using namespace cheetah;

TEST(DateTimeFormatterTest, testServesProperFormat)
{
    ASSERT_TRUE(std::regex_match(
        DateTimeFormatter::getCurrentTimeStamp(),
        std::wregex(L"\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}")));
}