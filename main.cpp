#include <iostream>

#include "lazy_sequence.h"

int main()
{
    int items[] =
    {
        10,
        20,
        30,
        40,
        50
    };

    lab4::LazySequence<int> sequence(
        items,
        5
    );

    std::cout
        << "Source materialized before: "
        << sequence.GetMaterializedCount()
        << '\n';

    lab4::LazySequence<int>* subsequence =
        sequence.GetSubsequence(
            1,
            3
        );

    std::cout
        << "Source materialized after: "
        << sequence.GetMaterializedCount()
        << '\n';

    std::cout
        << "Subsequence length: "
        << subsequence
            ->GetLength()
            .GetValue()
        << '\n';

    std::cout
        << "Subsequence materialized before access: "
        << subsequence->GetMaterializedCount()
        << '\n';

    std::cout
        << "Subsequence elements: "
        << subsequence->Get(0)
        << ' '
        << subsequence->Get(1)
        << ' '
        << subsequence->Get(2)
        << '\n';

    std::cout
        << "Subsequence materialized after access: "
        << subsequence->GetMaterializedCount()
        << '\n';

    delete subsequence;

    return 0;
}