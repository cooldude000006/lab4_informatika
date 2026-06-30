#pragma once

#include "../option.h"

namespace lab4
{
    template<class T>
    class IGenerator
    {
    public:
        virtual ~IGenerator() = default;

        // Проверить, можно ли получить следующий элемент.
        virtual bool HasNext() const = 0;

        // Получить следующий элемент.
        virtual T GetNext() = 0;

        // Создать независимую копию генератора.
        virtual IGenerator<T>* Clone() const = 0;

        Option<T> TryGetNext()
        {
            if (!HasNext())
            {
                return Option<T>::None();
            }

            return Option<T>::Some(GetNext());
        }
    };
}