#pragma once
#include <exception>
#include <string>
#include <cstddef>

namespace lab4
{
    class Exception : public std::exception
    {
    protected:
        std::string message_;
    public:
        explicit Exception(const std::string& message) : message_(message) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }
        virtual ~Exception() = default;
    };

	class IndexOutOfRangeException : public Exception
	{
	public:
		IndexOutOfRangeException(
			int index,
			int size,
			const std::string& context = ""
		)
			: Exception(
				"Выход за границы: индекс=" +
				std::to_string(index) +
				", размер=" +
				std::to_string(size) +
				(context.empty() ? "" : ", контекст=" + context)
			)
		{
		}
		IndexOutOfRangeException(
			std::size_t index,
			std::size_t size,
			const std::string& context = ""
		)
			: Exception(
				"Выход за границы: индекс=" +
				std::to_string(index) +
				", размер=" +
				std::to_string(size) +
				(context.empty() ? "" : ", контекст=" + context)
			)
		{
		}
	};

	class InvalidOperationException : public Exception
	{
	public:
		explicit InvalidOperationException(const std::string& message)
			: Exception("Недопустимая операция: " + message)
		{
		}
	};

	class EndOfStreamException : public Exception
	{
	public:
		explicit EndOfStreamException(const std::string& context = "")
			: Exception("Достигнут конец потока" +
				  (context.empty() ? "" : ", контекст=" + context))
		{
		}
	};
}