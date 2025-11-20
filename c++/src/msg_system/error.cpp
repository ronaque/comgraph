#include "error.hpp"
#include "tcolor.hpp"

inline std::ostream &operator<<(std::ostream &os, const SourceContext &sc)
{
    os << sc.file << ":" << sc.line;
    return os;
}
static constexpr short line_length{80};

void Debug(std::string_view msg, const SourceContext &sc)
{
    if (DEBUG_LEVEL >= 2)
    {
        std::ostringstream oss;
        oss << std::setw(line_length) << std::setfill('=') << " " << '\n'
            << "[DEBUG]: " << msg << '\n'
            << "Reported at: <" << sc << "> \n"
            << std::setw(line_length) << std::setfill('=') << " " << '\n';

        std::cerr << TColor::colorize(oss.str(), TColor::CYAN);
    }
}

void Trace(std::string_view msg, const SourceContext &sc)
{
    if (DEBUG_LEVEL >= 3)
    {
        std::ostringstream oss;
        oss << std::setw(line_length) << std::setfill('-') << " " << '\n'
            << "[TRACE]: " << msg << '\n'
            << "Tracepoint: <" << sc << ">\n"
            << std::setw(line_length) << std::setfill('-') << " " << '\n';

        std::cerr << TColor::colorize(oss.str(), TColor::MAGENTA);
    }
}

/// Prints out the warning, but the program keeps running.
void Warning(std::string_view msg, const SourceContext &sc)
{
    if (DEBUG_LEVEL >= 1)
    {
        std::ostringstream oss;
        oss << std::setw(line_length) << std::setfill('=') << " " << '\n'
            << "[WARNING]: " << msg << '\n'
            << "Reported at: <" << sc << "> \n"
            << std::setw(line_length) << std::setfill('=') << " " << '\n';

        std::cerr << TColor::colorize(oss.str(), TColor::YELLOW, TColor::BOLD);
    }
}

/// Prints out the error message and exits the program.
void Error(std::string_view msg, const SourceContext &sc)
{
    if (DEBUG_LEVEL >= 0)
    {
        std::ostringstream oss;
        oss << std::setw(line_length) << std::setfill('=') << " " << '\n'
            << "[ERROR]: " << msg << '\n'
            << "Reported at: < " << sc << " > " << '\n'
            << "exiting...\n"
            << std::setw(line_length) << std::setfill('=') << " " << '\n';

        std::cerr << TColor::colorize(oss.str(), TColor::RED, TColor::BOLD);
        std::exit(EXIT_FAILURE);
    }
}

void Message(std::string_view str)
{
    std::cout << str << '\n';
}
