#pragma once

#include "../../exceptions.h"
#include "../igenerator.h"
#include "../../sequence/mutable_array_sequence.h"
#include "../../sequence/sequence.h"

namespace lab4
{
    template<class T>
    class FiniteSequenceGenerator :
        public IGenerator<T>
    {
    private:
        MutableArraySequence<T> items_;
        int next_index_;

    public:
        FiniteSequenceGenerator(T* items, int count)
            : items_(items, count), next_index_(0)
        {
        }

        explicit FiniteSequenceGenerator( const Sequence<T>& sequence)
            : items_(), next_index_(0)
        {
            for (
                int index = 0;
                index < sequence.GetLength();
                ++index
            )
            {
                items_.Append(sequence.Get(index));
            }
        }

        bool HasNext() const override
        {
            return next_index_ < items_.GetLength();
        }

        T GetNext() override
        {
            if (!HasNext())
            {
                throw IndexOutOfRangeException(
                    next_index_,
                    items_.GetLength(),
                    "FiniteSequenceGenerator::GetNext"
                );
            }

            T result = items_.Get(next_index_);

            ++next_index_;

            return result;
        }

        IGenerator<T>* Clone() const override
        {
            return new FiniteSequenceGenerator<T>(*this);
        }
    };
}