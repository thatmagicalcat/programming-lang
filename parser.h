#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>

namespace parser
{
  void parse(std::string p_code, std::vector<std::string> *m_args);
}

#endif // !PARSER_H
