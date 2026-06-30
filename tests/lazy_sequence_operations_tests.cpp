#include <memory>

#include <gtest/gtest.h>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "exceptions.h"
#include "../include/lazy/lazy_sequence.h"

// Преобразует целое число в его половину типа double
class DivideByTwo
{
public:
    double operator()(int value) const
    {
        return static_cast<double>(value) / 2.0;
    }
};

// Проверяет, является ли целое число чётным
class IsEven
{
public:
    bool operator()(int value) const
    {
        return value % 2 == 0;
    }
};

// Добавляет элемент к целочисленному аккумулятору
class AddToAccumulator
{
public:
    int operator()(int value, int accumulator) const
    {
        return value + accumulator;
    }
};

// Проверяет ленивое добавление элемента в начало
TEST(LazySequenceOperationsTests, PrependReturnsExpectedSequence)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> source(items, 3);

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.Prepend(5)
    );

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetLength().GetValue(), 4U);

    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(1), 10);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 30);
}

// Проверяет ленивую вставку элемента по индексу
TEST(LazySequenceOperationsTests, InsertAtReturnsExpectedSequence)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> source(items, 3);

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.InsertAt(15, 1)
    );

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetMaterializedCount(), 0U);

    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 15);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 30);
}

// Проверяет добавление элемента в конец конечной последовательности
TEST(LazySequenceOperationsTests, AppendAddsElementToFiniteSequence)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> source(items, 3);

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.Append(40)
    );

    EXPECT_EQ(result->GetLength().GetValue(), 4U);
    EXPECT_EQ(result->GetLast(), 40);

    EXPECT_EQ(source.GetLength().GetValue(), 3U);
    EXPECT_EQ(source.GetLast(), 30);
}

// Проверяет наблюдаемое поведение Append для бесконечной последовательности
TEST(LazySequenceOperationsTests, AppendToInfiniteSequenceReturnsEquivalentSequence)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> source(generator, lab4::Cardinal::Infinity());

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.Append(1000)
    );

    EXPECT_TRUE(result->GetLength().IsInfinite());

    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
}

// Проверяет сцепление двух конечных последовательностей
TEST(LazySequenceOperationsTests, ConcatCombinesFiniteSequences)
{
    int first_items[] = {1, 2, 3};
    int second_items[] = {4, 5};

    lab4::LazySequence<int> first(first_items, 3);
    lab4::LazySequence<int> second(second_items, 2);

    std::unique_ptr<lab4::LazySequence<int>> result(
        first.Concat(&second)
    );

    EXPECT_EQ(first.GetMaterializedCount(), 0U);
    EXPECT_EQ(second.GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetLength().GetValue(), 5U);

    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);
}

// Проверяет сцепление конечной и бесконечной последовательностей
TEST(LazySequenceOperationsTests, ConcatFiniteAndInfiniteReturnsInfiniteSequence)
{
    int prefix_items[] = {100, 200};

    lab4::LazySequence<int> prefix(prefix_items, 2);

    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> infinite(generator, lab4::Cardinal::Infinity());

    std::unique_ptr<lab4::LazySequence<int>> result(
        prefix.Concat(&infinite)
    );

    EXPECT_TRUE(result->GetLength().IsInfinite());

    EXPECT_EQ(result->Get(0), 100);
    EXPECT_EQ(result->Get(1), 200);
    EXPECT_EQ(result->Get(2), 1);
    EXPECT_EQ(result->Get(3), 2);
    EXPECT_EQ(result->Get(4), 3);
}

// Проверяет сцепление после частичной материализации источников
TEST(LazySequenceOperationsTests, ConcatPreservesPartiallyMaterializedSources)
{
    int first_items[] = {10, 20, 30};
    int second_items[] = {40, 50, 60};

    lab4::LazySequence<int> first(first_items, 3);
    lab4::LazySequence<int> second(second_items, 3);

    EXPECT_EQ(first.Get(1), 20);
    EXPECT_EQ(second.Get(0), 40);

    std::unique_ptr<lab4::LazySequence<int>> result(
        first.Concat(&second)
    );

    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 20);
    EXPECT_EQ(result->Get(2), 30);
    EXPECT_EQ(result->Get(3), 40);
    EXPECT_EQ(result->Get(4), 50);
    EXPECT_EQ(result->Get(5), 60);
}

// Проверяет ленивое преобразование с изменением типа элемента
TEST(LazySequenceOperationsTests, MapTransformsValuesAndChangesResultType)
{
    int items[] = {1, 2, 3, 4};

    lab4::LazySequence<int> source(items, 4);
    DivideByTwo mapper;

    std::unique_ptr<lab4::LazySequence<double>> result(
        source.Map<double>(mapper)
    );

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetLength().GetValue(), 4U);

    EXPECT_DOUBLE_EQ(result->Get(0), 0.5);
    EXPECT_DOUBLE_EQ(result->Get(1), 1.0);
    EXPECT_DOUBLE_EQ(result->Get(2), 1.5);
    EXPECT_DOUBLE_EQ(result->Get(3), 2.0);
}

// Проверяет фильтрацию конечной последовательности
TEST(LazySequenceOperationsTests, WhereKeepsOnlyMatchingElements)
{
    int items[] = {1, 2, 3, 4, 5, 6};

    lab4::LazySequence<int> source(items, 6);
    IsEven predicate;

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.Where(predicate)
    );

    EXPECT_EQ(source.GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetMaterializedCount(), 0U);
    EXPECT_EQ(result->GetLength().GetValue(), 3U);

    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 6);
}

// Проверяет пустой результат фильтрации
TEST(LazySequenceOperationsTests, WhereCanReturnEmptySequence)
{
    int items[] = {1, 3, 5};

    lab4::LazySequence<int> source(items, 3);
    IsEven predicate;

    std::unique_ptr<lab4::LazySequence<int>> result(
        source.Where(predicate)
    );

    EXPECT_EQ(result->GetLength().GetValue(), 0U);
    EXPECT_THROW(result->GetFirst(), lab4::IndexOutOfRangeException);
}

// Проверяет свёртку конечной последовательности
TEST(LazySequenceOperationsTests, ReduceReturnsExpectedAccumulator)
{
    int items[] = {1, 2, 3, 4};

    lab4::LazySequence<int> sequence(items, 4);
    AddToAccumulator operation;

    int result = sequence.Reduce(operation, 0);

    EXPECT_EQ(result, 10);
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);
}

// Проверяет запрет полной свёртки бесконечной последовательности
TEST(LazySequenceOperationsTests, ReduceThrowsForInfiniteSequence)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());
    AddToAccumulator operation;

    EXPECT_THROW(sequence.Reduce(operation, 0), lab4::InvalidOperationException);
}