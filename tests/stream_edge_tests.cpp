#include <gtest/gtest.h>

#include "exceptions.h"
#include "lazy/lazy_sequence.h"
#include "sequence/mutable_array_sequence.h"
#include "stream/lazy_sequence_read_stream.h"
#include "stream/sequence_read_stream.h"
#include "stream/sequence_write_stream.h"

// Проверяет запрет повторного открытия уже открытого потока
TEST(StreamEdgeTests, OpeningAlreadyOpenedStreamThrows)
{
    int items[] = {10, 20};

    lab4::MutableArraySequence<int> sequence(items, 2);
    lab4::SequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_THROW(stream.Open(), lab4::InvalidOperationException);
    EXPECT_TRUE(stream.IsOpen());

    stream.Close();
}

// Проверяет запрет закрытия неоткрытого потока
TEST(StreamEdgeTests, ClosingClosedStreamThrows)
{
    int items[] = {10, 20};

    lab4::MutableArraySequence<int> sequence(items, 2);
    lab4::SequenceReadStream<int> stream(&sequence);

    EXPECT_THROW(stream.Close(), lab4::InvalidOperationException);
    EXPECT_FALSE(stream.IsOpen());
}

// Проверяет сброс позиции при повторном открытии потока чтения
TEST(StreamEdgeTests, ReopeningReadStreamResetsPosition)
{
    int items[] = {10, 20, 30};

    lab4::MutableArraySequence<int> sequence(items, 3);
    lab4::SequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(stream.Read(), 10);
    EXPECT_EQ(stream.Read(), 20);
    EXPECT_EQ(stream.GetPosition(), 2U);

    stream.Close();
    stream.Open();

    EXPECT_EQ(stream.GetPosition(), 0U);
    EXPECT_EQ(stream.Read(), 10);

    stream.Close();
}

// Проверяет запрет проверки конца до открытия потока
TEST(StreamEdgeTests, EndOfStreamCheckRequiresOpenStream)
{
    int items[] = {10};

    lab4::MutableArraySequence<int> sequence(items, 1);
    lab4::SequenceReadStream<int> stream(&sequence);

    EXPECT_THROW(stream.IsEndOfStream(), lab4::InvalidOperationException);
}

// Проверяет запрет перемещения до открытия потока
TEST(StreamEdgeTests, SeekRequiresOpenStream)
{
    int items[] = {10, 20};

    lab4::MutableArraySequence<int> sequence(items, 2);
    lab4::SequenceReadStream<int> stream(&sequence);

    EXPECT_THROW(stream.Seek(1), lab4::InvalidOperationException);
}

// Проверяет запрет записи до открытия потока
TEST(StreamEdgeTests, WriteRequiresOpenStream)
{
    lab4::MutableArraySequence<int> sequence;
    lab4::SequenceWriteStream<int> stream(&sequence);

    EXPECT_THROW(stream.Write(10), lab4::InvalidOperationException);
    EXPECT_EQ(sequence.GetLength(), 0);
    EXPECT_EQ(stream.GetPosition(), 0U);
}

// Проверяет запрет создания обычного потока чтения из nullptr
TEST(StreamEdgeTests, SequenceReadStreamRejectsNullPointer)
{
    EXPECT_THROW(lab4::SequenceReadStream<int>(nullptr), lab4::InvalidOperationException);
}

// Проверяет запрет создания ленивого потока чтения из nullptr
TEST(StreamEdgeTests, LazySequenceReadStreamRejectsNullPointer)
{
    EXPECT_THROW(lab4::LazySequenceReadStream<int>(nullptr), lab4::InvalidOperationException);
}

// Проверяет запрет создания потока записи из nullptr
TEST(StreamEdgeTests, SequenceWriteStreamRejectsNullPointer)
{
    EXPECT_THROW(lab4::SequenceWriteStream<int>(nullptr), lab4::InvalidOperationException);
}

// Проверяет достижение конца конечного ленивого потока
TEST(StreamEdgeTests, FiniteLazyStreamReportsEnd)
{
    int items[] = {5, 10};

    lab4::LazySequence<int> sequence(items, 2);
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_FALSE(stream.IsEndOfStream());
    EXPECT_EQ(stream.Read(), 5);

    EXPECT_FALSE(stream.IsEndOfStream());
    EXPECT_EQ(stream.Read(), 10);

    EXPECT_TRUE(stream.IsEndOfStream());
    EXPECT_EQ(stream.GetPosition(), 2U);
    EXPECT_THROW(stream.Read(), lab4::EndOfStreamException);

    stream.Close();
}

// Проверяет разрешение перехода в позицию конца конечного потока
TEST(StreamEdgeTests, FiniteLazyStreamAllowsSeekToEnd)
{
    int items[] = {5, 10};

    lab4::LazySequence<int> sequence(items, 2);
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_EQ(stream.Seek(2), 2U);
    EXPECT_TRUE(stream.IsEndOfStream());
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    stream.Close();
}

// Проверяет запрет перехода за конец конечного ленивого потока
TEST(StreamEdgeTests, FiniteLazyStreamRejectsSeekPastEnd)
{
    int items[] = {5, 10};

    lab4::LazySequence<int> sequence(items, 2);
    lab4::LazySequenceReadStream<int> stream(&sequence);

    stream.Open();

    EXPECT_THROW(stream.Seek(3), lab4::IndexOutOfRangeException);
    EXPECT_EQ(stream.GetPosition(), 0U);
    EXPECT_EQ(sequence.GetMaterializedCount(), 0U);

    stream.Close();
}