#include "common.h"

#include <Logging.h>

#include <codecvt>
#include <exception>
#include <list>
#include <memory>
#include <string>

// TODO raus hier damit
namespace cheetah
{
    class Application
    {
        public:
            Application(std::list<std::wstring> arguments);
            int run();
            void cleanUpResources() noexcept(true);
            std::shared_ptr<cheetah::StandardLogger> getLogger();

        private:
            std::shared_ptr<cheetah::StandardLogger> logger;
            std::list<std::wstring> arguments;
    };

    class BaseException : public std::exception
    {};
}

cheetah::Application::Application(std::list<std::wstring> arguments)
{
    this->arguments = arguments;
    this->logger = std::make_shared<cheetah::StandardLogger>(L"main");
}

std::shared_ptr<cheetah::StandardLogger> cheetah::Application::getLogger()
{
    return this->logger;
}

int cheetah::Application::run()
{
    MessageBoxW(GetDesktopWindow(),
        L"Das hier ist mein Text",
        L"Cheetah",
        MB_OKCANCEL | MB_ICONINFORMATION);
    return EXIT_SUCCESS;
}

// Free up resources in case the program has to terminate.
// Guarantee no-exception safety.
void cheetah::Application::cleanUpResources() noexcept(true)
{
}

namespace cheetah
{
    std::wstring StdUnicodeStringFromAnsiString(std::string in);
}

std::wstring cheetah::StdUnicodeStringFromAnsiString(std::string in)
{
    LPWSTR out = {};
    memset(&out, 0, in.length() * 2 + 1);

    ::MultiByteToWideChar(
        CP_UTF8,
        0,
        in.c_str(),
        in.length(),
        out,
        in.length() * 2 + 1);

    out[in.length()] = L'\0';

    return std::wstring(out);
}

// =====
// TODO Mention somewhere:
// - cheetah icon: <div>Icons made by <a href="https://www.freepik.com" title="Freepik">Freepik</a> from <a href="https://www.flaticon.com/" title="Flaticon">www.flaticon.com</a></div>

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine,
                    _In_ int nCmdShow)
{
    int argumentsCount;
    std::list<std::wstring> arguments;

    auto rawArguments = ::CommandLineToArgvW(pCmdLine, &argumentsCount);

    for (int i = 0; i < argumentsCount; i++) {
        arguments.push_back(rawArguments[i]);
    }

    auto application = std::make_unique <cheetah::Application>(
        arguments
    );

    int retCode;

    try {
        retCode = application->run();
    }
    catch (const cheetah::BaseException& e) {
        // Log error.
        retCode = EXIT_FAILURE;
    }
    catch (std::exception& e) {
        // Should regularly not occur.
        retCode = EXIT_FAILURE;
}
    catch (...) {
        // Log error. Something really strange happened.
        retCode = EXIT_FAILURE;
    }
    
    application->cleanUpResources();
    application->getLogger()->log(
        L"cheetah is terminating now.",
        cheetah::LoggerLevel::INFO);

    return retCode;
}