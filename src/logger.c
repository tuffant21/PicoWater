#include "logger.h"

const char *cyanColor = "\033[1;36m";
const char *greenColor = "\033[1;32m";
const char *yellowColor = "\033[1;33m";
const char *redColor = "\033[1;31m";
const char *resetColor = "\033[0m";

void echo(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);
}

void log_debug(const char *format, ...) {
#if DEBUG
    va_list args;
    va_start(args, format);

    printf("\n[%sdebug%s]: ", cyanColor, resetColor);
    vprintf(format, args);

    va_end(args);
#endif
}

void log_info(const char *format, ...) {
#if INFO
    va_list args;
    va_start(args, format);

    printf("\n[%sinfo%s]: ", greenColor, resetColor);
    vprintf(format, args);

    va_end(args);
#endif
}

void log_warn(const char *format, ...) {
#if WARN
    va_list args;
    va_start(args, format);

    printf("\n[%swarn%s]: ", yellowColor, resetColor);
    vprintf(format, args);

    va_end(args);
#endif
}

void log_error(const char *format, ...) {
#if ERROR
    va_list args;
    va_start(args, format);

    printf("\n[%serror%s]: ", redColor, resetColor);
    vprintf(format, args);

    va_end(args);
#endif
}