#include <cstddef>
#include <memory>

#include <gtest/gtest.h>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "../include/lazy/lazy_sequence.h"
#include "../include/stream/lazy_sequence_read_stream.h"
#include "../include/sequence/mutable_array_sequence.h"
#include "online_statistics.h"

// Проверяет добавление миллиона элементов без потери данных
TEST(LargeDataTests, MutableSequenceHandlesMillionAppends)
{
    constexpr int element_count = 1'000'000;
    constexpr int middle_index = element_count / 2;

    lab4::MutableArraySequence<int> sequence;

    for (int value = 0; value < element_count; ++value)
    {
        sequence.Append(value);
    }

    ASSERT_EQ(sequence.GetLength(), element_count);
    EXPECT_EQ(sequence.GetFirst(), 0);
    EXPECT_EQ(sequence.Get(middle_index), middle_index);
    EXPECT_EQ(sequence.GetLast(), element_count - 1);
}

// Проверяет материализацию большого префикса ленивой последовательности
TEST(LargeDataTests, LazySequenceMaterializesLargeRequestedPrefix)
{
    constexpr int requested_index = 100'000;

    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());

    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);
    EXPECT_EQ(sequence.Get(requested_index), requested_index + 1);
    EXPECT_EQ(sequence.GetMaterializedCount(), static_cast<std::size_t>(requested_index + 1));

    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.GetMaterializedCount(), static_cast<std::size_t>(requested_index + 1));
}

// Проверяет статистику для ста тысяч элементов бесконечного потока
TEST(LargeDataTests, OnlineStatisticsProcessesLargeInfiniteStream)
{
    constexpr std::size_t element_count = 100'000;

    lab4::ArithmeticProgressionGenerator<double> generator(1.0, 1.0);
    lab4::LazySequence<double> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<double> stream(&sequence);
    lab4::OnlineStatistics statistics;

    stream.Open();

    std::size_t processed = statistics.ProcessNext(stream, element_count);

    stream.Close();

    double count = static_cast<double>(element_count);
    double expected_sum = count * (count + 1.0) / 2.0;
    double expected_mean = (count + 1.0) / 2.0;
    double expected_variance = (count * count - 1.0) / 12.0;
    double variance_tolerance = expected_variance * 1e-12;

    EXPECT_EQ(processed, element_count);
    EXPECT_EQ(statistics.GetCount(), element_count);
    EXPECT_DOUBLE_EQ(statistics.GetMinimum(), 1.0);
    EXPECT_DOUBLE_EQ(statistics.GetMaximum(), count);
    EXPECT_DOUBLE_EQ(statistics.GetSum(), expected_sum);
    EXPECT_DOUBLE_EQ(statistics.GetMean(), expected_mean);
    EXPECT_NEAR(statistics.GetVariance(), expected_variance, variance_tolerance);
    EXPECT_EQ(sequence.GetMaterializedCount(), element_count);
}

// Проверяет повторное чтение большого кэша после возврата назад
TEST(LargeDataTests, BackwardSeekReusesLargeMaterializedCache)
{
    constexpr std::size_t target_index = 100'000;

    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(stream.Seek(target_index), target_index);
    EXPECT_EQ(stream.Read(), static_cast<int>(target_index + 1));
    EXPECT_EQ(sequence.GetMaterializedCount(), target_index + 1);

    EXPECT_EQ(stream.Seek(0), 0U);
    EXPECT_EQ(stream.Read(), 1);
    EXPECT_EQ(sequence.GetMaterializedCount(), target_index + 1);

    stream.Close();
}

// Проверяет ленивую подпоследовательность с большим начальным индексом
TEST(LargeDataTests, LargeSubsequenceDoesNotMaterializeOriginalSource)
{
    constexpr int start_index = 90'000;
    constexpr int end_index = 90'009;

    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> source(generator, lab4::Cardinal::Infinity());

    std::unique_ptr<lab4::LazySequence<int>> subsequence(source.GetSubsequence(start_index, end_index));

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(subsequence->GetMaterializedCount(), 0U);
    EXPECT_EQ(subsequence->GetLength().GetValue(), 10U);

    EXPECT_EQ(subsequence->GetFirst(), start_index + 1);
    EXPECT_EQ(subsequence->GetLast(), end_index + 1);

    EXPECT_EQ(subsequence->GetMaterializedCount(), 10U);
    EXPECT_EQ(source.GetMaterializedCount(), 0U);
}
