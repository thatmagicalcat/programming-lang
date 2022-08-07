#include <iostream>
#include <vector>
#include <fstream>

#include "application.h"

int main(int argc, char* argv[])
{
  std::string file_name = argv[1];

  std::ifstream ifs(file_name);
  std::string code((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

  if (argc != 2) return -1;
  int status = Application::Get().run(code);

  // system("g++ -o output output.cc --std=c++17 && rm output.cc");

  // Compile the code and remove `output.cc`
#if DEBUG == 1
  system("g++ -o output output.cc --std=c++17");
#else
  system("g++ -O3 -o output output.cc --std=c++17 && del output.cc"); // Use `rm` instead of `del` in linux
#endif

  return status;
}
