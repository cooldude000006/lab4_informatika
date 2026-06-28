#pragma once

#include <cstddef>

namespace lab4
{
    class Cardinal
    {
    private:
        bool is_infinite_;
        std::size_t value_;

    public:
        // Создать конечный кардинал со значением 0.
        Cardinal();

        // Создать конечный кардинал с заданным значением.
        explicit Cardinal(std::size_t value);

        // Создать кардинал, представляющий бесконечность.
        static Cardinal Infinity();

        // Проверить, представляет ли объект бесконечность.
        bool IsInfinite() const;

        // Получить конечное значение.
        std::size_t GetValue() const;
    };
}