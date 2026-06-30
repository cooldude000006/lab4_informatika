#pragma once
#include "ienumerator.h"
#include "sequence.h"

namespace lab4
{
    // Итератор для ArraySequence
    template <typename T>
    class ArrayEnumerator : public IEnumerator<T>
    {
    private:
        const Sequence<T>* sequence_;
        int index_;

    public:
        explicit ArrayEnumerator(const Sequence<T>* sequence)
            : sequence_(sequence), index_(-1){}
        bool MoveNext() override
        {
            if (index_ < sequence_->GetLength() - 1)
            {
                ++index_;
                return true;
            }
            index_ = sequence_->GetLength();
            return false;
        }
        Option<T> GetCurrent() const override
        {
            if (index_ < 0 || index_ >= sequence_->GetLength())
            {
                return Option<T>::None();
            }
            return Option<T>::Some(sequence_->Get(index_));
        }
        void Reset() override
        {
            index_=-1;
        }
    };
}