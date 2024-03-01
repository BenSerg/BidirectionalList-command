#include "commands/ListsCommandMaker.h"
#include <iostream>
#include <algorithm>
#include "printCommandMessages.h"

bendryshev::BidirectionalList< std::string > bendryshev::readListFromStream(std::istream& in)
{
  bendryshev::BidirectionalList< std::string > input_data;
  std::string temp;
  while (std::getline(in, temp))
  {
    if (temp.empty())
    {
      continue;
    }
    else
    {
      input_data.pushBack(temp);
    }
  }
  return input_data;
}

bendryshev::BidirectionalList< std::string > bendryshev::split(const std::string& data)
{
  bendryshev::BidirectionalList< std::string > string_parts;
  size_t last_space = 0;
  while (last_space < data.length())
  {
    size_t next_space = data.find_first_of(' ', last_space + 1);
    next_space = (next_space == std::string::npos) ? data.size() : next_space;
    std::string word = data.substr(last_space, next_space - last_space);

    string_parts.pushBack(word);
    last_space = next_space + 1;
  }
  return string_parts;
}

namespace
{
  using cmd_it = bendryshev::CommandMaker::command_list_iterator;
  using pos = bendryshev::CommandMaker::pos;

  bool isEqualRange(const pos& pos1, const pos& pos2, const pos& pos3, const pos& pos4)
  {
    return (pos2.index_ - pos1.index_ == pos4.index_ - pos3.index_);
  }

  void checkEndOfCommand(cmd_it begin, cmd_it end)
  {
    if (begin == end)
    {
      throw std::logic_error("end of command");
    }
  }

  void setPosName(cmd_it& begin, std::string& pos_name)
  {
    pos_name = *(begin++);
  }

  void checkListPresence(bendryshev::CommandMaker::list_dict& lists_dict, const std::string& name)
  {
    if (lists_dict.find(name) == lists_dict.end())
    {
      throw std::logic_error("no list in dict");
    }
  }

  void checkPosPresence(bendryshev::CommandMaker::pos_dict& pos_dict, const std::string& name)
  {
    if (pos_dict.find(name) == pos_dict.end())
    {
      throw std::logic_error("no pos in dict");
    }
  }

  void checkExistedPos(bendryshev::CommandMaker::pos_dict& pos_dict, cmd_it& begin, std::string& pos_name)
  {
    setPosName(begin, pos_name);
    checkPosPresence(pos_dict, pos_name);
  }

  using value_t = bendryshev::CommandMaker::value_t;

  bool isNumber(const std::string& str)
  {
    return std::all_of(str.begin(), str.end(), isdigit);
  }

  void replace(bendryshev::CommandMaker::list& list, value_t value1, value_t value2)
  {
    for (auto&& item: list)
    {
      if (item == value1)
      {
        item = value2;
      }
    }
  }

  void replace(bendryshev::CommandMaker::list& dest_list, value_t value, bendryshev::CommandMaker::list& arg_list)
  {
    while (dest_list.find(value) != dest_list.end())
    {
      auto it = dest_list.find(value);
      for (auto&& item: arg_list)
      {
        dest_list.emplace(it, item);
      }
      dest_list.erase(it);
    }
  }

  void remove(bendryshev::CommandMaker::list& list, value_t value)
  {
    auto it = list.begin();
    while (it != list.end())
    {
      if (*it == value)
      {
        list.erase(it++);
      }
      else
      {
        it++;
      }
    }
  }

  void remove(bendryshev::CommandMaker::list& list, bendryshev::CommandMaker::list& value_list)
  {
    for (auto&& value: value_list)
    {
      while (list.find(value) != list.end())
      {
        auto it = list.find(value);
        list.erase(it);
      }
    }
  }

  void pushBack(bendryshev::CommandMaker::list& list1, bendryshev::CommandMaker::list& list2)
  {
    for (auto&& item: list2)
    {
      list1.pushBack(item);
    }
  }
}

bendryshev::CommandMaker::CommandMaker(std::ostream& out):
  out_(out),
  lists_(),
  command_dictionary_(
   {
     { "print",    std::bind(&CommandMaker::doPrintCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "replace",  std::bind(&CommandMaker::doReplaceCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "remove",   std::bind(&CommandMaker::doRemoveCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "concat",   std::bind(&CommandMaker::doConcatCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "equal",    std::bind(&CommandMaker::doEqualCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "begin",    std::bind(&CommandMaker::doBeginCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "end",      std::bind(&CommandMaker::doEndCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "mismatch", std::bind(&CommandMaker::doMismatchCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "printPos", std::bind(&CommandMaker::doPrintPosCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "swap",     std::bind(&CommandMaker::doSwapCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "find",     std::bind(&CommandMaker::doFindCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "rotate",   std::bind(&CommandMaker::doRotateCommand, this, std::placeholders::_1, std::placeholders::_2) },
     { "search",   std::bind(&CommandMaker::doSearchCommand, this, std::placeholders::_1, std::placeholders::_2) }
   })
{}

void bendryshev::CommandMaker::readLists(std::istream& in)
{
  bendryshev::BidirectionalList< std::string > input_data = readListFromStream(in);
  for (auto&& item: input_data)
  {
    bendryshev::BidirectionalList< std::string > string_parts = split(item);
    if (!string_parts.isEmpty())
    {
      auto begin = string_parts.begin();
      auto end = string_parts.end();
      std::string new_list_name = *(begin++);
      list new_list;
      while (begin != end)
      {
        bendryshev::CommandMaker::value_t value = std::stoi(*(begin++));
        new_list.pushBack(value);
      }
      lists_.push(new_list_name, new_list);
    }
  }
}

void bendryshev::CommandMaker::doCommand(bendryshev::BidirectionalList< bendryshev::CommandMaker::command >& commandList)
{
  assert(!commandList.isEmpty());
  try
  {
    bendryshev::CommandMaker::command_list_iterator commandList_Iterator_begin = commandList.begin();
    bendryshev::CommandMaker::command_list_iterator commandList_Iterator_end = commandList.end();
    bendryshev::CommandMaker::command target = *commandList_Iterator_begin;
    if (command_dictionary_.find(target) != command_dictionary_.end())
    {
      command_dictionary_.get(target)(++commandList_Iterator_begin, commandList_Iterator_end);
    }
    else
    {
      bendryshev::printInvalidCommandMessage(out_);
    }
  }
  catch (const std::logic_error&)
  {
    bendryshev::printInvalidCommandMessage(out_);
  }
}

void bendryshev::CommandMaker::doPrintCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkListPresence(lists_, list_name);
  if (begin == end)
  {
    list& new_list = lists_.get(list_name);
    if (new_list.isEmpty())
    {
      bendryshev::printEmptyCommandMessage(out_);
    }
    else
    {
      out_ << list_name;
      for (auto&& item: new_list)
      {
        out_ << " " << item;
      }
      out_ << "\n";
    }
  }
  else
  {
    throw std::logic_error("");
  }
}


void bendryshev::CommandMaker::doReplaceCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkListPresence(lists_, list_name);
  list& dest_list = lists_.get(list_name);
  checkEndOfCommand(begin, end);
  value_t first_value = std::stoi(*(begin++));
  checkEndOfCommand(begin, end);
  std::string next_part = *(begin++);
  if (isNumber(next_part))
  {
    value_t next_value = std::stoi(next_part);
    replace(dest_list, first_value, next_value);
  }
  else
  {
    checkListPresence(lists_, next_part);
    list& second_list = lists_.get(next_part);
    replace(dest_list, first_value, second_list);
  }
}

void bendryshev::CommandMaker::doRemoveCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkEndOfCommand(begin, end);
  checkListPresence(lists_, list_name);
  list& dest_list = lists_.get(list_name);
  std::string next_part = *(begin++);
  if (isNumber(next_part))
  {
    value_t value_to_delete = std::stoi(next_part);
    remove(dest_list, value_to_delete);
  }
  else
  {
    checkListPresence(lists_, next_part);
    list& new_list = lists_.get(next_part);
    remove(dest_list, new_list);
  }
}

void bendryshev::CommandMaker::doConcatCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  list concat_list;
  checkEndOfCommand(begin, end);
  std::string dest_list_name = *(begin++);
  checkListPresence(lists_, dest_list_name);
  list& dest_list = lists_.get(dest_list_name);
  pushBack(concat_list, dest_list);
  checkEndOfCommand(begin, end);
  while (begin != end)
  {
    std::string new_list_name = *(begin++);
    checkListPresence(lists_, new_list_name);
    list& new_list = lists_.get(new_list_name);
    pushBack(concat_list, new_list);
  }
  lists_.drop(list_name);
  lists_.push(list_name, concat_list);
}

void bendryshev::CommandMaker::doEqualCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string first_list_name = *(begin++);
  checkListPresence(lists_, first_list_name);
  list& first_list = lists_.get(first_list_name);
  checkEndOfCommand(begin, end);
  std::string second_list_name = *(begin++);
  checkListPresence(lists_, second_list_name);
  list& second_list = lists_.get(second_list_name);
  bool is_equal = first_list == second_list;
  while (begin != end && is_equal)
  {
    std::string new_list_name = *(begin++);
    checkListPresence(lists_, new_list_name);
    is_equal = (first_list == lists_.get(new_list_name));
  }
  if (is_equal)
  {
    bendryshev::printTrueMessage(out_);
  }
  else
  {
    bendryshev::printFalseMessage(out_);
  }
}

void bendryshev::CommandMaker::doBeginCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  setPosName(begin, pos_name);
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkListPresence(lists_, list_name);
  list& list = lists_.get(list_name);
  pos new_pos = { 0, list, list.begin() };
  positions_.drop(pos_name);
  positions_.push(pos_name, new_pos);
}

void bendryshev::CommandMaker::doEndCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  setPosName(begin, pos_name);
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkListPresence(lists_, list_name);
  list& list = lists_.get(list_name);
  pos new_pos { static_cast< value_t >(list.getSize()), list, list.end() };
  positions_.drop(pos_name);
  positions_.push(pos_name, new_pos);
}

void bendryshev::CommandMaker::doMismatchCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  setPosName(begin, pos_name);
  checkEndOfCommand(begin, end);
  std::string list1_name = *(begin++);
  checkEndOfCommand(begin, end);
  checkListPresence(lists_, list1_name);
  list& list1 = lists_.get(list1_name);
  std::string list2_name = *(begin++);
  checkListPresence(lists_, list2_name);
  list& list2 = lists_.get(list2_name);
  value_t count = 0;
  auto it1 = list1.begin();
  auto it2 = list2.begin();
  while (it1 != list1.end() && it2 != list2.end())
  {
    if (*it1 != *it2)
    {
      break;
    }
    ++count;
    ++it1;
    ++it2;
  }
  pos new_pos = { count, list1, it1 };
  positions_.drop(pos_name);
  positions_.push(pos_name, new_pos);
}

void bendryshev::CommandMaker::doPrintPosCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  checkExistedPos(positions_, begin, pos_name);
  if (begin == end)
  {
    pos new_pos = positions_.get(pos_name);
    out_ << pos_name << " ";
    out_ << new_pos.index_ << "\n";
  }
  else
  {
    throw std::logic_error("");
  }
}

void bendryshev::CommandMaker::doSwapCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos1_name;
  checkExistedPos(positions_, begin, pos1_name);
  auto& pos1 = positions_.get(pos1_name);
  checkEndOfCommand(begin, end);
  std::string pos2_name;
  checkExistedPos(positions_, begin, pos2_name);
  auto& pos2 = positions_.get(pos2_name);
  std::swap(pos1, pos2);
}

void bendryshev::CommandMaker::doFindCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  setPosName(begin, pos_name);
  checkEndOfCommand(begin, end);
  std::string list_name = *(begin++);
  checkListPresence(lists_, list_name);
  list list = lists_.get(list_name);
  checkEndOfCommand(begin, end);
  value_t elem = std::stoi(*(begin++));
  value_t count = 0;
  auto it = list.begin();
  while (it != list.find(elem))
  {
    ++count;
    ++it;
  }
  positions_.drop(pos_name);
  positions_.push(pos_name, { count, list, it });
}

void bendryshev::CommandMaker::doRotateCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos1_name;
  checkExistedPos(positions_, begin, pos1_name);
  checkEndOfCommand(begin,end);
  std::string pos2_name;
  checkExistedPos(positions_, begin, pos2_name);
  checkEndOfCommand(begin,end);
  std::string pos3_name;
  checkExistedPos(positions_, begin, pos3_name);
  checkEndOfCommand(begin,end);
  std::string pos4_name;
  checkExistedPos(positions_, begin, pos4_name);
  auto pos1 = positions_.get(pos1_name);
  auto pos2 = positions_.get(pos2_name);
  auto pos3 = positions_.get(pos3_name);
  auto pos4 = positions_.get(pos4_name);
  if (!isEqualRange(pos1, pos2, pos3, pos4))
  {
    throw std::logic_error("");
  }
  auto iter1 = pos1.iter_;
  auto iter2 = pos3.iter_;
  while (iter1 != pos2.iter_)
  {
    std::iter_swap(iter1, iter2);
    ++iter1;
    ++iter2;
  }
}

void bendryshev::CommandMaker::doSearchCommand(command_list_iterator begin, command_list_iterator end)
{
  checkEndOfCommand(begin, end);
  std::string pos_name;
  setPosName(begin, pos_name);
  checkEndOfCommand(begin, end);
  std::string pos1_name;
  checkExistedPos(positions_, begin, pos1_name);
  checkEndOfCommand(begin, end);
  std::string pos2_name;
  checkExistedPos(positions_, begin, pos2_name);
  auto pos1 = positions_.get(pos1_name);
  auto pos2 = positions_.get(pos2_name);
  if (pos1.list_ != pos2.list_)
  {
    throw std::logic_error("");
  }
  value_t elem = std::stoi(*(begin++));
  value_t count = pos1.index_;
  auto it = pos1.iter_;
  while (it != pos2.iter_)
  {
    if (*it == elem)
    {
      break;
    }
    ++count;
    ++it;
  }
  positions_.drop(pos_name);
  positions_.push(pos_name, { count, pos1.list_, it });
}

