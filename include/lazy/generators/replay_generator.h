#pragma once

#include <cstddef>

#include "../../cardinal.h"
#include "../../exceptions.h"
#include "../igenerator.h"
#include "../../sequence/mutable_array_sequence.h"

namespace lab4
{
    template<class T>
    class ReplayGenerator : public IGenerator<T>
    {
    private:
        MutableArraySequence<T> prefix_;
        int prefix_index_;
        IGenerator<T>* continuation_;
        Cardinal length_;
        std::size_t produced_count_;

    public:
        ReplayGenerator(
            const MutableArraySequence<T>& prefix,
            const IGenerator<T>* continuation,
            const Cardinal& length
        )
            : prefix_(prefix),
              prefix_index_(0),
              continuation_(continuation == nullptr ? nullptr : continuation->Clone()),
              length_(length),
              produced_count_(0)
        {
        }

        ReplayGenerator(const ReplayGenerator<T>& other)
            : prefix_(other.prefix_),
              prefix_index_(other.prefix_index_),
              continuation_(
                  other.continuation_ == nullptr
                      ? nullptr
                      : other.continuation_->Clone()
              ),
              length_(other.length_),
              produced_count_(other.produced_count_)
        {
        }

        ReplayGenerator<T>& operator=(const ReplayGenerator<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            IGenerator<T>* new_continuation = nullptr;

            if (other.continuation_ != nullptr)
            {
                new_continuation = other.continuation_->Clone();
            }

            try
            {
                prefix_ = other.prefix_;
            }
            catch (...)
            {
                delete new_continuation;
                throw;
            }

            delete continuation_;

            prefix_index_ = other.prefix_index_;
            continuation_ = new_continuation;
            length_ = other.length_;
            produced_count_ = other.produced_count_;

            return *this;
        }

        ~ReplayGenerator() override
        {
            delete continuation_;
            continuation_ = nullptr;
        }

        bool HasNext() const override
        {
            if (!length_.IsInfinite() &&
                produced_count_ >= length_.GetValue())
            {
                return false;
            }

            if (prefix_index_ < prefix_.GetLength())
            {
                return true;
            }

            return continuation_ != nullptr && continuation_->HasNext();
        }

        T GetNext() override
        {
            if (!HasNext())
            {
                std::size_t size = produced_count_;

                if (!length_.IsInfinite())
                {
                    size = length_.GetValue();
                }

                throw IndexOutOfRangeException(
                    produced_count_,
                    size,
                    "ReplayGenerator::GetNext"
                );
            }

            T result;

            if (prefix_index_ < prefix_.GetLength())
            {
                result = prefix_.Get(prefix_index_);
                ++prefix_index_;
            }
            else
            {
                result = continuation_->GetNext();
            }

            ++produced_count_;

            return result;
        }

        IGenerator<T>* Clone() const override
        {
            return new ReplayGenerator<T>(*this);
        }
    };
}