#pragma once

#include <stdio.h>	// vsnprintf
#include <stdarg.h>	// va_list

#include "constants.h"

/*
 * The following are the logging levels we have.
 *
 * Depending on the value of the 'LOGGING_LEVEL', all the messages 
 * that are at level < LOGGING_LEVEL will be ignored and will not
 * be printed on screen.
 */ 
enum
{
  // Used to print DEBUG information.
  ORCH_DEBUG = 1,
  
  // Used to print DEBUG information, with a priority that is higher than standard DEBUG messages.
  ORCH_DEBUG_INFO,

  // Used to print WARNING information, which may suggest that something is wrong.
  ORCH_WARNING,
  
  // Used to print ERROR information. This level should always be turned on.
  ORCH_ERROR,

  // Used to print general INFO that should always be shown on screen.
  ORCH_INFO
};


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Formats a message string and prints it on screen.

	This function is basically a printf() enriched with some parameters
	needed to log messages a better way.
	This functions prints everything on a standard output file, on a single line.

	\param LoggingLevel Level of this logging message. It may not be printed on screen depending on the current logging threshold.
	\param ModuleName Name of the module that is generating this message. It would be the first text printed on each line.
	\param File Name of the file in which this function has been invoked.
	\param Line Line in which this function has been invoked.
	\param Format Format-control string, according to syntax of the printf() function.
*/
extern void logger(int LoggingLevel, const char *ModuleName, const char *File, int Line, const char *Format, ...);


#ifdef __cplusplus
}
#endif

