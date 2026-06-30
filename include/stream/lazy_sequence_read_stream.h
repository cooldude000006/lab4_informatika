#pragma once

#include <cstddef>
#include <limits>

#include "../lazy/lazy_sequence.h"
#include "read_only_stream.h"

namespace lab4
{
    template<class T>class LazySequenceReadStream : public ReadOnlyStream<T>
    {
    private:
        const LazySequence<T>* sequence_;

        void ValidateSupportedPosition(std::size_t position) const
        {
            if (
                position >
                static_cast<std::size_t>(std::numeric_limits<int>::max())
            )
            {
                throw InvalidOperationException(
                    "Позиция превышает поддерживаемый диапазон индексов LazySequence");
            }
        }

    public:
        explicit LazySequenceReadStream(const LazySequence<T>* sequence)
            : sequence_(sequence)
        {
            if (sequence_ == nullptr)
            {
                throw InvalidOperationException(
                    "Невозможно создать поток чтения из nullptr");
            }
        }

        bool IsEndOfStream() const override
        {
            this->EnsureOpen(
                "LazySequenceReadStream::IsEndOfStream"
            );

            Cardinal length = sequence_->GetLength();

            if (length.IsInfinite())
            {
                return false;
            }

            return this->GetPosition() >= length.GetValue();
        }

        T Read() override
        {
            this->EnsureOpen("LazySequenceReadStream::Read");

            if (IsEndOfStream())
            {
                throw EndOfStreamException(
                    "LazySequenceReadStream::Read");
            }

            std::size_t position = this->GetPosition();

            ValidateSupportedPosition(position);

            T result = sequence_->Get(static_cast<int>(position)
            );

            this->SetPosition(
                this->GetNextPosition("LazySequenceReadStream::Read")
            );

            return result;
        }

        bool IsCanSeek() const override
        {
            return true;
        }

        std::size_t Seek(std::size_t index) override
        {
            this->EnsureOpen("LazySequenceReadStream::Seek");

            Cardinal length =
                sequence_->GetLength();

            if (
                !length.IsInfinite() && index > length.GetValue()
            )
            {
                throw IndexOutOfRangeException(
                    index,
                    length.GetValue(),
                    "LazySequenceReadStream::Seek"
                );
            }

            ValidateSupportedPosition(index);

            this->SetPosition(index);

            return index;
        }

        bool IsCanGoBack() const override
        {
            return true;
        }
    };
}