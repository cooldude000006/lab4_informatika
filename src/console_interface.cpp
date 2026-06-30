#include "console_interface.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../include/lazy/generators/arithmetic_progression_generator.h"
#include "exceptions.h"
#include "../include/lazy/lazy_sequence.h"
#include "../include/stream/lazy_sequence_read_stream.h"
#include "../include/sequence/mutable_array_sequence.h"
#include "../include/stream/sequence_read_stream.h"

namespace lab4
{
    std::string ConsoleInterface::ReadLine(const std::string& prompt) const
    {
        std::cout << prompt;

        std::string line;

        if (!std::getline(std::cin, line))
        {
            throw InvalidOperationException("Ввод данных был завершён");
        }

        return line;
    }

    void ConsoleInterface::WaitForEnter() const
    {
        ReadLine("\nНажмите Enter, чтобы вернуться в главное меню...");
    }

    int ConsoleInterface::ReadInteger(
        const std::string& prompt,
        int minimum,
        int maximum
    ) const
    {
        while (true)
        {
            std::string line = ReadLine(prompt);
            std::istringstream input(line);

            long long value = 0;
            char extra = '\0';

            if (
                input >> value &&
                !(input >> extra) &&
                value >= minimum &&
                value <= maximum
            )
            {
                return static_cast<int>(value);
            }

            std::cout
                << "Ошибка: введите целое число от minimum до maximum.\n";
        }
    }

    std::size_t ConsoleInterface::ReadSize(
        const std::string& prompt,
        std::size_t minimum,
        std::size_t maximum
    ) const
    {
        while (true)
        {
            std::string line = ReadLine(prompt);
            std::istringstream input(line);

            long long value = 0;
            char extra = '\0';

            if (
                input >> value &&
                !(input >> extra) &&
                value >= 0
            )
            {
                std::size_t converted = static_cast<std::size_t>(value);

                if (converted >= minimum && converted <= maximum)
                {
                    return converted;
                }
            }

            std::cout
                << "Ошибка: введите целое число от "
                << minimum
                << " до "
                << maximum
                << ".\n";
        }
    }

    double ConsoleInterface::ReadFiniteDouble(const std::string& prompt) const
    {
        while (true)
        {
            std::string line = ReadLine(prompt);
            std::istringstream input(line);

            double value = 0.0;
            char extra = '\0';

            if (
                input >> value &&
                !(input >> extra) &&
                std::isfinite(value)
            )
            {
                return value;
            }

            std::cout
                << "Ошибка: введите конечное вещественное число без дополнительных символов.\n";
        }
    }

    bool ConsoleInterface::ReadBoolean(const std::string& prompt) const
    {
        return ReadInteger(prompt, 0, 1) == 1;
    }

    void ConsoleInterface::PrintMainMenu() const
    {
        std::cout
            << "\n"
            << "========================================\n"
            << " ЛР-4. Онлайн-статистика потока данных\n"
            << "            Мусин Иван Б25-507        \n"
            << "========================================\n"
            << "1. Ручной ввод конечной последовательности\n"
            << "2. Автоматическая арифметическая прогрессия\n"
            << "0. Завершить программу\n";
    }

    void ConsoleInterface::PrintManualSource(const Sequence<double>& sequence) const
    {
        std::cout
            << "\nИсходная последовательность"
            << " (" << sequence.GetLength() << " элементов):\n";

        if (sequence.GetLength() == 0)
        {
            std::cout << "[пустая последовательность]\n";
            return;
        }

        for (
            int index = 0;
            index < sequence.GetLength();
            ++index
        )
        {
            std::cout << sequence.Get(index);

            if (index + 1 < sequence.GetLength())
            {
                std::cout << ' ';
            }
        }

        std::cout << '\n';
    }

    void ConsoleInterface::PrintAutomaticSource(
        double first,
        double step,
        std::size_t count
    ) const
    {
        std::cout
            << "\nПараметры автоматического источника:\n"
            << "Первый элемент: " << first << '\n'
            << "Шаг: " << step << '\n'
            << "Количество обрабатываемых элементов:" << count << '\n';

        std::size_t preview_count =
            count < 20 ? count : 20;

        std::cout << "Предпросмотр данных: ";

        double current = first;

        for (
            std::size_t index = 0;
            index < preview_count;
            ++index
        )
        {
            if (!std::isfinite(current))
            {
                std::cout << "[переполнение]";
                break;
            }

            std::cout << current;

            if (index + 1 < preview_count)
            {
                std::cout << ' ';
            }

            current += step;
        }

        if (count > preview_count)
        {
            std::cout << " ...";
        }

        std::cout << '\n';
    }

    void ConsoleInterface::PrintStatistics(
        const OnlineStatistics& statistics,
        const std::string& title
    ) const
    {
        std::cout
            << "\n--- "
            << title
            << " ---\n";

        std::cout
            << std::fixed
            << std::setprecision(6);

        std::cout
            << "Количество: "
            << statistics.GetCount()
            << '\n';

        std::cout
            << "Сумма: "
            << statistics.GetSum()
            << '\n';

        if (statistics.GetCount() == 0)
        {
            std::cout
                << "Минимум: не определён\n"
                << "Максимум: не определён\n"
                << "Среднее: не определено\n"
                << "Дисперсия: не определена\n"
                << "Стандартное отклонение: не определено\n";

            return;
        }

        std::cout
            << "Минимум: "
            << statistics.GetMinimum()
            << '\n';

        std::cout
            << "Максимум: "
            << statistics.GetMaximum()
            << '\n';

        std::cout
            << "Среднее: "
            << statistics.GetMean()
            << '\n';

        std::cout
            << "Дисперсия: "
            << statistics.GetVariance()
            << '\n';

        std::cout
            << "Стандартное отклонение: "
            << statistics.GetStandardDeviation()
            << '\n';
    }

    void ConsoleInterface::ProcessStream(
        ReadOnlyStream<double>& stream,
        std::size_t max_items,
        std::size_t batch_size,
        bool show_intermediate
    ) const
    {
        if (batch_size == 0)
        {
            throw InvalidOperationException(
                "Размер порции обработки должен быть больше нуля");
        }

        OnlineStatistics statistics;
        std::size_t total_processed = 0;

        stream.Open();

        try
        {
            while (
                total_processed < max_items &&
                !stream.IsEndOfStream()
            )
            {
                std::size_t remaining =
                    max_items - total_processed;

                std::size_t requested =
                    remaining < batch_size
                        ? remaining
                        : batch_size;

                std::size_t processed =
                    statistics.ProcessNext(
                        stream,
                        requested
                    );

                if (processed == 0)
                {
                    break;
                }

                total_processed += processed;

                if (show_intermediate)
                {
                    std::string title =
                        "Промежуточный результат после " +
                        std::to_string(total_processed) +
                        " элементов";

                    PrintStatistics(statistics, title);
                }
            }
            stream.Close();
        }
        catch (...)
        {
            if (stream.IsOpen())
            {
                try
                {
                    stream.Close();
                }
                catch (...)
                {
                }
            }
            throw;
        }

        std::cout
            << "\nФактически обработано элементов: "
            << total_processed
            << '\n';

        PrintStatistics(
            statistics,
            "Итоговая статистика"
        );
    }

    void ConsoleInterface::RunManualMode() const
    {
        std::cout
            << "\n=== Ручной режим ===\n";

        std::size_t count = ReadSize(
            "Введите количество элементов: ",
            0,
            MaxManualCount
        );

        MutableArraySequence<double> sequence;

        for (
            std::size_t index = 0;
            index < count;
            ++index
        )
        {
            std::string prompt =
                "Элемент [" + std::to_string(index) + "]: ";

            double value =
                ReadFiniteDouble(prompt);

            sequence.Append(value);
        }

        PrintManualSource(sequence);

        bool show_intermediate = ReadBoolean(
            "Показывать промежуточную статистику? "
            "1 — да, 0 — нет: "
        );

        std::size_t batch_size = 1;

        if (count > 0)
        {
            batch_size = ReadSize(
                "Введите размер порции обработки: ", 1, count
            );
        }

        SequenceReadStream<double> stream(&sequence);

        ProcessStream(
            stream,
            count,
            batch_size,
            show_intermediate
        );
    }

    void ConsoleInterface::RunAutomaticMode() const
    {
        std::cout
            << "\n=== Автоматический режим ===\n";

        double first = ReadFiniteDouble(
            "Введите первый элемент прогрессии: ");

        double step = ReadFiniteDouble(
            "Введите шаг прогрессии: ");

        std::size_t count = ReadSize(
            "Введите количество обрабатываемых элементов: ",
            1,
            MaxAutomaticCount
        );

        std::size_t batch_size = ReadSize(
            "Введите размер порции обработки: ",
            1,
            count
        );

        bool show_intermediate = ReadBoolean(
            "Показывать статистику после каждой порции? 1 — да, 0 — нет: ");

        PrintAutomaticSource(first, step, count);

        ArithmeticProgressionGenerator<double> generator(
            first,
            step
        );

        LazySequence<double> sequence(
            generator,
            Cardinal::Infinity()
        );

        LazySequenceReadStream<double> stream(&sequence);

        ProcessStream(
            stream,
            count,
            batch_size,
            show_intermediate
        );

        std::cout
            << "\nМатериализовано элементов ленивой последовательности: "
            << sequence.GetMaterializedCount()
            << '\n';
    }

    void ConsoleInterface::Run() const
    {
        while (true)
        {
            try
            {
                PrintMainMenu();

                int command = ReadInteger(
                    "Выберите команду: ", 0, 2);

                if (command == 0)
                {
                    std::cout
                        << "Работа программы завершена.\n";
                    return;
                }

                if (command == 1)
                {
                    RunManualMode();
                }
                else
                {
                    RunAutomaticMode();
                }
                WaitForEnter();
            }
            catch (const Exception& exception)
            {
                if (std::cin.eof())
                {
                    std::cout
                        << "\nВвод завершён. Программа остановлена.\n";
                    return;
                }

                std::cout
                    << "\nОшибка: "
                    << exception.what()
                    << '\n';
            }
            catch (const std::exception& exception)
            {
                std::cout
                    << "\nНеожиданная ошибка стандартной библиотеки: "
                    << exception.what()
                    << '\n';
            }
        }
    }
}