#pragma once

#include <cstddef>

#include "read_only_stream.h"
#include "sequence.h"

namespace lab4
{
    template<class T>
    class SequenceReadStream : public ReadOnlyStream<T>
    {
    private:
        const Sequence<T>* sequence_;

        std::size_t GetSourceLength() const
        {
            int length = sequence_->GetLength();

            if (length < 0)
            {
                throw InvalidOperationException(
                    "Длина исходной последовательности не может быть отрицательной");
            }
            return static_cast<std::size_t>(length);
        }

    public:
        explicit SequenceReadStream(const Sequence<T>* sequence)
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
            this->EnsureOpen("SequenceReadStream::IsEndOfStream");

            return this->GetPosition() >= GetSourceLength();
        }

        T Read() override
        {
            this->EnsureOpen(
                "SequenceReadStream::Read"
            );

            if (IsEndOfStream())
            {
                throw EndOfStreamException("SequenceReadStream::Read");
            }

            std::size_t position = this->GetPosition();

            T result = sequence_->Get(static_cast<int>(position)
            );

            this->SetPosition(
                this->GetNextPosition("SequenceReadStream::Read")
            );
            return result;
        }

        bool IsCanSeek() const override
        {
            return true;
        }

        std::size_t Seek(
            std::size_t index
        ) override
        {
            this->EnsureOpen("SequenceReadStream::Seek");

            std::size_t length =GetSourceLength();

            if (index > length)
            {
                throw IndexOutOfRangeException(
                    index,
                    length,
                    "SequenceReadStream::Seek"
                );
            }

            this->SetPosition(index);

            return index;
        }

        bool IsCanGoBack() const override
        {
            return true;
        }
    };
}