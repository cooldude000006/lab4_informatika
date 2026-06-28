#pragma once

#include <cstddef>
#include <limits>

#include "cardinal.h"
#include "exceptions.h"
#include "finite_sequence_generator.h"
#include "igenerator.h"
#include "mutable_array_sequence.h"
#include "sequence.h"

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
                static_cast<std::size_t>(index) >=
                    length_.GetValue()
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
                if (generator_ == nullptr || !generator_->HasNext()
                )
                {
                    throw InvalidOperationException(
                        "Генератор завершился раньше объявленной длины последовательности");
                }

                materialized_items_.Append(generator_->GetNext());
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

        // Создать последовательность по генератору.
        LazySequence(const IGenerator<T>& generator,const Cardinal& length)
            : materialized_items_(), generator_(generator.Clone()), length_(length)
        {
        }

        // Скопировать элементы переданного массива.
        LazySequence(T* items, int count)
            : materialized_items_(), generator_(nullptr), length_(0)
        {
            if (count < 0)
            {
                throw InvalidOperationException(
                    "Количество элементов не может быть отрицательным");
            }

            if (items == nullptr && count > 0)
            {
                throw InvalidOperationException(
                    "Указатель items равен nullptr при положительном количестве элементов");
            }

            generator_ = new FiniteSequenceGenerator<T>(items,count);

            length_ = Cardinal(static_cast<std::size_t>(count));
        }

        // Скопировать элементы обычной последовательности.
        explicit LazySequence(const Sequence<T>* sequence)
            : materialized_items_(), generator_(nullptr), length_(0)
        {
            if (sequence == nullptr)
            {
                throw InvalidOperationException(
                    "Невозможно создать LazySequence из nullptr");
            }

            int sequence_length = sequence->GetLength();

            if (sequence_length < 0)
            {
                throw InvalidOperationException(
                    "Длина исходной последовательности не может быть отрицательной");
            }

            generator_ = new FiniteSequenceGenerator<T>(*sequence);

            length_ = Cardinal(static_cast<std::size_t>(sequence_length));
        }

        LazySequence(const LazySequence<T>& other)
            : materialized_items_(other.materialized_items_),
              generator_(other.generator_ == nullptr ? nullptr : other.generator_->Clone()),
              length_(other.length_)
        {
        }

        LazySequence<T>& operator=(const LazySequence<T>& other)
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
                materialized_items_ = other.materialized_items_;
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

        T GetFirst()
        {
            if (!length_.IsInfinite() && length_.GetValue() == 0)
            {
                throw IndexOutOfRangeException(0, 0, "LazySequence::GetFirst");
            }

            return Get(0);
        }

        T GetLast()
        {
            if (length_.IsInfinite())
            {
                throw InvalidOperationException(
                    "У бесконечной последовательности нет последнего элемента");
            }

            std::size_t length = length_.GetValue();

            if (length == 0)
            {
                throw IndexOutOfRangeException(0, 0, "LazySequence::GetLast");
            }

            std::size_t last_index = length - 1;

            if (
                last_index >
                static_cast<std::size_t>(std::numeric_limits<int>::max())
            )
            {
                throw InvalidOperationException(
                    "Индекс последнего элемента превышает поддерживаемый диапазон int");
            }

            return Get(static_cast<int>(last_index));
        }

        T Get(int index)
        {
            ValidateIndex(index);
            MaterializeUntil(index);

            return materialized_items_.Get(index);
        }

        LazySequence<T>* GetSubsequence(int start_index, int end_index)
        {
            ValidateIndex(start_index);
            ValidateIndex(end_index);

            if (start_index > end_index)
            {
                throw InvalidOperationException(
                    "Начальный индекс подпоследовательности не может быть больше конечного индекса");
            }

            MutableArraySequence<T> extracted_items;

            for (int index = start_index; ; ++index)
            {
                extracted_items.Append(Get(index));

                if (index == end_index)
                {
                    break;
                }
            }

            return new LazySequence<T>(&extracted_items);
        }

        Cardinal GetLength() const
        {
            return length_;
        }

        std::size_t GetMaterializedCount() const
        {
            return static_cast<std::size_t>(materialized_items_.GetLength());
        }
    };
}