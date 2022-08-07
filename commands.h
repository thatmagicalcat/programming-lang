#pragma once
#ifndef __HANDLER_H
#define __HANDLER_H

#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <ostream>
#include <array>

#include "variables.hpp"
#include "parser.h"
#include "stringTools.h"

typedef std::vector<std::string> str_vec;
typedef std::unordered_map<std::string, std::string> umap;

#define find_str(x) (str.find(x) <= str.size())

namespace cmds
{
  namespace stringTools {
    void replace(std::string &str, char v1, char v2);
  }

  namespace in
  {
    bool is_block(const std::string &str);
    bool is_string(const std::string &str);
    std::string get_var(const char* name);
    void write(const char* text);
  }

  void capture_func(const char* func_name);
  void include(const std::string &name);
  void output(str_vec &p_args, int &idx);
  void output_ln(str_vec &p_args, int &idx);
  void file_import(str_vec &p_args, int &idx);
  void output_f(str_vec &p_args, int &idx);
  void for_loop(str_vec &p_args, int &idx);
  void assign(str_vec &p_args, int &idx);
  void raw_cpp(str_vec &p_args, int &idx);
  void function(str_vec &p_args, int &idx);
  void include(str_vec &p_args, int &idx);
  void loop(str_vec &p_args, int &idx);
  void end(str_vec &p_args, int &idx);
  void label(str_vec &p_args, int &idx);
  void goto_label(str_vec &p_args, int &idx);
  void input(str_vec &p_args, int &idx);
  void repeat(str_vec &p_args, int &idx);
  void raw_input(str_vec &p_args, int &idx);
  void at_list(str_vec &p_args, int &idx);
  void if_block(str_vec &p_args, int &idx);
  void else_if_block(str_vec &p_args, int &idx);
  void else_block(str_vec &p_args, int &idx);

  // Function that should not be used in `handler.hpp`
  void handle_raw_variables(str_vec &p_args, int &idx);

  class var {
  public:
    static std::unordered_map<std::string, void(*)(str_vec&, int&)> commands;
  };
}

#endif
