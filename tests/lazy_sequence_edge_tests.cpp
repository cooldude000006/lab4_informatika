
#include <gtest/gtest.h>

#include "exceptions.h"
#include "lazy/lazy_sequence.h"
#include "sequence/sequence.h"

// Проверяет исключения при обращении к пустой последовательности
TEST(LazySequenceEdgeTests, EmptySequenceRejectsFirstAndLast)
{
    lab4::LazySequence<int> sequence;

    EXPECT_THROW(sequence.GetFirst(), lab4::IndexOutOfRangeException);
    EXPECT_THROW(sequence.GetLast(), lab4::IndexOutOfRangeException);
}

// Проверяет сохранение данных после уничтожения источника присваивания
TEST(LazySequenceEdgeTests, CopyAssignmentSurvivesSourceDestruction)
{
    lab4::LazySequence<int> target;

    {
        int items[] = {10, 20, 30, 40};

        lab4::LazySequence<int> source(items, 4);

        EXPECT_EQ(source.Get(1), 20);
        EXPECT_EQ(source.GetMaterializedCount(), 2U);

        target = source;
    }

    EXPECT_EQ(target.GetLength().GetValue(), 4U);
    EXPECT_EQ(target.GetMaterializedCount(), 2U);
    EXPECT_EQ(target.Get(0), 10);
    EXPECT_EQ(target.Get(1), 20);
    EXPECT_EQ(target.Get(2), 30);
    EXPECT_EQ(target.Get(3), 40);
}

// Проверяет запрет вставки по отрицательному индексу
TEST(LazySequenceEdgeTests, InsertAtRejectsNegativeIndex)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.InsertAt(100, -1), lab4::IndexOutOfRangeException);
}

// Проверяет запрет вставки после последнего допустимого индекса
TEST(LazySequenceEdgeTests, InsertAtRejectsIndexPastEnd)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.InsertAt(100, 3), lab4::IndexOutOfRangeException);
}

// Проверяет запрет сцепления с нулевым указателем
TEST(LazySequenceEdgeTests, ConcatRejectsNullPointer)
{
    int items[] = {1, 2, 3};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.Concat(nullptr), lab4::InvalidOperationException);
}

// Проверяет отрицательный начальный индекс подпоследовательности
TEST(LazySequenceEdgeTests, SubsequenceRejectsNegativeStartIndex)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(-1, 1), lab4::IndexOutOfRangeException);
}

// Проверяет конечный индекс за границей последовательности
TEST(LazySequenceEdgeTests, SubsequenceRejectsEndIndexPastLength)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(1, 3), lab4::IndexOutOfRangeException);
}

// Проверяет диапазон с начальным индексом больше конечного
TEST(LazySequenceEdgeTests, SubsequenceRejectsReversedRange)
{
    int items[] = {10, 20, 30};

    lab4::LazySequence<int> sequence(items, 3);

    EXPECT_THROW(sequence.GetSubsequence(2, 1), lab4::InvalidOperationException);
}

// Проверяет запрет создания последовательности из nullptr
TEST(LazySequenceEdgeTests, SequenceConstructorRejectsNullPointer)
{
    const lab4::Sequence<int>* source = nullptr;

    EXPECT_THROW(lab4::LazySequence<int>(source), lab4::InvalidOperationException);
}

// Проверяет запрет положительного размера при нулевом указателе на массив
TEST(LazySequenceEdgeTests, ArrayConstructorRejectsNullPointerWithPositiveCount)
{
    int* items = nullptr;

    EXPECT_THROW(lab4::LazySequence<int>(items, 1), lab4::InvalidOperationException);
}

// Проверяет запрет отрицательного количества элементов массива
TEST(LazySequenceEdgeTests, ArrayConstructorRejectsNegativeCount)
{
    int items[] = {1, 2, 3};

    EXPECT_THROW(lab4::LazySequence<int>(items, -1), lab4::InvalidOperationException);
}