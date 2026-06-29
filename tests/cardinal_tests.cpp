#include <cstddef>

#include <gtest/gtest.h>

#include "cardinal.h"
#include "exceptions.h"

// Проверяет, что Cardinal по умолчанию конечный и равен нулю
TEST(CardinalTests, DefaultCardinalIsFiniteZero)
{
    lab4::Cardinal cardinal;

    EXPECT_FALSE(cardinal.IsInfinite());
    EXPECT_EQ(cardinal.GetValue(), 0U);
}

// Проверяет сохранение заданного конечного значения
TEST(CardinalTests, FiniteCardinalStoresValue)
{
    const std::size_t expected_value = 125;

    lab4::Cardinal cardinal(expected_value);

    EXPECT_FALSE(cardinal.IsInfinite());
    EXPECT_EQ(cardinal.GetValue(), expected_value);
}

// Проверяет бесконечный Cardinal и запрет получения конечного значения
TEST(CardinalTests, InfiniteCardinalHasNoFiniteValue)
{
    lab4::Cardinal cardinal = lab4::Cardinal::Infinity();

    EXPECT_TRUE(cardinal.IsInfinite());
    EXPECT_THROW(cardinal.GetValue(), lab4::InvalidOperationException);
}
