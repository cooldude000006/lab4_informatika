#include <memory>

#include <gtest/gtest.h>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "exceptions.h"
#include "../include/lazy/generators/finite_sequence_generator.h"
#include "../include/lazy/igenerator.h"
#include "option.h"

// Проверяет генерацию элементов арифметической прогрессии
TEST(ArithmeticProgressionGeneratorTests, GeneratesExpectedValues)
{
    lab4::ArithmeticProgressionGenerator<int> generator(2, 3);

    EXPECT_TRUE(generator.HasNext());

    EXPECT_EQ(generator.GetNext(), 2);
    EXPECT_EQ(generator.GetNext(), 5);
    EXPECT_EQ(generator.GetNext(), 8);
    EXPECT_EQ(generator.GetNext(), 11);

    EXPECT_TRUE(generator.HasNext());
}

// Проверяет независимость состояния клонированного генератора
TEST(ArithmeticProgressionGeneratorTests, CloneHasIndependentState)
{
    lab4::ArithmeticProgressionGenerator<int> generator(10, 5);

    EXPECT_EQ(generator.GetNext(), 10);

    std::unique_ptr<lab4::IGenerator<int>> clone(generator.Clone());

    EXPECT_EQ(generator.GetNext(), 15);
    EXPECT_EQ(generator.GetNext(), 20);

    EXPECT_EQ(clone->GetNext(), 15);
    EXPECT_EQ(clone->GetNext(), 20);
}

// Проверяет последовательную выдачу всех элементов конечного массива
TEST(FiniteSequenceGeneratorTests, GeneratesEveryArrayElement)
{
    int items[] = {10, 20, 30};

    lab4::FiniteSequenceGenerator<int> generator(items, 3);

    EXPECT_TRUE(generator.HasNext());
    EXPECT_EQ(generator.GetNext(), 10);

    EXPECT_TRUE(generator.HasNext());
    EXPECT_EQ(generator.GetNext(), 20);

    EXPECT_TRUE(generator.HasNext());
    EXPECT_EQ(generator.GetNext(), 30);

    EXPECT_FALSE(generator.HasNext());
}

// Проверяет исключение при чтении после конца генератора
TEST(FiniteSequenceGeneratorTests, GetNextThrowsAfterEnd)
{
    int items[] = {7};

    lab4::FiniteSequenceGenerator<int> generator(items, 1);

    EXPECT_EQ(generator.GetNext(), 7);
    EXPECT_FALSE(generator.HasNext());

    EXPECT_THROW(generator.GetNext(), lab4::IndexOutOfRangeException);
}

// Проверяет возврат пустого Option после завершения генератора
TEST(FiniteSequenceGeneratorTests, TryGetNextReturnsNoneAfterEnd)
{
    int items[] = {4};

    lab4::FiniteSequenceGenerator<int> generator(items, 1);

    lab4::Option<int> first = generator.TryGetNext();

    EXPECT_TRUE(first.HasValue());
    EXPECT_EQ(first.GetValue(), 4);

    lab4::Option<int> after_end = generator.TryGetNext();

    EXPECT_FALSE(after_end.HasValue());
}
