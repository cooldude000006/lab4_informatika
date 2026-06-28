#include <iostream>

#include "finite_sequence_generator.h"

int main()
{
    int items[] = {10, 20, 30};

    lab4::FiniteSequenceGenerator<int> generator(
        items,
        3
    );

    while (generator.HasNext())
    {
        std::cout
            << generator.GetNext()
            << ' ';
    }

    std::cout << '\n';

    try
    {
        generator.GetNext();
    }
    catch (
        const lab4::IndexOutOfRangeException&
            exception
    )
    {
        std::cout
            << exception.what()
            << '\n';
    }

    return 0;
}