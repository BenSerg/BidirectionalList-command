#ifndef S3_PRINT_COMMAND_MESSAGES_H
#define S3_PRINT_COMMAND_MESSAGES_H

#include <iosfwd>

namespace bendryshev
{
  void printEmptyCommandMessage(std::ostream&);
  void printInvalidCommandMessage(std::ostream&);
  void printTrueMessage(std::ostream&);
  void printFalseMessage(std::ostream&);
}
#endif

