#pragma once

#include "../../exceptions.h"
#include "../igenerator.h"

namespace lab4
{
    template<class TSource, class TResult, class TMapper>
    class MapGenerator : public IGenerator<TResult>
    {
    private:
        IGenerator<TSource>* source_;
        TMapper mapper_;

    public:
        MapGenerator(
            const IGenerator<TSource>& source,
            const TMapper& mapper
        )
            : source_(source.Clone()),
              mapper_(mapper)
        {
        }

        MapGenerator(
            const MapGenerator<TSource, TResult, TMapper>& other
        )
            : source_(other.source_ == nullptr ? nullptr : other.source_->Clone()),
              mapper_(other.mapper_)
        {
        }

        MapGenerator<TSource, TResult, TMapper>& operator=(
            const MapGenerator<TSource, TResult, TMapper>& other
        )
        {
            if (this == &other)
            {
                return *this;
            }

            IGenerator<TSource>* new_source = nullptr;

            if (other.source_ != nullptr)
            {
                new_source = other.source_->Clone();
            }

            try
            {
                mapper_ = other.mapper_;
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

        ~MapGenerator() override
        {
            delete source_;
            source_ = nullptr;
        }

        bool HasNext() const override
        {
            return source_ != nullptr && source_->HasNext();
        }

        TResult GetNext() override
        {
            if (!HasNext())
            {
                throw InvalidOperationException("В MapGenerator отсутствует следующий элемент");
            }
            return mapper_(source_->GetNext());
        }

        IGenerator<TResult>* Clone() const override
        {
            return new MapGenerator<TSource, TResult, TMapper>(*this);
        }
    };
}