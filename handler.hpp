#pragma once
#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "commands.h"

typedef std::vector<std::string> str_vec;
#define find_str(x) (str.find(x) <= str.size())

class Handle
{
public:
  std::unordered_map<std::string, void(*)(str_vec&, int&)> *commands = &cmds::var::commands;

  static Handle &Get()
  {
    static Handle hnd;
    return hnd;
  }
};

#endif // !HANDLER_HPP
