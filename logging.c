#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logging.h"

char * levels[] = {"ERROR", "WARNING", "INFO", "DEBUG"};

void log_message(int level, const char * format, ...) {
    char buffer[150];
    va_list arg;
    va_start(arg, format);
    vsnprintf(buffer, 149, format, arg);
    va_end(arg);

    printf("%s: %s \n", levels[level], buffer);
    fflush(stdout);
}
