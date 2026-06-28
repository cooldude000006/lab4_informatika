#pragma once

#include <cstddef>

#include "stream.h"

namespace lab4
{
    template<class T>
    class WriteOnlyStream : public Stream<T>
    {
    protected:
        WriteOnlyStream() = default;

    public:
        ~WriteOnlyStream() override = default;

        virtual std::size_t Write(const T& item) = 0;
    };
}