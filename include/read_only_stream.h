#pragma once

#include <cstddef>

#include "stream.h"

namespace lab4
{
    template<class T>
    class ReadOnlyStream : public Stream<T>
    {
    protected:
        ReadOnlyStream() = default;

    public:
        ~ReadOnlyStream() override = default;

        virtual bool IsEndOfStream() const = 0;

        virtual T Read() = 0;

        virtual bool IsCanSeek() const = 0;

        virtual std::size_t Seek(std::size_t index) = 0;

        virtual bool IsCanGoBack() const = 0;
    };
}