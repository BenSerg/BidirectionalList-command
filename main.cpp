#include <iostream>
#include <fstream>
#include "data_structures/BidirectionalList.h"
#include "commands/ListsCommandMaker.h"

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Wrong command line params\n";
    return 1;
  }
  std::ifstream fin(argv[1]);
  if (!fin)
  {
    std::cerr << "File can not be opened\n";
    return 1;
  }
  bendryshev::CommandMaker cmd(std::cout);
  cmd.readLists(fin);
  fin.close();
  std::string s;
  while (std::getline(std::cin, s))
  {
    if (s.empty())
    {
      continue;
    }
    bendryshev::BidirectionalList< std::string > command = bendryshev::split(s);
    cmd.doCommand(command);
  }
  return 0;
}
