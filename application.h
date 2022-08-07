#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>

#include "handler.hpp"
#include "variables.hpp"
#include "parser.h"

#define LOG(x) std::cout << "[DEBUG] " << x << std::endl

class Application
{
  private:
    void write(const char* text);
  public:  
    static Application &Get()
    {
      static Application app;
      return app;
    }

    int run(std::string code);
};

#endif // !APPLICATION_H
