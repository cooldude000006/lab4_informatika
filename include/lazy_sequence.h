#pragma once

#include <cstddef>

#include "cardinal.h"
#include "exceptions.h"
#include "igenerator.h"
#include "mutable_array_sequence.h"

namespace lab4
{
    template<class T>
    class LazySequence
    {
    private:
        MutableArraySequence<T> materialized_items_;
        IGenerator<T>* generator_;
        Cardinal length_;

        void ValidateIndex(int index) const
        {
            if (index < 0)
            {
                throw IndexOutOfRangeException(
                    index,
                    0,
                    "LazySequence::Get"
                );
            }

            if (
                !length_.IsInfinite() &&
                static_cast<std::size_t>(index) >= length_.GetValue()
            )
            {
                throw IndexOutOfRangeException(
                    static_cast<std::size_t>(index),
                    length_.GetValue(),
                    "LazySequence::Get"
                );
            }
        }

        void MaterializeUntil(int index)
        {
            while (materialized_items_.GetLength() <= index)
            {
                if (
                    generator_ == nullptr ||
                    !generator_->HasNext()
                )
                {
                    throw InvalidOperationException(
                        "Генератор завершился раньше объявленной длины последовательности"
                    );
                }

                materialized_items_.Append(
                    generator_->GetNext()
                );
            }
        }

    public:
        // Создать пустую конечную последовательность.
        LazySequence()
            : materialized_items_(),
              generator_(nullptr),
              length_(0)
        {
        }

        // Создать последовательность с заданным генератором и длиной.
        LazySequence(
            const IGenerator<T>& generator,
            const Cardinal& length
        )
            : materialized_items_(),
              generator_(generator.Clone()),
              length_(length)
        {
        }

        // Глубокое копирование последовательности.
        LazySequence(const LazySequence<T>& other)
            : materialized_items_(other.materialized_items_),
              generator_(
                  other.generator_ == nullptr
                      ? nullptr
                      : other.generator_->Clone()
              ),
              length_(other.length_)
        {
        }

        LazySequence<T>& operator=(
            const LazySequence<T>& other
        )
        {
            if (this == &other)
            {
                return *this;
            }

            IGenerator<T>* new_generator = nullptr;

            if (other.generator_ != nullptr)
            {
                new_generator = other.generator_->Clone();
            }

            try
            {
                materialized_items_ =
                    other.materialized_items_;
            }
            catch (...)
            {
                delete new_generator;
                throw;
            }

            delete generator_;

            generator_ = new_generator;
            length_ = other.length_;

            return *this;
        }

        ~LazySequence()
        {
            delete generator_;
            generator_ = nullptr;
        }

        T Get(int index)
        {
            ValidateIndex(index);
            MaterializeUntil(index);

            return materialized_items_.Get(index);
        }

        Cardinal GetLength() const
        {
            return length_;
        }

        std::size_t GetMaterializedCount() const
        {
            return static_cast<std::size_t>(
                materialized_items_.GetLength()
            );
        }
    };
}