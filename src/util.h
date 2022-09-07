#ifndef UTIL_H
#define UTIL_H
#define ANSI_COLOR_BOLD ANSI_ESCAPE "[1m"
#define ANSI_COLOR_GREEN ANSI_ESCAPE "[32m"
#define ANSI_COLOR_RED ANSI_ESCAPE "[31m"
#define ANSI_COLOR_RESET ANSI_ESCAPE "[0m"
#define ANSI_COLOR_WHITE ANSI_ESCAPE "[37m"
#define ANSI_ESCAPE "\033"
#define LENGTH(x) ((int)(sizeof(x) / sizeof(x)[0]))
#define MOD(a, b) ((a) - ((b)*floor((a) / (b))))
#endif
