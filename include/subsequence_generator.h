#pragma once

#include <cstddef>

#include "exceptions.h"
#include "igenerator.h"

namespace lab4
{
    template<class T>
    class SubsequenceGenerator : public IGenerator<T>
    {
    private:
        IGenerator<T>* source_;
        std::size_t skip_count_;
        std::size_t remaining_count_;
        std::size_t total_count_;

        void SkipPrefix()
        {
            while (skip_count_ > 0)
            {
                if (
                    source_ == nullptr ||
                    !source_->HasNext()
                )
                {
                    throw InvalidOperationException(
                        "Исходный генератор завершился до начала подпоследовательности");
                }

                source_->GetNext();
                --skip_count_;
            }
        }

    public:
        SubsequenceGenerator(
            const IGenerator<T>& source,
            std::size_t start_index,
            std::size_t count
        )
            : source_(source.Clone()),
              skip_count_(start_index),
              remaining_count_(count),
              total_count_(count)
        {
        }

        SubsequenceGenerator(
            const SubsequenceGenerator<T>& other
        )
            : source_(
                  other.source_ == nullptr
                      ? nullptr
                      : other.source_->Clone()
              ),
              skip_count_(other.skip_count_),
              remaining_count_(other.remaining_count_),
              total_count_(other.total_count_)
        {
        }

        SubsequenceGenerator<T>& operator=(
            const SubsequenceGenerator<T>& other
        )
        {
            if (this == &other)
            {
                return *this;
            }

            IGenerator<T>* new_source = nullptr;

            if (other.source_ != nullptr)
            {
                new_source = other.source_->Clone();
            }

            delete source_;

            source_ = new_source;
            skip_count_ = other.skip_count_;
            remaining_count_ = other.remaining_count_;
            total_count_ = other.total_count_;

            return *this;
        }

        ~SubsequenceGenerator() override
        {
            delete source_;
            source_ = nullptr;
        }

        bool HasNext() const override
        {
            return remaining_count_ > 0;
        }

        T GetNext() override
        {
            if (remaining_count_ == 0)
            {
                std::size_t produced_count =
                    total_count_ - remaining_count_;

                throw IndexOutOfRangeException(
                    produced_count,
                    total_count_,
                    "SubsequenceGenerator::GetNext"
                );
            }

            SkipPrefix();

            if (
                source_ == nullptr ||
                !source_->HasNext()
            )
            {
                throw InvalidOperationException(
                    "Исходный генератор завершился раньше "
                    "конца подпоследовательности"
                );
            }

            T result = source_->GetNext();

            --remaining_count_;

            return result;
        }

        IGenerator<T>* Clone() const override
        {
            return new SubsequenceGenerator<T>(*this);
        }
    };
}