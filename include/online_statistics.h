#pragma once

#include <cstddef>
#include <string>

#include "read_only_stream.h"

namespace lab4
{
    class OnlineStatistics
    {
    private:
        std::size_t count_;
        double sum_;
        double minimum_;
        double maximum_;
        double mean_;
        double squared_deviation_sum_;

        void EnsureNotEmpty(const std::string& context) const;

    public:
        OnlineStatistics();

        void Reset();

        void Add(double value);

        std::size_t ProcessNext(
            ReadOnlyStream<double>& stream,
            std::size_t max_items
        );

        std::size_t GetCount() const;

        double GetSum() const;

        double GetMinimum() const;

        double GetMaximum() const;

        double GetMean() const;

        double GetVariance() const;

        double GetStandardDeviation() const;
    };
}