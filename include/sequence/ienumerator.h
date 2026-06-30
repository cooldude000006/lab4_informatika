#pragma once
#include "../option.h"

namespace lab4
{
    template <typename T>
    class IEnumerator
    {
    public:
        virtual ~IEnumerator() = default;

        // Переместить курсор к следующему элементу.
        virtual bool MoveNext() = 0;

        // Получить текущий элемент.
        virtual Option<T> GetCurrent() const = 0;

        // Сбросить курсор в позицию перед первым элементом.
        virtual void Reset() = 0;
    };
}