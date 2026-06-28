#pragma once

#include <cstddef>
#include <limits>

#include "cardinal.h"
#include "exceptions.h"
#include "ienumerator.h"

namespace lab4
{
    template<class T>
    class LazySequence;

    template<class T>
    class LazySequenceEnumerator : public IEnumerator<T>
    {
    private:
        const LazySequence<T>* sequence_;
        int index_;

    public:
        explicit LazySequenceEnumerator(const LazySequence<T>* sequence)
            : sequence_(sequence),
              index_(-1)
        {
            if (sequence_ == nullptr)
            {
                throw InvalidOperationException(
                    "Невозможно создать итератор для nullptr");
            }
        }

        bool MoveNext() override
        {
            if (index_ == std::numeric_limits<int>::max())
            {
                throw InvalidOperationException(
                    "Индекс итератора превысил поддерживаемый диапазон int");
            }

            Cardinal length = sequence_->GetLength();

            if (!length.IsInfinite())
            {
                std::size_t next_index = static_cast<std::size_t>(index_ + 1);

                if (next_index >= length.GetValue())
                {
                    if (
                        length.GetValue() <=
                        static_cast<std::size_t>(std::numeric_limits<int>::max())
                    )
                    {
                        index_ = static_cast<int>(length.GetValue());
                    }
                    else
                    {
                        index_ = std::numeric_limits<int>::max();
                    }
                    return false;
                }
            }
            ++index_;
            return true;
        }

        Option<T> GetCurrent() const override
        {
            if (index_ < 0)
            {
                return Option<T>::None();
            }

            Cardinal length = sequence_->GetLength();

            if (
                !length.IsInfinite() && static_cast<std::size_t>(index_) >= length.GetValue()
            )
            {
                return Option<T>::None();
            }

            return Option<T>::Some(sequence_->Get(index_));
        }

        void Reset() override
        {
            index_ = -1;
        }
    };
}