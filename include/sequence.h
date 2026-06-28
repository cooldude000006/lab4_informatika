#pragma  once

#include "exceptions.h"
#include "ienumerable.h"
#include "icollection.h"
namespace lab4 {
    template<class T>
    class Sequence :
        public ICollection<T>,
        public IEnumerable<T>
    {
    public:
        virtual ~Sequence() = default;

        // итератор
        IEnumerator<T>* GetEnumerator() const override = 0;

        //декомпозиция
        virtual T GetFirst() const = 0; //получить первый элемент

        virtual T GetLast() const = 0;//получить последний элемент

        virtual T Get(int index) const = 0;//получить элемент по индексу

        virtual  Sequence<T>*  GetSubsequence(int startIndex, int endIndex) const = 0;//получить подпоследовательность

        virtual int GetLength() const = 0;//получить количество элементов

        std::size_t GetCount() const override
        {
            return static_cast<std::size_t>(GetLength());
        }

        T Get(std::size_t index) const override
        {
            if (index >= GetCount())
            {
                throw IndexOutOfRangeException(
                    index,
                    GetCount(),
                    "ICollection::Get"
                );
            }
            return Get(static_cast<int>(index));
        }

        //операции
        virtual Sequence<T>* Append(const T& item) = 0;//добавить эл в конец

        virtual Sequence<T>* Prepend(const T& item) = 0;//добавить эл в начало

        virtual Sequence<T>* InsertAt(const T& item, int index) = 0;//вставить эл ао индексу

        virtual Sequence<T>* Concat(Sequence<T>* list) const = 0;//сцепить с другой последовательностью

        //map-reduce
        template<typename Func>
        Sequence<T>* Map(Func function) const
        {
            Sequence<T>* result = CreateEmpty();

            try
            {
                for (int i = 0; i < GetLength(); ++i)
                {
                    Sequence<T>* next =
                        result->Append(
                            function(Get(i))
                        );

                    if (next != result)
                    {
                        delete result;
                        result = next;
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }
            return result;
        }

        template<typename Predicate>
        Sequence<T>* Where(Predicate predicate) const
        {
            Sequence<T>* result = CreateEmpty();

            try
            {
                for (int i = 0; i < GetLength(); ++i)
                {
                    T item = Get(i);

                    if (predicate(item))
                    {
                        Sequence<T>* next =
                            result->Append(item);

                        if (next != result)
                        {
                            delete result;
                            result = next;
                        }
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }
            return result;
        }


        template<typename Func, typename TAcc>
        TAcc Reduce(Func f, TAcc initial) const
        {
            TAcc acc = initial;
            for (int i = 0; i < GetLength(); ++i)
            {
                acc = f(Get(i), acc);
            }
            return acc;
        }



        T operator[](int index) const //оператор [] доступа по индексу(перегрузка)
        {
            return Get(index);
        }

    protected:
        virtual Sequence<T>* CreateEmpty() const = 0; //вспомогательный метод создания пустой последовательности того же типа
    };
}






























