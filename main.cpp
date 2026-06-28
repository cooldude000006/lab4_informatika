#include <iostream>

#include "arithmetic_progression_generator.h"

int main()
{
    lab4::ArithmeticProgressionGenerator<int> generator(
        2,
        3
    );

    std::cout << "Progression: ";

    for (int i = 0; i < 5; ++i)
    {
        std::cout
            << generator.GetNext()
            << ' ';
    }

    std::cout << '\n';

    lab4::Option<int> next =
        generator.TryGetNext();

    if (next.HasValue())
    {
        std::cout
            << "Next value: "
            << next.GetValue()
            << '\n';
    }

    return 0;
}