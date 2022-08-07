#include "stringTools.h"

namespace StringTools {
  bool startswith(const std::string &str, const char* fac) {
    if (strlen(fac) == 0 || strlen(fac) > str.size()) return false;
    if (strlen(fac) == str.size() && fac == str.c_str()) return true;

    if (str.substr(0, strlen(fac)) == std::string(fac)) return true;
    return false;
  }

  bool endswith(const std::string &str, const char* fac) {
    if (strlen(fac) == 0 || strlen(fac) > str.size()) return false;
    if (strlen(fac) == str.size() && fac == str.c_str()) return true;

    if (str.substr(str.size() - strlen(fac), str.size() - 1) == std::string(fac)) return true;
    return false;
  }

  void replace(std::string &str, const char *from, const char *to, bool repeat) {
    int where = str.find(from);
    while (where <= str.size()) {
      str = str.replace(where, strlen(from), to);
      if (repeat)
        where = str.find(from);
      else
        break;
    }
  }

  template <class ... T>
  std::string concat(T ... args) {
    const int size = sizeof...(args);
    std::string list[size] = { args... };

    std::string return_val = "";
    for (const std::string &str : list)
      return_val += str;

    return return_val;
  }
}
