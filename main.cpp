#include <cstddef>
#include <iostream>

#include "write_only_stream.h"

class TestWriteStream :
    public lab4::WriteOnlyStream<int>
{
public:
    std::size_t Write(const int&) override
    {
        this->EnsureOpen("TestWriteStream::Write");

        std::size_t next_position =
            this->GetNextPosition(
                "TestWriteStream::Write"
            );

        this->SetPosition(next_position);

        return next_position;
    }
};

int main()
{
    TestWriteStream stream;

    std::cout
        << "Initially open: "
        << stream.IsOpen()
        << '\n';

    std::cout
        << "Initial position: "
        << stream.GetPosition()
        << '\n';

    try
    {
        stream.Write(10);
    }
    catch (
        const lab4::InvalidOperationException&
            exception
    )
    {
        std::cout
            << exception.what()
            << '\n';
    }

    stream.Open();

    std::cout
        << "After Open: "
        << stream.IsOpen()
        << '\n';

    std::cout
        << "Write position: "
        << stream.Write(10)
        << '\n';

    std::cout
        << "Write position: "
        << stream.Write(20)
        << '\n';

    std::cout
        << "Current position: "
        << stream.GetPosition()
        << '\n';

    try
    {
        stream.Open();
    }
    catch (
        const lab4::InvalidOperationException&
            exception
    )
    {
        std::cout
            << exception.what()
            << '\n';
    }

    stream.Close();

    std::cout
        << "After Close: "
        << stream.IsOpen()
        << '\n';

    std::cout
        << "Position after Close: "
        << stream.GetPosition()
        << '\n';

    try
    {
        stream.Close();
    }
    catch (
        const lab4::InvalidOperationException&
            exception
    )
    {
        std::cout
            << exception.what()
            << '\n';
    }

    stream.Open();

    std::cout
        << "Position after reopening: "
        << stream.GetPosition()
        << '\n';

    stream.Close();

    return 0;
}