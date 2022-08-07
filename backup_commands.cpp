#include "commands.h"

namespace cmds
{
  namespace stringTools {
    void replace(std::string &str, char v1, char v2)
    {
      for (char &c : str)
        if (c == v1)
          c = v2;
    }
  }

  namespace in
  {
    void write(const char* text) {
      std::ofstream out("output.cc", std::ios::app);
      out << text;
      out.close();
    }

    bool is_block(const std::string &str)
    {
      if (str[0] == '[' && str[str.size() - 1] == ']')
        return true;
      return false;
    }
    bool is_string(const std::string &str)
    {
      if (str[0] == '"' && str[str.size() - 1] == '"')
        return true;
      return false;
    }
  }

  void output_f(str_vec &p_args, int &idx) {
    const std::string &str = p_args[++idx];
    in::write(("printf(" + str + ");").c_str());
  }

  void output(str_vec &p_args, int &idx)
  {
    int &index = idx;
    std::string str = p_args[++index];

    in::write(("std::cout<<" + str + ';').c_str());
  }

  void output_ln(str_vec &p_args, int &idx)
  {
    int &index = idx;
    std::string str = p_args[++index];

    in::write(("std::cout<<" + str + "<<std::endl;").c_str());
  }

  void function(str_vec &p_args, int &idx) {
    int &index = idx;
    
    const std::string &m_function_name = p_args[++index];
    const std::string &m_function_args = p_args[++index];
    
    bool m_template = true;
    std::string prefix = "auto ";
    if (m_function_name == "main") { 
      prefix = "int "; 
    }

    /*
     *  -----------------------
     *  | Example native code |
     *  -----------------------
     *
     *  fn wish(name)
     *    print("Hello ")
     *    println(name)
     *  end
     *
     *  ------------------------
     *  | alternating C++ code |
     *  ------------------------
     *
     *  template <typename T1>
     *  auto wish(T1 name) {
     *   std::cout << "Hello ";
     *   std::cout << name << std::endl
     *  }
     *
     */

    //////////////////// count how many arguments are there
    std::vector<std::string> m_arguments;
    std::string buffer = "";
    int no_of_args = 0;
    for (const char &c : m_function_args) {
      if (c == ',')
      { m_arguments.push_back(buffer); buffer = ""; }
      else
        buffer += c;

      // for example
      // the argument can be: (name,age)
      if (c == ',')
        ++ no_of_args;
    }

    if (m_function_args.size() != 0)
      ++ no_of_args;
    if (buffer.size() != 0)
    { m_arguments.push_back(buffer); buffer = ""; }

    if (no_of_args == 0)
      m_template = false;
    ////////////////////

    //////////////////// add all the contents line-by-line in a vector before erasing the contents of the file
    std::vector<std::string> contents;
    buffer = "";

    // Read the contents of the file
    std::ifstream in("output.cc");

    while (!in.eof()) {
      std::getline(in, buffer);

      if (buffer.find("#include") <= buffer.size()) continue;

      contents.push_back(buffer);
      buffer = ""; 
    }

    in.close();
    ////////////////////


    // Clear the contents of the file
#if LINUX == 1
    system("echo > output.cc");
#else
    system("copy /Y nul output.cc");
#endif

    //////////////////// include all the headers
    std::ofstream os("output.cc");
    os << "#include<iostream>\n";
    for (const std::string &str : includes_)
      os << "#include<" << str << ">" << "\n";
    os.close();
    ////////////////////
    
    //////////////////// Prepare the template_string i.e. `template <typename T1, typename T2>`
    std::string template_string = "";
    if (m_template) {
      template_string = "template <";
      for (int c = 0; c < no_of_args; c++) {
        template_string += "typename T" + std::to_string(c + 1);
      
        if (c != no_of_args - 1) // `- 1` because `c` will start from 0
          template_string += ',';
      }
      template_string += ">";
    }
    ////////////////////

    //////////////////// create the actual function
    std::ofstream out("output.cc", std::ios::app);

    if (m_template)
      out << template_string << std::endl;

    out << prefix << m_function_name;

    out << '(';
    for (unsigned c = 0; c < m_arguments.size(); c++) {
      out << 'T' 
          << std::to_string(c + 1)
          << ' '
          << m_arguments[c];

      if (c != m_arguments.size() - 1)
        out << ',';
    }
    out << "){";
    out.close();
    ////////////////////
  
    // append the rest of code
    master_end_buffer = std::move(contents);

    // std::cout << "{" << master_end_buffer << "}\n";
 
    functions_.push_back(m_function_name);
  }

  void file_import(str_vec &p_args, int &idx) {
    const std::string &file_name = p_args[++idx];

    std::ifstream ifs(file_name);
    std::string code((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
   
    std::vector<std::string> vec;
    parser::parse(code, &vec);

    for (const std::string &str : vec)
      p_args.push_back(str);

    for (unsigned c = 0; c < vec.size(); c++)
      if (vec[c] == "fn")
        cmds::capture_func(vec[c + 1].c_str());
  }

  void end(str_vec &p_args, int &idx) {
    std::string buf;
    for (const std::string &str : master_end_buffer)
      buf += str;
    in::write(('}' + buf).c_str());
  }

  void capture_func(const char* func_name) {
    functions_.push_back(func_name);
  }

  void assign(str_vec &p_args, int &idx)
  {
    const std::string &key = p_args[++idx];
    std::string value = p_args[++idx];

    std::string type = "auto";
    std::string s = type + ' ' + key + '=' + value + ';';
    bool const_ = false;

    if (value == "$const") {
      const_ = true;
      value = p_args[++idx];
    }
    else if (value == "$int")
      type = "int";
    else if (value == "$string")
      type = "std::string";
    else if (value == "$cstring")
      type = "const char*";
    else if (value == "$char")
      type = "char";
    else if (value == "$float")
      type = "float";
    else if (value == "$double")
      type = "double";
    else if (value == "$array:string") {
      cmds::include("vector");
      type = "std::vector<std::string>";
    } else if (value == "$array:int") {
      cmds::include("vector");
      type = "std::vector<int>";
    } else if (value == "$array:float") {
      cmds::include("vector");
      type = "std::vector<float>";
    } else if (value == "$static") {
      type = "static auto";
    } else if (value == "$array:double") {
      cmds::include("vector");
      type = "std::vector<double>";
    } else if (value == "$array:char") {
      cmds::include("vector");
      type = "std::vector<char>";
    } else if (value[0] == '$') {
      std::cout << "[]Unknown valiable type: " << value << "\n";
    }

    if (type != "auto") { 
      value += p_args[++idx];
    } else {
      value += p_args[++idx];
    }

    StringTools::replace(value, "⌂", "");

    if (const_)
      type = "const " + type;

    for (const std::string &str : functions_) {
      if (value == str) {
        std::cout << "Found a func\n";
        const std::string &next_func = str;
        std::string next_func_args = p_args[++idx];
        if (next_func_args == "⌂")
          goto pro2;
pro1:
        value = next_func + '(' + next_func_args + ");";
        s = type + ' ' + key + '=' + value + ';';

pro2:
        value = next_func;
        s = type + ' ' + key + '=' + value + ';';


goto_statement:
        goto main;
      }
    }

    if (value != "⌂")
      s = type + ' ' + key + '=' + value + ';';
    else
      s = type + ' ' + key + ';';

main:
    in::write(s.c_str());
    variables_.push_back(key);
  }

  void repeat(str_vec &p_args, int &idx) {
    const std::string &str = p_args[++idx];
    int times = 0;
    try {
      times = stoi(str);
    } catch (std::exception &e) {
      printf("Error: %s\n", e.what());
      exit(-1);
    }
    in::write((std::string("for(int _=0;") + "_<" + std::to_string(times) + ";_++)").c_str());
  }

  void at_list(str_vec &p_args, int &idx) {
    if (p_args.size() >= idx - 1) {
      const std::string &var_name = p_args[idx - 1];
      const std::string &index = p_args[++idx];

      in::write((var_name + "[" + index + "]").c_str());
    }
  }

  void handle_raw_variables(str_vec &p_args, int &idx) {
    const std::string &var_name = p_args[idx];

    for (const std::string &str : functions_) {
      if (var_name == str) {
        std::cout << "found a func: " << str << std::endl;
        std::string block = var_name + p_args[++idx];

        StringTools::replace(block, str.c_str(), (str + "(").c_str());
        StringTools::replace(block, ";", ");");
        std::cout << "block is: " << block << std::endl;

        in::write(block.c_str());

        break;
      }
    }

    // for (const std::string &str : functions_) {
    //   if (str == var_name) {
    //     std::stringstream ss;
    //     for (;p_args[idx] != "⌂";++idx)
    //       ss << p_args[idx];
    //     ss << ';';
    //     std::string block = ss.str();
    //     ss.clear();
    //
    //     for (const std::string &func_name : functions_) {
    //       if (block.find(func_name) <= block.size()) {
    //         StringTools::replace(block, func_name.c_str(), (func_name + "(").c_str());
    //         StringTools::replace(block, ";", ");");
    //
    //         break;
    //       }
    //     }
    //     in::write(block.c_str());
    //     return;
    //   }
    // }
  }

  void label(str_vec &p_args, int &idx)
  {
    int &index = idx;
    const std::string &m_label_name = p_args[++index];
    in::write((m_label_name + ":").c_str());
  }

  void include(const std::string &name) {
    std::vector<std::string> contents;
    std::string buffer;

    // Read the contents of the file
    std::ifstream in("output.cc");

    while (!in.eof()) {
      std::getline(in, buffer);

      if (buffer.find("#include") <= buffer.size()) continue;

      contents.push_back(buffer);
      buffer = ""; 
    }

    in.close();

    // Clear the contents of the file
#if LINUX == 1
    system("echo > output.cc");
#else
    system("copy /Y nul output.cc");
#endif

    includes_.push_back(name);
    
    // include the library specified by the user
    std::ofstream out("output.cc");
    out << "#include<iostream>\n";
    for (const std::string &str_ : includes_)
      out << "#include<" << str_ << ">" << "\n";

    // append the rest of code
    for (std::string &str : contents)
      out << str << std::endl;

    out.close();
  }

  void raw_input(str_vec &p_args, int &idx) {
    const std::string &var_name = p_args[++idx];
    if (var_name == "⌂")
      in::write("std::cin.get();");
    else
      in::write(("std::cin>>" + var_name + ";").c_str());
  }

  void include(str_vec &p_args, int &idx) {
    int &index = idx;
    const std::string &m_name = p_args[++index];

    std::vector<std::string> contents;
    std::string buffer;

    // Read the contents of the file
    std::ifstream in("output.cc");

    while (!in.eof())
    { std::getline(in, buffer); contents.push_back(buffer); buffer = ""; }

    in.close();

    // Clear the contents of the file
#if LINUX == 1
    system("echo > output.cc");
#else
    system("copy /Y nul output.cc");
#endif
    
    // include the library specified by the user
    std::ofstream out("output.cc");
    out << "#include<iostream>\n";
    out << "#include<" << m_name << '>' << '\n';

    includes_.push_back(m_name);

    // append the rest of code
    for (std::string &str : contents)
    { out << str << std::endl; }
  }

  void raw_cpp(str_vec &p_args, int &idx) {
    int &index = idx;
    const std::string &m_name = p_args[++index];
    in::write(m_name.c_str());
  }

  void goto_label(str_vec &p_args, int &idx) {
    int &index = idx;

    const std::string &m_label_name = p_args[++index];
    in::write(("goto " + m_label_name + ';').c_str());
  }
  
  void if_block(str_vec &p_args, int &idx) {
    int &index = idx;
    std::string block = p_args[++++index];

    StringTools::replace(block, "and", "&&");
    StringTools::replace(block, "or", "||");

    in::write(("if(" + block + ")").c_str());
  }

  void for_loop(str_vec &p_args, int &idx) {
    const std::string &next_block = p_args[++++idx];
    const std::string &var_name = next_block.substr(0, 1).c_str();
    
    std::string times = next_block.substr(1, next_block.find(',')-1);

    std::string type;
    for (const std::string &str : variables_) {
      if (str == var_name) { type = ""; goto after; };
    }
    type = "auto";

after:

    std::string condition = next_block.substr(next_block.find(',') + 1);
    stringTools::replace(condition, ',', ';');

    in::write(("for (" + type + " " + var_name + '=' + times + ';' + condition + ')').c_str());
  }

  void loop(str_vec &p_args, int &idx) {
    in::write("while(true)");
  }

  void input(str_vec &p_args, int &idx) {
    int &index = idx;
    const std::string &m_var_name = p_args[++index];

    in::write(("std::getline(std::cin," + m_var_name + ");").c_str());
  }

  void else_if_block(str_vec &p_args, int &idx) {
    int &index = idx;
    const std::string &block = p_args[++++index];

    in::write(("else if(" + block + ")").c_str());
  }

  void else_block(str_vec &p_args, int &idx) {
    in::write("else ");
  }

  std::unordered_map<std::string, void(*)(str_vec&, int&)> var::commands  = {
    { "print", cmds::output },
    { "println", cmds::output_ln },
    { "printf", cmds::output_f },
    { "let", cmds::assign },
    { "lab", cmds::label },
    { "goto", cmds::goto_label },
    { "!", cmds::raw_cpp },
    { "fn", cmds::function },
    { "repeat", cmds::repeat },
    { "for", cmds::for_loop },
    { "input", cmds::input },
    { "raw_input", cmds::raw_input },
    { "include", cmds::include },
    { "if", cmds::if_block },
    { "else", cmds::else_block },
    { "at", cmds::at_list },
    { "elseif", cmds::else_if_block },
    { "loop", cmds::loop },
    { "import", cmds::file_import },
    { "end", cmds::end },

    { "{", [](str_vec& vec, int& idx){ cmds::in::write("{"); } },
    { "}", [](str_vec& vec, int& idx){ cmds::in::write("}"); } },
    { "return", [](str_vec& vec, int& idx){ cmds::in::write(("return " + vec[++idx] + ";").c_str());} },
    { "break", [](str_vec& vec, int& idx){ cmds::in::write("break;");} },
    { "continue", [](str_vec& vec, int& idx){ cmds::in::write("continue;"); }},
    { "\n",  [](str_vec& vec, int& idx){ /* do nothing */ } },
  };
}
