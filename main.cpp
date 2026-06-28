#include <iostream>

#include "arithmetic_progression_generator.h"
#include "lazy_sequence.h"

int main()
{
    lab4::ArithmeticProgressionGenerator<int> generator(
        2,
        3
    );

    lab4::LazySequence<int> sequence(
        generator,
        lab4::Cardinal::Infinity()
    );

    std::cout
        << "Materialized before Get: "
        << sequence.GetMaterializedCount()
        << '\n';

    std::cout
        << "Element 4: "
        << sequence.Get(4)
        << '\n';

    std::cout
        << "Materialized after Get(4): "
        << sequence.GetMaterializedCount()
        << '\n';

    std::cout
        << "Element 2: "
        << sequence.Get(2)
        << '\n';

    std::cout
        << "Materialized after repeated access: "
        << sequence.GetMaterializedCount()
        << '\n';

    return 0;
}