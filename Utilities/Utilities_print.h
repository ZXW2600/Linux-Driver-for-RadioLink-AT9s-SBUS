/*!
 * @file Utilities_print.h
 * @brief Common utilities for printing
 */

#ifndef PRINT_OUT_H
#define PRINT_OUT_H

#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

/*!
 * Floating point value to std::string.
 */
template <typename T>
std::string prettyString(T vv)
{
  static int const buflen(32);
  static char buf[buflen];
  // memset(buf, 0, sizeof(buf));
  snprintf(buf, buflen - 1, "% 6.6f  ", vv);
  std::string str(buf);
  return str;
}

// template <typename T>
// void pretty_print(DMat<T> const &mm, std::ostream &os, std::std::string const &title,
//                   std::std::string const &prefix = "", bool vecmode = false,
//                   bool nonl = false)
// {
//   char const *nlornot("\n");
//   if (nonl)
//   {
//     nlornot = "";
//   }
//   if (!title.empty())
//   {
//     os << title << nlornot;
//   }
//   if ((mm.rows() <= 0) || (mm.cols() <= 0))
//   {
//     os << prefix << " (empty)" << nlornot;
//   }
//   else
//   {
//     if (vecmode)
//     {
//       if (!prefix.empty())
//         os << prefix;
//       for (int ir(0); ir < mm.rows(); ++ir)
//       {
//         os << pretty_std::string(mm.coeff(ir, 0));
//       }
//       os << nlornot;
//     }
//     else
//     {
//       for (int ir(0); ir < mm.rows(); ++ir)
//       {
//         if (!prefix.empty())
//           os << prefix;
//         for (int ic(0); ic < mm.cols(); ++ic)
//         {
//           os << pretty_std::string(mm.coeff(ir, ic));
//         }
//         os << nlornot;
//       }
//     }
//   }
// }

template <typename T>
void pretty_print(const std::vector<T> &_vec, const char *title)
{
  printf("%s: ", title);
  for (size_t i(0); i < _vec.size(); ++i)
  {
    printf("% 6.4f, \t", _vec[i]);
  }
  printf("\n");
}

template <typename T>
void pretty_print(const T *_vec, const char *title, size_t size)
{
  printf("%s: ", title);
  for (size_t i(0); i < size; ++i)
  {
    printf("% 6.4f, \t", _vec[i]);
  }
  printf("\n");
}

enum class PrintColor
{
  Default,
  Red,
  Green,
  Yellow,
  Blue,
  Magenta,
  Cyan
};

void printf_color(enum PrintColor color, const char *fmt, ...);
void fprintf_color(enum PrintColor color, FILE *stream, const char *fmt, ...);

void printError(std::string message, std::string module_name);
void printWarning(std::string message, std::string module_name);
void printLog(std::string message, std::string module_name);
#endif
