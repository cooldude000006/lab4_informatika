#include <cmath>
#include <cstddef>
#include <limits>

#include <gtest/gtest.h>

#include "arithmetic_progression_generator.h"
#include "exceptions.h"
#include "lazy_sequence.h"
#include "lazy_sequence_read_stream.h"
#include "mutable_array_sequence.h"
#include "online_statistics.h"
#include "sequence_read_stream.h"

// Проверяет вычисление основных статистических характеристик
TEST(OnlineStatisticsTests, CalculatesKnownStatistics)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(1.0);
    statistics.Add(2.0);
    statistics.Add(3.0);
    statistics.Add(4.0);

    EXPECT_EQ(statistics.GetCount(), 4U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 10.0);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), 1.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), 4.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 2.5);
    EXPECT_NEAR(statistics.GetVariance(), 1.25, 1e-12);
    EXPECT_NEAR(statistics.GetStandardDeviation(), std::sqrt(1.25), 1e-12);
}

// Проверяет поведение статистики без добавленных элементов
TEST(OnlineStatisticsTests, EmptyStatisticsHasNoMeanOrVariance)
{
    lab4::OnlineStatistics statistics;

    EXPECT_EQ(statistics.GetCount(), 0U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 0.0);

    EXPECT_THROW(statistics.GetMinimum(), lab4::InvalidOperationException);
    EXPECT_THROW(statistics.GetMaximum(), lab4::InvalidOperationException);
    EXPECT_THROW(statistics.GetMean(), lab4::InvalidOperationException);
    EXPECT_THROW(statistics.GetVariance(), lab4::InvalidOperationException);
    EXPECT_THROW(statistics.GetStandardDeviation(), lab4::InvalidOperationException);
}

// Проверяет полную очистку накопленной статистики методом Reset
TEST(OnlineStatisticsTests, ResetClearsAllAccumulatedValues)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(10.0);
    statistics.Add(20.0);

    ASSERT_EQ(statistics.GetCount(), 2U);

    statistics.Reset();

    EXPECT_EQ(statistics.GetCount(), 0U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 0.0);
    EXPECT_THROW(statistics.GetMean(), lab4::InvalidOperationException);
}

// Проверяет запрет добавления NaN и бесконечных значений
TEST(OnlineStatisticsTests, RejectsNaNAndInfinity)
{
    lab4::OnlineStatistics statistics;

    EXPECT_THROW(
        statistics.Add(std::numeric_limits<double>::quiet_NaN()),
        lab4::InvalidOperationException
    );

    EXPECT_THROW(
        statistics.Add(std::numeric_limits<double>::infinity()),
        lab4::InvalidOperationException
    );

    EXPECT_THROW(
        statistics.Add(-std::numeric_limits<double>::infinity()),
        lab4::InvalidOperationException
    );

    EXPECT_EQ(statistics.GetCount(), 0U);
}

// Проверяет устойчивое вычисление дисперсии для больших близких чисел
TEST(OnlineStatisticsTests, CalculatesStableVarianceForLargeValues)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(1000000004.0);
    statistics.Add(1000000007.0);
    statistics.Add(1000000013.0);
    statistics.Add(1000000016.0);

    EXPECT_NEAR(statistics.GetMean(), 1000000010.0, 1e-9);
    EXPECT_NEAR(statistics.GetVariance(), 22.5, 1e-9);
}

// Проверяет обработку конечного потока несколькими порциями
TEST(OnlineStatisticsTests, ProcessesFiniteStreamInParts)
{
    double items[] = {1.0, 2.0, 3.0, 4.0};

    lab4::MutableArraySequence<double> sequence(items, 4);
    lab4::SequenceReadStream<double> stream(&sequence);
    lab4::OnlineStatistics statistics;

    EXPECT_THROW(statistics.ProcessNext(stream, 2), lab4::InvalidOperationException);

    stream.Open();

    EXPECT_EQ(statistics.ProcessNext(stream, 2), 2U);
    EXPECT_EQ(statistics.GetCount(), 2U);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 1.5);
    EXPECT_EQ(stream.GetPosition(), 2U);

    EXPECT_EQ(statistics.ProcessNext(stream, 10), 2U);
    EXPECT_EQ(statistics.GetCount(), 4U);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 2.5);
    EXPECT_TRUE(stream.IsEndOfStream());

    EXPECT_EQ(statistics.ProcessNext(stream, 10), 0U);

    stream.Close();
}

// Проверяет ограниченную обработку бесконечного ленивого потока
TEST(OnlineStatisticsTests, ProcessesLimitedPartOfInfiniteStream)
{
    lab4::ArithmeticProgressionGenerator<double> generator(1.0, 1.0);

    lab4::LazySequence<double> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<double> stream(&sequence);
    lab4::OnlineStatistics statistics;

    stream.Open();

    std::size_t processed = statistics.ProcessNext(stream, 5);

    EXPECT_EQ(processed, 5U);
    EXPECT_EQ(statistics.GetCount(), 5U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 15.0);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), 1.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), 5.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 3.0);
    EXPECT_NEAR(statistics.GetVariance(), 2.0, 1e-12);

    EXPECT_EQ(sequence.GetMaterializedCount(), 5U);
    EXPECT_FALSE(stream.IsEndOfStream());

    stream.Close();
}
