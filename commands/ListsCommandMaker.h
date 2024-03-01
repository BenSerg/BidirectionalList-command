#ifndef S3_COMMANDMAKER_H
#define S3_COMMANDMAKER_H

#include <string>
#include <functional>
#include "data_structures/BidirectionalList.h"
#include "data_structures/TreeDictionary.h"

namespace bendryshev
{
  BidirectionalList< std::string > split(const std::string&);
  BidirectionalList< std::string > readListFromStream(std::istream&);

  class CommandMaker
  {
  public:
    using command = std::string;
    using value_t = int;
    using command_list_iterator = bendryshev::BidirectionalList< command >::Iterator;
    using commandsAction = std::function< void(command_list_iterator, command_list_iterator) >;
    using cmd_dict = TreeDictionary< command, commandsAction, std::less< > >;
    using list_dict = TreeDictionary< std::string, BidirectionalList< value_t >, std::less< > >;
    struct pos
    {
      value_t index_;
      BidirectionalList< value_t > list_;
      BidirectionalList< value_t >::Iterator iter_;
    };
    using pos_dict = TreeDictionary< std::string, pos, std::less< > >;
    using list = BidirectionalList< value_t >;
    explicit CommandMaker(std::ostream&);
    void readLists(std::istream&);
    void doCommand(BidirectionalList< command >&);
  private:
    std::ostream& out_;
    list_dict lists_;
    pos_dict positions_;
    cmd_dict command_dictionary_;
    void doPrintCommand(command_list_iterator, command_list_iterator);
    void doReplaceCommand(command_list_iterator, command_list_iterator);
    void doRemoveCommand(command_list_iterator, command_list_iterator);
    void doConcatCommand(command_list_iterator, command_list_iterator);
    void doEqualCommand(command_list_iterator, command_list_iterator);
    void doBeginCommand(command_list_iterator, command_list_iterator);
    void doEndCommand(command_list_iterator, command_list_iterator);
    void doMismatchCommand(command_list_iterator, command_list_iterator);
    void doPrintPosCommand(command_list_iterator, command_list_iterator);
    void doSwapCommand(command_list_iterator, command_list_iterator);
    void doFindCommand(command_list_iterator, command_list_iterator);
    void doRotateCommand(command_list_iterator, command_list_iterator);
    void doSearchCommand(command_list_iterator, command_list_iterator);
  };
}

#endif
