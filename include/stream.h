#pragma once

#include <cstddef>
#include <limits>
#include <string>

#include "exceptions.h"

namespace lab4
{
    template<class T>
    class Stream
    {
    private:
        bool is_open_;
        std::size_t position_;

    protected:
        Stream() : is_open_(false), position_(0)
        {
        }

        void EnsureOpen(const std::string& context) const
        {
            if (!is_open_)
            {
                throw InvalidOperationException(
                    "Поток не открыт, контекст=" + context);
            }
        }

        std::size_t GetNextPosition(
            const std::string& context
        ) const
        {
            if (
                position_ == std::numeric_limits<std::size_t>::max()
            )
            {
                throw InvalidOperationException(
                "Переполнение позиции потока, контекст=" + context);
            }
            return position_ + 1;
        }

        void SetPosition(std::size_t position)
        {
            position_ = position;
        }

        virtual void OnOpen()
        {
        }

        virtual void OnClose()
        {
        }

    public:
        Stream(const Stream<T>& other) = delete;

        Stream<T>& operator=(const Stream<T>& other) = delete;

        virtual ~Stream() = default;

        void Open()
        {
            if (is_open_)
            {
                throw InvalidOperationException("Поток уже открыт");
            }
            OnOpen();
            position_ = 0;
            is_open_ = true;
        }

        void Close()
        {
            EnsureOpen("Stream::Close");
            OnClose();
            is_open_ = false;
        }

        bool IsOpen() const
        {
            return is_open_;
        }

        std::size_t GetPosition() const
        {
            return position_;
        }
    };
}