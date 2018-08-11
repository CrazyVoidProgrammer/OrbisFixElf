#include <stdio.h>
#include <stdarg.h>

#include <stdint.h>

#include "logger.h"


void debugWrite(enum LogLevels level, char *fmt, ...)
{
	char buffer[MAX_LOGSIZE];
	char finalBuffer[MAX_LOGSIZE];
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	const char *levelString = "None";
	const char *levelColor = COLOR_NORMAL;
	
	switch(level)
	{
		case LL_Info:
			levelString = "Info";
			levelColor = COLOR_GREEN;
		break;
		case LL_Warn:
			levelString = "Warn";
			levelColor = COLOR_YELLOW;

		break;
		case LL_Error:
			levelString = "Error";
			levelColor = COLOR_RED;

		break;
		case LL_Debug:
			levelString = "Debug";
			levelColor = KMAG;

		break;
		case LL_None:
		default:
			levelString = "None";
			levelColor = COLOR_NORMAL;
		break;
	}
	
	snprintf(finalBuffer, sizeof(finalBuffer), "%s[%s] %s %s\n", levelColor, levelString, buffer, COLOR_NORMAL);
	printf("%s", finalBuffer);
}

void errorWrite(char *fmt, ...)
{
	char buffer[MAX_LOGSIZE];
	char finalBuffer[MAX_LOGSIZE];
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	const char *levelString = "Error";
	const char *levelColor = COLOR_RED;
	
	snprintf(finalBuffer, sizeof(finalBuffer), "%s[%s] %s %s\n", levelColor, levelString, buffer, COLOR_NORMAL);
	printf("%s", finalBuffer);
}

void infoWrite(char *fmt, ...)
{
	char buffer[MAX_LOGSIZE];
	char finalBuffer[MAX_LOGSIZE];
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	const char *levelString = "Info";
	const char *levelColor = COLOR_GREEN;
	
	snprintf(finalBuffer, sizeof(finalBuffer), "%s[%s] %s %s\n", levelColor, levelString, buffer, COLOR_NORMAL);
	printf("%s", finalBuffer);
}

void warningWrite(char *fmt, ...)
{
	char buffer[MAX_LOGSIZE];
	char finalBuffer[MAX_LOGSIZE];
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	const char *levelString = "Warn";
	const char *levelColor = COLOR_YELLOW;
	
	snprintf(finalBuffer, sizeof(finalBuffer), "%s[%s] %s %s\n", levelColor, levelString, buffer, COLOR_NORMAL);
	printf("%s", finalBuffer);
}

void functionLog(enum LogLevels level, char *function, int line, char *fmt, ...)
{
	char buffer[MAX_LOGSIZE];
	char finalBuffer[MAX_LOGSIZE];
	
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	
	const char *levelString = "None";
	const char *levelColor = COLOR_NORMAL;
	
	switch(level)
	{
		case LL_Info:
			levelString = "Info";
			levelColor = COLOR_GREEN;
		break;
		case LL_Warn:
			levelString = "Warn";
			levelColor = COLOR_YELLOW;

		break;
		case LL_Error:
			levelString = "Error";
			levelColor = COLOR_RED;

		break;
		case LL_Debug:
			levelString = "Debug";
			levelColor = KMAG;

		break;
		case LL_None:
		default:
			levelString = "None";
			levelColor = COLOR_NORMAL;
		break;
	}
	
	snprintf(finalBuffer, sizeof(finalBuffer), "%s[%s] %s:%d : %s %s\n", levelColor, levelString, function, line, buffer, COLOR_NORMAL);
	printf("%s", finalBuffer);
}