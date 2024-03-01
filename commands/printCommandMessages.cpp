#include "printCommandMessages.h"
#include <iostream>

void bendryshev::printInvalidCommandMessage(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
}

void bendryshev::printEmptyCommandMessage(std::ostream& out)
{
  out << "<EMPTY>\n";
}

void bendryshev::printTrueMessage(std::ostream& out)
{
  out << "<TRUE>\n";
}

void bendryshev::printFalseMessage(std::ostream& out)
{
  out << "<FALSE>\n";
}

