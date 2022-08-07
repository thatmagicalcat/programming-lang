#pragma once
#ifndef STRINGTOOLS_H
#define STRINGTOOLS_H

#include <string>
#include <cstring>
#include <iostream>

namespace StringTools {
  bool startswith(const std::string&, const char*);
  bool endswith(const std::string&, const char*);
  void replace(std::string&, const char*, const char*, bool=false);

  template <class ... T>
  std::string concat(T ... args);
}

#endif
