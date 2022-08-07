#include "application.h"

std::ostream& operator<<(std::ostream &os, const std::vector<std::string> &vec) {
  for (const std::string &s : vec)
    os << s << std::endl; 

  return os;
}

int Application::run(std::string code)
{
  std::vector<std::string> vec;
  parser::parse(code, &vec);

  for (unsigned c = 0; c < vec.size(); c++)
    if (vec[c] == "fn")
      cmds::capture_func(vec[c + 1].c_str());

  auto &hnd = Handle::Get();

#if LINUX == 1
  system("echo > output.cc");
#else
  system("copy /Y nul output.cc");
#endif

  write("#include<iostream>");

  int _skip_idx = 0;
  for (int idx = 0; idx < vec.size(); idx++)
  {
    const std::string &s = vec[idx];
    printf("[]: '%s'\n", s.c_str());

    if (idx < _skip_idx) { ++idx; continue; }
    if (s == "-")
      _skip_idx = idx + 2;

    if (hnd.commands->find(s) != hnd.commands->end())
      hnd.commands->at(s)(vec, idx); /* LOG("a function is called upon " << s); */
    else
      cmds::handle_raw_variables(vec, idx);
  }

  return 0;
}

void Application::write(const char* text) {
  std::ofstream out("output.cc", std::ios::app);
  out << text;
  out.close();
}
