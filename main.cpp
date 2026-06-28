#include <iostream>

#include "cardinal.h"
#include "mutable_array_sequence.h"

int main()
{
    lab4::MutableArraySequence<int> sequence;

    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);

    lab4::Cardinal finiteLength(
        static_cast<std::size_t>(sequence.GetLength())
    );

    lab4::Cardinal infiniteLength =
        lab4::Cardinal::Infinity();

    std::cout
        << "Sequence length: "
        << finiteLength.GetValue()
        << '\n';

    std::cout
        << "Finite cardinal is infinite: "
        << finiteLength.IsInfinite()
        << '\n';

    std::cout
        << "Infinite cardinal is infinite: "
        << infiniteLength.IsInfinite()
        << '\n';
    try
    {
        infiniteLength.GetValue();
    }
    catch (const lab4::InvalidOperationException& exception)
    {
        std::cout
            << exception.what()
            << '\n';
    }
    return 0;
}