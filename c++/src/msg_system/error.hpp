#ifndef ERROR_H
#define ERROR_H

#include <cstdlib> // std::exit
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

// 0 -> ERROR | 1 -> WARNING | 2 -> DEBUG | 3 -> FILE_OUTPUT | 4 -> TRACE
// Se DEBUG_LEVEL não foi definido via compilador (-DDEBUG_LEVEL=...), defina aqui com valor padrão.
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#define SC SourceContext(__FILE__, __LINE__)
#define ERROR(msg) Error(msg, SC)
#define WARNING(msg) Warning(msg, SC)
#define DEBUG(msg) Debug(msg, SC)
#define TRACE(msg) Trace(msg, SC)
#define MESSAGE(msg) Message(msg)
#define FORMAT(...) std::string_view(Format(__VA_ARGS__))

// Holds context information for a warning or error while pre-processing scene file or executing the
// renderer. Use macro SC to create one
struct SourceContext
{
    const char *file;
    int line;
    SourceContext(const char *file = "n/a", int line = 0) : file(file), line(line)
    {
    }
};

template <typename... Args> inline std::string Format(Args &&...args)
{
    std::ostringstream oss;
    (oss << ... << args);
    return oss.str();
}

void Debug(std::string_view msg, const SourceContext &sc);
void Trace(std::string_view msg, const SourceContext &sc);

/// Prints out the error message and exits the program.
void Error(std::string_view, const SourceContext &);
/// Prints out the warning, but the program keeps running.
void Warning(std::string_view, const SourceContext &);
/// Prints out a simple message, program keeps running.
void Message(std::string_view);

#endif
//-------------------------------------------------------------------------------
