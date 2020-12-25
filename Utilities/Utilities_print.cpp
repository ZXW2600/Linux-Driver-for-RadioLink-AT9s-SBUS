/*!
 * @file Utilities_print.cpp
 * @brief Common utilities for printing
 */

#include "Utilities_print.h"

 /*!
  * Printf, but with color.
  */
void printf_color(PrintColor color, const char* fmt, ...)
{
  auto color_id = (uint32_t)color;
  if (color_id)
    printf("\033[1;%dm", (uint32_t)color + 30);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\033[0m");
}

/*!
 * fprintf, but with color (used to print color to STDERR)
 */
void fprintf_color(PrintColor color, FILE* stream, const char* fmt, ...)
{
  auto color_id = (uint32_t)color;
  if (color_id)
    fprintf(stream, "\033[1;%dm", (uint32_t)color + 30);
  va_list args;
  va_start(args, fmt);
  vfprintf(stream, fmt, args);
  va_end(args);
  fprintf(stream, "\033[0m");
}

void printError(std::string message, std::string module_name)
{
  printf_color(PrintColor::Red, "[%s] : %s\n", module_name, message);
}
void printWarning(std::string message, std::string module_name)
{
  printf_color(PrintColor::Yellow, "[%s] : %s\n", module_name, message);
}
void printLog(std::string message, std::string module_name)
{
  printf_color(PrintColor::Default, "[%s] : %s\n", module_name, message);
}