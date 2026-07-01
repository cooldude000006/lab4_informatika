#pragma once

#include "../exceptions.h"
#include "sequence.h"
#include "dynamic_array.h"
#include "array_enumerator.h"

namespace lab4
{
    template<class T>
    class MutableArraySequence : public Sequence<T>
    {
    private:
        DynamicArray<T>* items_;
    //конструкторы
    public:
        MutableArraySequence(T* items, int count)
            : items_(new DynamicArray<T>(items, count))
        {
        }

        explicit MutableArraySequence(int size)
            : items_(new DynamicArray<T>(size))
        {
        }

        MutableArraySequence()
            : items_(new DynamicArray<T>(0))
        {
        }

        MutableArraySequence(const MutableArraySequence<T>& other)
            : items_(new DynamicArray<T>(*other.items_))
        {
        }

        MutableArraySequence<T>& operator=(
            const MutableArraySequence<T>& other)
        {
            if (this != &other)
            {
                DynamicArray<T>* newItems =
                    new DynamicArray<T>(*other.items_);
                delete items_;
                items_ = newItems;
            }
            return *this;
        }


        ~MutableArraySequence() override
        {
            delete items_;
            items_ = nullptr;
        }
        //декомпозиция
        T GetFirst() const override
        {
            if (items_->GetSize() == 0)
            {
                throw InvalidOperationException("Невозможно получить первый элемент: последовательность пуста");
            }
            return items_->Get(0);
        }

        T GetLast() const override
        {
            if (items_->GetSize() == 0)
            {
                throw InvalidOperationException("Невозможно получить последний элемент: последовательность пуста");
            }
            return items_->Get(items_->GetSize() - 1);
        }

        T Get(int index) const override
        {
            return items_->Get(index);
        }

        Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override
        {
            if (startIndex < 0 || endIndex >= items_->GetSize() || startIndex > endIndex)
            {
                throw IndexOutOfRangeException(startIndex, items_->GetSize(), "MutableArraySequence::GetSubsequence");
            }

            MutableArraySequence<T>* result = new MutableArraySequence<T>();
            for (int i = startIndex; i <= endIndex; ++i)
            {
                result->Append(items_->Get(i));
            }
            return result;
        }

        int GetLength() const override
        {
            return items_->GetSize();
        }
        //операции(mutable)
        Sequence<T>* Append(const T& item) override
        {
            items_->Resize(items_->GetSize() + 1);
            items_->Set(items_->GetSize() - 1,item);
            return this;
        }

        Sequence<T>* Prepend(const T& item) override
        {
            DynamicArray<T>* newItems = new DynamicArray<T>(items_->GetSize()+1);
            newItems->Set(0, item);
            for (int i=0; i<items_->GetSize(); ++i)
            {
                newItems->Set(i+1, items_->Get(i));
            }
            delete items_;
            items_=newItems;
            return this;
        }

        Sequence<T>* InsertAt(const T& item, int index) override
        {
            if (index < 0  || index >= items_->GetSize())
            {
                throw IndexOutOfRangeException(index,items_->GetSize(), "MutableArraySequence::InsertAt");
            }
            if (index == 0)
            {
                return Prepend(item);
            }

            //общий случай, сдвиг элементов в середину массива
            DynamicArray<T>* newItems = new DynamicArray<T>(items_->GetSize() + 1);
            for (int i=0; i<index; ++i)
            {
                newItems->Set(i, items_->Get(i));
            }
            newItems->Set(index, item);
            for (int i=index;i<items_->GetSize(); ++i)
            {
                newItems->Set(i+1, items_->Get(i));
            }
            delete items_;
            items_=newItems;
            return this;
        }

        Sequence<T>* Concat(Sequence<T>* list) const override
        {
            if (list == nullptr)
            {
                return new MutableArraySequence<T>(*this);
            }
            MutableArraySequence<T>* result = new MutableArraySequence<T>(*this);
            for (int i = 0; i < list->GetLength(); ++i)
            {
                result->Append(list->Get(i));
            }
            return result;
        }

        IEnumerator<T>* GetEnumerator() const override
        {
            return new ArrayEnumerator<T>(this);
        }

        //оператор доступа [] по индексу
        T& operator[](int index)
        {
            return (*items_)[index];
        }

        //создание пустой последовательности того же типа
    protected:
        Sequence<T>* CreateEmpty() const override
        {
            return new MutableArraySequence<T>();
        }
    };
}









