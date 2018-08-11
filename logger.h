#include <stdio.h>
#include <stdarg.h>

#define MAX_LOGSIZE 500

enum LogLevels
{
	LL_None,
	LL_Info,
	LL_Warn,
	LL_Error,
	LL_Debug,
	LL_All
};

// Log Colors
#define COLOR_NORMAL  "\x1B[0m"
#define COLOR_RED  "\x1B[31m"
#define COLOR_GREEN  "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define COLOR_CYAN  "\x1B[36m"
#define COLOR_WHITE  "\x1B[37m"


void debugWrite(enum LogLevels level, char *fmt, ...);

void errorWrite(char *fmt, ...);

void infoWrite(char *fmt, ...);

void warningWrite(char *fmt, ...);

void functionLog(enum LogLevels level, char *function, int line, char *fmt, ...);