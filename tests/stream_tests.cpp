#include <gtest/gtest.h>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "exceptions.h"
#include "../include/lazy/lazy_sequence.h"
#include "../include/stream/lazy_sequence_read_stream.h"
#include "../include/sequence/mutable_array_sequence.h"
#include "../include/stream/sequence_read_stream.h"
#include "../include/stream/sequence_write_stream.h"

// Проверяет запрет чтения до открытия потока
TEST(SequenceReadStreamTests, ReadRequiresOpenStream)
{
    int items[] = {10, 20};

    lab4::MutableArraySequence<int> sequence(items, 2);
    lab4::SequenceReadStream<int> stream(&sequence);

    EXPECT_THROW(stream.Read(), lab4::InvalidOperationException);
}

// Проверяет последовательное чтение и изменение позиции
TEST(SequenceReadStreamTests, ReadReturnsValuesAndUpdatesPosition)
{
    int items[] = {10, 20, 30};

    lab4::MutableArraySequence<int> sequence(items, 3);
    lab4::SequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(stream.GetPosition(), 0U);
    EXPECT_EQ(stream.Read(), 10);
    EXPECT_EQ(stream.GetPosition(), 1U);
    EXPECT_EQ(stream.Read(), 20);
    EXPECT_EQ(stream.GetPosition(), 2U);

    stream.Close();
}

// Проверяет определение конца и исключение при чтении после него
TEST(SequenceReadStreamTests, EndOfStreamIsReportedAndReadThrows)
{
    int items[] = {7};

    lab4::MutableArraySequence<int> sequence(items, 1);
    lab4::SequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_FALSE(stream.IsEndOfStream());
    EXPECT_EQ(stream.Read(), 7);
    EXPECT_TRUE(stream.IsEndOfStream());
    EXPECT_THROW(stream.Read(), lab4::EndOfStreamException);

    stream.Close();
}

// Проверяет переход вперёд и назад через Seek
TEST(SequenceReadStreamTests, SeekMovesToRequestedPosition)
{
    int items[] = {10, 20, 30, 40};

    lab4::MutableArraySequence<int> sequence(items, 4);
    lab4::SequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_TRUE(stream.IsCanSeek());
    EXPECT_TRUE(stream.IsCanGoBack());

    EXPECT_EQ(stream.Seek(2), 2U);
    EXPECT_EQ(stream.Read(), 30);

    EXPECT_EQ(stream.Seek(0), 0U);
    EXPECT_EQ(stream.Read(), 10);

    EXPECT_EQ(stream.Seek(4), 4U);
    EXPECT_TRUE(stream.IsEndOfStream());

    EXPECT_THROW(stream.Seek(5), lab4::IndexOutOfRangeException);

    stream.Close();
}

// Проверяет материализацию только действительно прочитанных элементов
TEST(LazySequenceReadStreamTests, ReadMaterializesRequestedElements)
{
    lab4::ArithmeticProgressionGenerator<int> generator(2, 3);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    EXPECT_EQ(stream.Read(), 2);
    EXPECT_EQ(sequence.GetMaterializedCount(), 1U);

    EXPECT_EQ(stream.Read(), 5);
    EXPECT_EQ(sequence.GetMaterializedCount(), 2U);

    EXPECT_EQ(stream.Read(), 8);
    EXPECT_EQ(sequence.GetMaterializedCount(), 3U);

    stream.Close();
}

// Проверяет, что Seek сам не материализует ленивую последовательность
TEST(LazySequenceReadStreamTests, SeekDoesNotMaterializeUntilRead)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(stream.Seek(10), 10U);
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    EXPECT_EQ(stream.Read(), 11);
    EXPECT_EQ(sequence.GetMaterializedCount(), 11U);

    stream.Close();
}

// Проверяет, что бесконечный поток не сообщает о конце
TEST(LazySequenceReadStreamTests, InfiniteStreamDoesNotReachEnd)
{
    lab4::ArithmeticProgressionGenerator<int> generator(1, 1);
    lab4::LazySequence<int> sequence(generator, lab4::Cardinal::Infinity());
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    for (int index = 0; index < 100; ++index)
    {
        EXPECT_FALSE(stream.IsEndOfStream());
        EXPECT_EQ(stream.Read(), index + 1);
    }

    EXPECT_FALSE(stream.IsEndOfStream());

    stream.Close();
}

// Проверяет добавление элементов через поток записи
TEST(SequenceWriteStreamTests, WriteAppendsValuesAndUpdatesPosition)
{
    lab4::MutableArraySequence<int> target;
    lab4::SequenceWriteStream<int> stream(&target);

    stream.Open();

    EXPECT_EQ(stream.Write(10), 1U);
    EXPECT_EQ(stream.Write(20), 2U);
    EXPECT_EQ(stream.GetPosition(), 2U);

    stream.Close();

    ASSERT_EQ(target.GetLength(), 2);
    EXPECT_EQ(target.Get(0), 10);
    EXPECT_EQ(target.Get(1), 20);
}

// Проверяет сброс позиции при повторном открытии без очистки данных
TEST(SequenceWriteStreamTests, ReopeningResetsPositionButPreservesTarget)
{
    lab4::MutableArraySequence<int> target;
    lab4::SequenceWriteStream<int> stream(&target);

    stream.Open();
    stream.Write(10);
    stream.Write(20);
    stream.Close();

    ASSERT_EQ(target.GetLength(), 2);

    stream.Open();

    EXPECT_EQ(stream.GetPosition(), 0U);
    EXPECT_EQ(stream.Write(30), 1U);

    stream.Close();

    ASSERT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 10);
    EXPECT_EQ(target.Get(1), 20);
    EXPECT_EQ(target.Get(2), 30);
}