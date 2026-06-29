#ifdef _WIN32
#include <windows.h>
#endif

#include <clocale>

#include "console_interface.h"

int main()
{
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::setlocale(LC_ALL, ".UTF-8");

    lab4::ConsoleInterface application;
    application.Run();

    return 0;
}