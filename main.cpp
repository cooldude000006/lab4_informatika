#include <iostream>

#include "lazy_sequence.h"

void PrintFiniteSequence(
    const char* name,
    lab4::LazySequence<int>* sequence
)
{
    std::cout << name << ": ";

    std::size_t length =
        sequence->GetLength().GetValue();

    for (
        std::size_t index = 0;
        index < length;
        ++index
    )
    {
        std::cout
            << sequence->Get(
                   static_cast<int>(index)
               )
            << ' ';
    }

    std::cout << '\n';
}

int main()
{
    int first_items[] = {10, 20, 30};
    int second_items[] = {40, 50};

    lab4::LazySequence<int> first(first_items, 3);
    lab4::LazySequence<int> second(second_items, 2);

    lab4::LazySequence<int>* result =
        first.Concat(&second);

    std::cout
        << "First materialized: "
        << first.GetMaterializedCount()
        << '\n';

    std::cout
        << "Second materialized: "
        << second.GetMaterializedCount()
        << '\n';

    std::cout
        << "Result materialized: "
        << result->GetMaterializedCount()
        << '\n';

    PrintFiniteSequence("Result", result);

    delete result;

    return 0;
}