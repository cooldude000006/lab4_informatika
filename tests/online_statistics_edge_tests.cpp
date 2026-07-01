#include <limits>

#include <gtest/gtest.h>

#include "exceptions.h"
#include "online_statistics.h"
#include "sequence/mutable_array_sequence.h"
#include "stream/sequence_read_stream.h"

// Проверяет нулевую дисперсию для одного элемента
TEST(OnlineStatisticsEdgeTests, SingleValueHasZeroVariance)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(25.0);

    EXPECT_EQ(statistics.GetCount(), 1U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 25.0);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), 25.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), 25.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 25.0);
    EXPECT_DOUBLE_EQ(statistics.GetVariance(), 0.0);
    EXPECT_DOUBLE_EQ(statistics.GetStandardDeviation(), 0.0);
}

// Проверяет вычисление статистики для отрицательных значений
TEST(OnlineStatisticsEdgeTests, HandlesNegativeValues)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(-5.0);
    statistics.Add(-2.0);
    statistics.Add(-9.0);

    EXPECT_EQ(statistics.GetCount(), 3U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), -16.0);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), -9.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), -2.0);
    EXPECT_NEAR(statistics.GetMean(), -16.0 / 3.0, 1e-12);
    EXPECT_NEAR(statistics.GetVariance(), 74.0 / 9.0, 1e-12);
}

// Проверяет отсутствие чтения при нулевом лимите
TEST(OnlineStatisticsEdgeTests, ProcessNextWithZeroLimitDoesNotReadStream)
{
    double items[] = {1.0, 2.0, 3.0};

    lab4::MutableArraySequence<double> sequence(items, 3);
    lab4::SequenceReadStream<double> stream(&sequence);
    lab4::OnlineStatistics statistics;

    stream.Open();

    EXPECT_EQ(statistics.ProcessNext(stream, 0), 0U);
    EXPECT_EQ(statistics.GetCount(), 0U);
    EXPECT_EQ(stream.GetPosition(), 0U);
    EXPECT_FALSE(stream.IsEndOfStream());

    stream.Close();
}

// Проверяет сохранение состояния после отклонения бесконечного значения
TEST(OnlineStatisticsEdgeTests, InvalidValueDoesNotChangeExistingStatistics)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(10.0);
    statistics.Add(20.0);

    EXPECT_THROW(
        statistics.Add(std::numeric_limits<double>::infinity()),
        lab4::InvalidOperationException
    );

    EXPECT_EQ(statistics.GetCount(), 2U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 30.0);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), 10.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), 20.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 15.0);
    EXPECT_DOUBLE_EQ(statistics.GetVariance(), 25.0);
}

// Проверяет сохранение состояния после отклонения NaN
TEST(OnlineStatisticsEdgeTests, NaNDoesNotChangeExistingStatistics)
{
    lab4::OnlineStatistics statistics;

    statistics.Add(7.0);

    EXPECT_THROW(
        statistics.Add(std::numeric_limits<double>::quiet_NaN()),
        lab4::InvalidOperationException
    );

    EXPECT_EQ(statistics.GetCount(), 1U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 7.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 7.0);
}

// Проверяет накопление результатов несколькими вызовами ProcessNext
TEST(OnlineStatisticsEdgeTests, ProcessNextContinuesPreviousStatistics)
{
    double items[] = {2.0, 4.0, 6.0, 8.0};

    lab4::MutableArraySequence<double> sequence(items, 4);
    lab4::SequenceReadStream<double> stream(&sequence);
    lab4::OnlineStatistics statistics;

    stream.Open();

    EXPECT_EQ(statistics.ProcessNext(stream, 1), 1U);
    EXPECT_EQ(statistics.GetCount(), 1U);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 2.0);

    EXPECT_EQ(statistics.ProcessNext(stream, 2), 2U);
    EXPECT_EQ(statistics.GetCount(), 3U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 12.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 4.0);

    EXPECT_EQ(statistics.ProcessNext(stream, 10), 1U);
    EXPECT_EQ(statistics.GetCount(), 4U);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), 20.0);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), 5.0);
    EXPECT_TRUE(stream.IsEndOfStream());

    stream.Close();
}