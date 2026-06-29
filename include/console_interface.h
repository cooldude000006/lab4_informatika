#pragma once

#include <cstddef>
#include <string>

#include "online_statistics.h"
#include "read_only_stream.h"
#include "sequence.h"

namespace lab4
{
    class ConsoleInterface
    {
    private:
        static constexpr std::size_t MaxManualCount = 100000;
        static constexpr std::size_t MaxAutomaticCount = 1000000;

        std::string ReadLine(const std::string& prompt) const;

        int ReadInteger(
            const std::string& prompt,
            int minimum,
            int maximum
        ) const;

        std::size_t ReadSize(
            const std::string& prompt,
            std::size_t minimum,
            std::size_t maximum
        ) const;

        double ReadFiniteDouble(const std::string& prompt) const;

        bool ReadBoolean(const std::string& prompt) const;

        void PrintMainMenu() const;

        void PrintManualSource(const Sequence<double>& sequence) const;

        void PrintAutomaticSource(
            double first,
            double step,
            std::size_t count
        ) const;

        void PrintStatistics(
            const OnlineStatistics& statistics,
            const std::string& title
        ) const;

        void ProcessStream(
            ReadOnlyStream<double>& stream,
            std::size_t max_items,
            std::size_t batch_size,
            bool show_intermediate
        ) const;

        void RunManualMode() const;

        void RunAutomaticMode() const;

        void WaitForEnter() const;
    public:
        void Run() const;
    };
}