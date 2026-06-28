#pragma once
#include <cstddef>
namespace lab4
{
    template<class T>
    class ICollection
    {
    public:
        virtual ~ICollection() = default;
        virtual T Get(std::size_t index) const = 0;
        virtual std::size_t GetCount() const = 0;
    };
}