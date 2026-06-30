#include <cstddef>
#include <memory>

#include <gtest/gtest.h>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "exceptions.h"
#include "../include/sequence/ienumerator.h"
#include "../include/lazy/lazy_sequence.h"
#include "option.h"

// Проверяет параметры пустой ленивой последовательности
TEST(LazySequenceBasicTests, EmptySequenceIsFiniteAndHasZeroLength)
{
    lab4::LazySequence<int> sequence;

    EXPECT_FALSE(sequence.GetLength().IsInfinite());
    EXPECT_EQ(sequence.GetLength().GetValue(), 0U);
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);
}

// Проверяет постепенную материализацию и повторное использование кэша
TEST(LazySequenceBasicTests, FiniteSequenceMaterializesOnlyRequestedPrefix)
{
    int items[] = {10, 20, 30, 40};

    lab4::LazySequence<int> sequence(items, 4);

    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    EXPECT_EQ(sequence.Get(2), 30);
    EXPECT_EQ(sequence.GetMaterializedCount(), 3U);

    EXPECT_EQ(sequence.Get(1), 20);
    EXPECT_EQ(sequence.GetMaterializedCount(), 3U);
}

// Проверяет ленивую генерацию бесконечной арифметической прогрессии
TEST(LazySequenceBasicTests, InfiniteProgressionGeneratesValuesOnDemand)
{
    lab4::ArithmeticProgressionGenerator<int> generator(2, 3);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());

    EXPECT_TRUE(sequence.GetLength().IsInfinite());
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    EXPECT_EQ(sequence.Get(4), 14);
    EXPECT_EQ(sequence.GetMaterializedCount(), 5U);
}

// Проверяет исключение при отрицательном индексе
TEST(LazySequenceBasicTests, NegativeIndexThrows)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.Get(-1), lab4::IndexOutOfRangeException);
}

// Проверяет исключение при индексе за границей конечной последовательности
TEST(LazySequenceBasicTests, FiniteIndexPastEndThrows)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.Get(3), lab4::IndexOutOfRangeException);
}

// Проверяет получение первого и последнего элементов
TEST(LazySequenceBasicTests, GetFirstAndGetLastReturnExpectedValues)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_EQ(sequence.GetFirst(), 10);
    EXPECT_EQ(sequence.GetMaterializedCount(), 1U);

    EXPECT_EQ(sequence.GetLast(), 30);
    EXPECT_EQ(sequence.GetMaterializedCount(), 3U);
}

// Проверяет запрет получения последнего элемента бесконечной последовательности
TEST(LazySequenceBasicTests, GetLastThrowsForInfiniteSequence)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());

    EXPECT_THROW(sequence.GetLast(), lab4::InvalidOperationException);
}

// Проверяет независимость кэшей скопированных последовательностей
TEST(LazySequenceBasicTests, CopyHasIndependentMaterializationState)
{
    int items[] = {1, 2, 3, 4};

    lab4::LazySequence<int> original(items, 4);

    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(original.GetMaterializedCount(), 2U);

    lab4::LazySequence<int> copy(original);

    EXPECT_EQ(original.Get(3), 4);
    EXPECT_EQ(original.GetMaterializedCount(), 4U);

    EXPECT_EQ(copy.Get(2), 3);
    EXPECT_EQ(copy.GetMaterializedCount(), 3U);
}

// Проверяет ленивое создание подпоследовательности
TEST(LazySequenceBasicTests, SubsequenceCreationDoesNotMaterializeSource)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> source(generator, lab4::Cardinal::Infinity());

    std::unique_ptr<lab4::LazySequence<int>> subsequence(
        source.GetSubsequence(3, 5)
    );

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(subsequence->GetMaterializedCount(), 0U);
    EXPECT_EQ(subsequence->GetLength().GetValue(), 3U);

    EXPECT_EQ(subsequence->Get(0), 4);
    EXPECT_EQ(subsequence->Get(1), 5);
    EXPECT_EQ(subsequence->Get(2), 6);

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(subsequence->GetMaterializedCount(), 3U);
}

// Проверяет последовательный обход и сброс итератора
TEST(LazySequenceBasicTests, EnumeratorTraversesSequenceAndResets)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    std::unique_ptr<lab4::IEnumerator<int>> enumerator(
        sequence.GetEnumerator()
    );

    EXPECT_FALSE(enumerator->GetCurrent().HasValue());

    ASSERT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->GetCurrent().GetValue(), 10);

    ASSERT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->GetCurrent().GetValue(), 20);

    ASSERT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->GetCurrent().GetValue(), 30);

    EXPECT_FALSE(enumerator->MoveNext());
    EXPECT_FALSE(enumerator->GetCurrent().HasValue());

    enumerator->Reset();

    EXPECT_FALSE(enumerator->GetCurrent().HasValue());
    ASSERT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->GetCurrent().GetValue(), 10);
}