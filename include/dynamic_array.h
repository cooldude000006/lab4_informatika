#pragma once

#include <algorithm>
#include <limits>

#include "exceptions.h"

namespace lab4
{
    template<class T>
    class DynamicArray
    {
    private:
        T* data_;
        int size_;
        int capacity_;

        void ValidateIndex(int index) const
        {
            if (index < 0 || index >= size_)
            {
                throw IndexOutOfRangeException(
                    index,
                    size_,
                    "DynamicArray::ValidateIndex"
                );
            }
        }

        int CalculateCapacity(int required_size) const
        {
            int new_capacity = capacity_ == 0 ? 1 : capacity_;

            while (new_capacity < required_size)
            {
                if (
                    new_capacity >
                    std::numeric_limits<int>::max() / 2
                )
                {
                    return required_size;
                }

                new_capacity *= 2;
            }

            return new_capacity;
        }

    public:
        DynamicArray(T* items, int count)
            : data_(nullptr),
              size_(0),
              capacity_(0)
        {
            if (count < 0)
            {
                throw InvalidOperationException("Размер массива не может быть отрицательным");
            }

            if (items == nullptr && count > 0)
            {
                throw InvalidOperationException(
                    "Указатель items равен nullptr при положительном количестве элементов");
            }

            if (count == 0)
            {
                return;
            }

            T* new_data = new T[count];

            try
            {
                std::copy(items, items + count, new_data);
            }
            catch (...)
            {
                delete[] new_data;
                throw;
            }

            data_ = new_data;
            size_ = count;
            capacity_ = count;
        }

        explicit DynamicArray(int size)
            : data_(nullptr),
              size_(0),
              capacity_(0)
        {
            if (size < 0)
            {
                throw InvalidOperationException("Размер массива не может быть отрицательным");
            }

            if (size == 0)
            {
                return;
            }

            data_ = new T[size]();
            size_ = size;
            capacity_ = size;
        }

        DynamicArray(const DynamicArray<T>& other)
            : data_(nullptr),
              size_(0),
              capacity_(0)
        {
            if (other.size_ == 0)
            {
                return;
            }

            T* new_data = new T[other.size_];

            try
            {
                std::copy(
                    other.data_,
                    other.data_ + other.size_,
                    new_data
                );
            }
            catch (...)
            {
                delete[] new_data;
                throw;
            }

            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.size_;
        }

        DynamicArray<T>& operator=(const DynamicArray<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            T* new_data = nullptr;

            if (other.size_ > 0)
            {
                new_data = new T[other.size_];

                try
                {
                    std::copy(
                        other.data_,
                        other.data_ + other.size_,
                        new_data
                    );
                }
                catch (...)
                {
                    delete[] new_data;
                    throw;
                }
            }

            delete[] data_;

            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.size_;

            return *this;
        }

        ~DynamicArray()
        {
            delete[] data_;
            data_ = nullptr;
        }

        T& operator[](int index)
        {
            ValidateIndex(index);
            return data_[index];
        }

        const T& operator[](int index) const
        {
            ValidateIndex(index);
            return data_[index];
        }

        T Get(int index) const
        {
            ValidateIndex(index);
            return data_[index];
        }

        void Set(int index, const T& value)
        {
            ValidateIndex(index);
            data_[index] = value;
        }

        int GetSize() const
        {
            return size_;
        }

        void Resize(int new_size)
        {
            if (new_size < 0)
            {
                throw InvalidOperationException("Размер массива не может быть отрицательным");
            }

            if (new_size == size_)
            {
                return;
            }

            if (new_size <= capacity_)
            {
                if (new_size > size_)
                {
                    for (
                        int index = size_;
                        index < new_size;
                        ++index
                    )
                    {
                        data_[index] = T();
                    }
                }

                size_ = new_size;
                return;
            }

            int new_capacity = CalculateCapacity(new_size);
            T* new_data = new T[new_capacity]();

            try
            {
                if (size_ > 0)
                {
                    std::copy(
                        data_,
                        data_ + size_,
                        new_data
                    );
                }
            }
            catch (...)
            {
                delete[] new_data;
                throw;
            }

            delete[] data_;

            data_ = new_data;
            size_ = new_size;
            capacity_ = new_capacity;
        }
    };
}