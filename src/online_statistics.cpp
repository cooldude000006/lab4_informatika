#include "online_statistics.h"

#include <cmath>
#include <limits>

#include "exceptions.h"

namespace lab4
{
    OnlineStatistics::OnlineStatistics()
        : count_(0),
          sum_(0.0),
          minimum_(0.0),
          maximum_(0.0),
          mean_(0.0),
          squared_deviation_sum_(0.0)
    {
    }

    void OnlineStatistics::EnsureNotEmpty(const std::string& context) const
    {
        if (count_ == 0)
        {
            throw InvalidOperationException(
                "Статистика не содержит элементов, контекст=" + context);
        }
    }

    void OnlineStatistics::Reset()
    {
        count_ = 0;
        sum_ = 0.0;
        minimum_ = 0.0;
        maximum_ = 0.0;
        mean_ = 0.0;
        squared_deviation_sum_ = 0.0;
    }

    void OnlineStatistics::Add(double value)
    {
        if (!std::isfinite(value))
        {
            throw InvalidOperationException(
                "Невозможно добавить NaN или бесконечность в статистику");
        }

        if (
            count_ ==
            std::numeric_limits<std::size_t>::max()
        )
        {
            throw InvalidOperationException(
                "Переполнение количества элементов статистики");
        }

        std::size_t new_count = count_ + 1;

        if (count_ == 0)
        {
            count_ = new_count;
            sum_ = value;
            minimum_ = value;
            maximum_ = value;
            mean_ = value;
            squared_deviation_sum_ = 0.0;

            return;
        }

        double delta = value - mean_;

        mean_ += delta / static_cast<double>(new_count);

        double second_delta = value - mean_;

        squared_deviation_sum_ += delta * second_delta;

        sum_ += value;

        if (value < minimum_)
        {
            minimum_ = value;
        }

        if (value > maximum_)
        {
            maximum_ = value;
        }

        count_ = new_count;
    }

    std::size_t OnlineStatistics::ProcessNext(
        ReadOnlyStream<double>& stream,
        std::size_t max_items
    )
    {
        if (!stream.IsOpen())
        {
            throw InvalidOperationException(
                "Поток должен быть открыт перед сбором статистики");
        }

        std::size_t processed_count = 0;

        while (
            processed_count < max_items &&
            !stream.IsEndOfStream()
        )
        {
            Add(stream.Read());
            ++processed_count;
        }

        return processed_count;
    }

    std::size_t OnlineStatistics::GetCount() const
    {
        return count_;
    }

    double OnlineStatistics::GetSum() const
    {
        return sum_;
    }

    double OnlineStatistics::GetMinimum() const
    {
        EnsureNotEmpty("OnlineStatistics::GetMinimum");
        return minimum_;
    }

    double OnlineStatistics::GetMaximum() const
    {
        EnsureNotEmpty("OnlineStatistics::GetMaximum");
        return maximum_;
    }

    double OnlineStatistics::GetMean() const
    {
        EnsureNotEmpty("OnlineStatistics::GetMean");
        return mean_;
    }

    double OnlineStatistics::GetVariance() const
    {
        EnsureNotEmpty("OnlineStatistics::GetVariance");

        double variance = squared_deviation_sum_ / static_cast<double>(count_);

        if (variance < 0.0)
        {
            return 0.0;
        }
        return variance;
    }

    double OnlineStatistics::GetStandardDeviation() const
    {
        return std::sqrt(GetVariance());
    }
}