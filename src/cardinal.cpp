#include "cardinal.h"

#include "exceptions.h"

namespace lab4
{
    Cardinal::Cardinal()
        : is_infinite_(false),
          value_(0)
    {
    }

    Cardinal::Cardinal(std::size_t value)
        : is_infinite_(false),
          value_(value)
    {
    }

    Cardinal Cardinal::Infinity()
    {
        Cardinal result;

        result.is_infinite_ = true;

        return result;
    }

    bool Cardinal::IsInfinite() const
    {
        return is_infinite_;
    }

    std::size_t Cardinal::GetValue() const
    {
        if (is_infinite_)
        {
            throw InvalidOperationException(
                "Невозможно получить конечное значение бесконечного Cardinal"
            );
        }

        return value_;
    }
}