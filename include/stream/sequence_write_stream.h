#pragma once

#include <cstddef>

#include "../exceptions.h"
#include "../sequence/mutable_array_sequence.h"
#include "write_only_stream.h"

namespace lab4
{
    template<class T> class SequenceWriteStream :
        public WriteOnlyStream<T>
    {
    private:
        MutableArraySequence<T>* sequence_;

    public:
        explicit SequenceWriteStream(MutableArraySequence<T>* sequence)
            : sequence_(sequence)
        {
            if (sequence_ == nullptr)
            {
                throw InvalidOperationException(
                    "Невозможно создать поток записи для nullptr");
            }
        }

        std::size_t Write(const T& item) override
        {
            this->EnsureOpen("SequenceWriteStream::Write");

            std::size_t next_position =
                this->GetNextPosition("SequenceWriteStream::Write");

            sequence_->Append(item);
            this->SetPosition(next_position);

            return next_position;
        }
    };
}