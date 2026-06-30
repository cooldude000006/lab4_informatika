#pragma once

#include <cstddef>

#include "../../exceptions.h"
#include "../igenerator.h"

namespace lab4
{
    template<class T>
    class InsertionGenerator : public IGenerator<T>
    {
    private:
        IGenerator<T>* source_;
        T item_;
        std::size_t insertion_index_;
        std::size_t output_index_;
        bool inserted_;

    public:
        InsertionGenerator(
            const IGenerator<T>& source,
            const T& item,
            std::size_t insertion_index
        )
            : source_(source.Clone()),
              item_(item),
              insertion_index_(insertion_index),
              output_index_(0),
              inserted_(false)
        {
        }

        InsertionGenerator(const InsertionGenerator<T>& other)
            : source_(other.source_ == nullptr ? nullptr : other.source_->Clone()),
              item_(other.item_),
              insertion_index_(other.insertion_index_),
              output_index_(other.output_index_),
              inserted_(other.inserted_)
        {
        }

        InsertionGenerator<T>& operator=(
            const InsertionGenerator<T>& other
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

            try
            {
                item_ = other.item_;
            }
            catch (...)
            {
                delete new_source;
                throw;
            }

            delete source_;

            source_ = new_source;
            insertion_index_ = other.insertion_index_;
            output_index_ = other.output_index_;
            inserted_ = other.inserted_;

            return *this;
        }

        ~InsertionGenerator() override
        {
            delete source_;
            source_ = nullptr;
        }

        bool HasNext() const override
        {
            if (!inserted_ && output_index_ == insertion_index_)
            {
                return true;
            }

            return source_ != nullptr && source_->HasNext();
        }

        T GetNext() override
        {
            if (!inserted_ && output_index_ == insertion_index_)
            {
                inserted_ = true;
                ++output_index_;

                return item_;
            }

            if (source_ == nullptr || !source_->HasNext())
            {
                throw IndexOutOfRangeException(
                    output_index_,
                    output_index_,
                    "InsertionGenerator::GetNext"
                );
            }

            T result = source_->GetNext();

            ++output_index_;

            return result;
        }

        IGenerator<T>* Clone() const override
        {
            return new InsertionGenerator<T>(*this);
        }
    };
}