#pragma once
#include <algorithm>
#include "exceptions.h"

namespace lab4
{
    template<class T>
    class DynamicArray
    {
    private:
        T* data_;
        int size_;

        void validate_index(int index) const
        {
            if (index < 0 || index >= size_)
            {
                throw IndexOutOfRangeException(index, size_, "DynamicArray::validate_index");
            }
        }

    public:
        // ================= КОНСТРУКТОРЫ =================

        DynamicArray(T* items, int count) : size_(count)
        {
            if (count < 0)
            {
                throw InvalidOperationException("Размер массива не может быть отрицательным");
            }
            if (items == nullptr && count > 0)
            {
                throw InvalidOperationException(
                    "Указатель items равен nullptr при положительном количистве элементов");
            }
            if (count == 0)
            {
                data_ = nullptr;
                return;
            }
            data_ = new T[count];
            try
            {
                std::copy(items, items + count, data_);
            }
            catch (...)
            {
                delete[] data_;
                data_ = nullptr;
                throw;
            }
        }

        explicit DynamicArray(int size) : size_(size)
        {
            if (size < 0)
            {
                throw InvalidOperationException("Размер массива не может быть отрицательным");
            }
            if (size == 0)
            {
                data_ = nullptr;
                return;
            }
            data_ = new T[size]();
        }

        DynamicArray(const DynamicArray<T>& other) : size_(other.size_)
        {
            if (size_ == 0)
            {
                data_ = nullptr;
                return;
            }
            data_ = new T[size_];
            try
            {
                std::copy(other.data_, other.data_ + size_, data_);
            }
            catch (...)
            {
                delete[] data_;
                data_ = nullptr;
                throw;
            }
        }

        ~DynamicArray()
        {
            delete[] data_;
            data_ = nullptr;
        }

        // ================= ОПЕРАТОРЫ =================

        DynamicArray<T>& operator=(const DynamicArray<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            T* newData = nullptr;

            if (other.size_ > 0)
            {
                newData = new T[other.size_];

                try
                {
                    std::copy(
                        other.data_,
                        other.data_ + other.size_,
                        newData
                    );
                }
                catch (...)
                {
                    delete[] newData;
                    throw;
                }
            }

            // Старая память удаляется только после того, как новая создана
            delete[] data_;

            data_ = newData;
            size_ = other.size_;

            return *this;
        }

        T& operator[](int index)
        {
            validate_index(index);
            return data_[index];
        }

        const T& operator[](int index) const
        {
            validate_index(index);
            return data_[index];
        }

        // ================= МЕТОДЫ ДОСТУПА =================

        T Get(int index) const
        {
            validate_index(index);
            return data_[index];
        }

        void Set(int index, const T& value)
        {
            validate_index(index);
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
                throw InvalidOperationException(
                    "Размер массива не может быть отрицательным"
                );
            }

            if (new_size == size_)
            {
                return;
            }

            T* new_data = nullptr;

            if (new_size > 0)
            {
                new_data = new T[new_size]();
            }

            try
            {
                if (data_ != nullptr && new_data != nullptr)
                {
                    int copy_count = std::min(size_, new_size);

                    std::copy(
                        data_,
                        data_ + copy_count,
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
        }
    };
}