#include <iostream>

#include "lazy_sequence.h"

int main()
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    lab4::IEnumerator<int>* enumerator =
        sequence.GetEnumerator();

    lab4::Option<int> before =
        enumerator->GetCurrent();

    std::cout
        << "Current exists before MoveNext: "
        << before.HasValue()
        << '\n';

    std::cout
        << "Materialized before iteration: "
        << sequence.GetMaterializedCount()
        << '\n';

    while (enumerator->MoveNext())
    {
        std::cout
            << "After MoveNext: "
            << sequence.GetMaterializedCount()
            << '\n';

        lab4::Option<int> current =
            enumerator->GetCurrent();

        if (current.HasValue())
        {
            std::cout
                << "Current: "
                << current.GetValue()
                << '\n';
        }

        std::cout
            << "After GetCurrent: "
            << sequence.GetMaterializedCount()
            << '\n';
    }

    lab4::Option<int> after =
        enumerator->GetCurrent();

    std::cout
        << "Current exists after end: "
        << after.HasValue()
        << '\n';

    delete enumerator;

    return 0;
}