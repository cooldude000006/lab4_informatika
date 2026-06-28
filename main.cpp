#include <iostream>

#include "mutable_array_sequence.h"

int main()
{
    lab4::MutableArraySequence<int> sequence;

    sequence.Append(10);
    sequence.Append(20);
    sequence.Append(30);

    std::cout
        << "Length: "
        << sequence.GetLength()
        << '\n';

    std::cout
        << "Elements: "
        << sequence.Get(0)
        << ' '
        << sequence.Get(1)
        << ' '
        << sequence.Get(2)
        << '\n';

    return 0;
}