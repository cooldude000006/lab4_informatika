#pragma once

#include <cstddef>

#include "../../exceptions.h"
#include "../igenerator.h"

namespace lab4
{
    template<class T>
    class ConcatGenerator : public IGenerator<T>
    {
    private:
        IGenerator<T>* first_;
        IGenerator<T>* second_;
        std::size_t produced_count_;

    public:
        ConcatGenerator(
            const IGenerator<T>& first,
            const IGenerator<T>& second
        )
            : first_(nullptr),
              second_(nullptr),
              produced_count_(0)
        {
            first_ = first.Clone();

            try
            {
                second_ = second.Clone();
            }
            catch (...)
            {
                delete first_;
                first_ = nullptr;
                throw;
            }
        }

        ConcatGenerator(const ConcatGenerator<T>& other)
            : first_(nullptr),
              second_(nullptr),
              produced_count_(other.produced_count_)
        {
            if (other.first_ != nullptr)
            {
                first_ = other.first_->Clone();
            }

            try
            {
                if (other.second_ != nullptr)
                {
                    second_ = other.second_->Clone();
                }
            }
            catch (...)
            {
                delete first_;
                first_ = nullptr;
                throw;
            }
        }

        ConcatGenerator<T>& operator=(const ConcatGenerator<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            IGenerator<T>* new_first = nullptr;
            IGenerator<T>* new_second = nullptr;

            if (other.first_ != nullptr)
            {
                new_first = other.first_->Clone();
            }

            try
            {
                if (other.second_ != nullptr)
                {
                    new_second = other.second_->Clone();
                }
            }
            catch (...)
            {
                delete new_first;
                throw;
            }

            delete first_;
            delete second_;

            first_ = new_first;
            second_ = new_second;
            produced_count_ = other.produced_count_;

            return *this;
        }

        ~ConcatGenerator() override
        {
            delete first_;
            delete second_;

            first_ = nullptr;
            second_ = nullptr;
        }

        bool HasNext() const override
        {
            if (first_ != nullptr && first_->HasNext())
            {
                return true;
            }

            return second_ != nullptr && second_->HasNext();
        }

        T GetNext() override
        {
            if (first_ != nullptr && first_->HasNext())
            {
                T result = first_->GetNext();
                ++produced_count_;
                return result;
            }

            if (second_ != nullptr && second_->HasNext())
            {
                T result = second_->GetNext();
                ++produced_count_;
                return result;
            }

            throw IndexOutOfRangeException(
                produced_count_,
                produced_count_,
                "ConcatGenerator::GetNext"
            );
        }

        IGenerator<T>* Clone() const override
        {
            return new ConcatGenerator<T>(*this);
        }
    };
}