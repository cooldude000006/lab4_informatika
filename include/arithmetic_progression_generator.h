#pragma once

#include "igenerator.h"

namespace lab4
{
    template<class T>
    class ArithmeticProgressionGenerator :
        public IGenerator<T>
    {
    private:
        T next_value_;
        T step_;

    public:
        ArithmeticProgressionGenerator(
            const T& first,
            const T& step
        )
            : next_value_(first),
              step_(step)
        {
        }

        bool HasNext() const override
        {
            return true;
        }

        T GetNext() override
        {
            T result = next_value_;

            next_value_ = next_value_ + step_;

            return result;
        }

        IGenerator<T>* Clone() const override
        {
            return new ArithmeticProgressionGenerator<T>(*this);
        }
    };
}