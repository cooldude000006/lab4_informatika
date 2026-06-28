#pragma once

#include "exceptions.h"
#include "igenerator.h"

namespace lab4
{
    template<class T, class TPredicate>
    class WhereGenerator : public IGenerator<T>
    {
    private:
        IGenerator<T>* source_;
        TPredicate predicate_;

    public:
        WhereGenerator(const IGenerator<T>& source, const TPredicate& predicate)
            : source_(source.Clone()),
              predicate_(predicate)
        {
        }

        WhereGenerator(const WhereGenerator<T, TPredicate>& other)
            : source_(
                  other.source_ == nullptr
                      ? nullptr
                      : other.source_->Clone()
              ),
              predicate_(other.predicate_)
        {
        }

        WhereGenerator<T, TPredicate>& operator=(const WhereGenerator<T, TPredicate>& other)
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

            try
            {
                predicate_ = other.predicate_;
            }
            catch (...)
            {
                delete new_source;
                throw;
            }

            delete source_;

            source_ = new_source;

            return *this;
        }

        ~WhereGenerator() override
        {
            delete source_;
            source_ = nullptr;
        }

        bool HasNext() const override
        {
            return source_ != nullptr && source_->HasNext();
        }

        T GetNext() override
        {
            while (source_ != nullptr && source_->HasNext())
            {
                T item = source_->GetNext();

                if (predicate_(item))
                {
                    return item;
                }
            }

            throw InvalidOperationException(
                "В WhereGenerator отсутствует следующий подходящий элемент");
        }

        IGenerator<T>* Clone() const override
        {
            return new WhereGenerator<T, TPredicate>(*this);
        }
    };
}