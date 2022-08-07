#include "parser.h"

namespace parser
{
  void parse(std::string p_code, std::vector<std::string> *m_args)
  {
    p_code += ' ';
    
    std::string m_str_buffer = "";
    bool m_block_start = false;
    bool m_tuple_start = false;
    bool m_str_start = false;

    for ( const char &m_current_char : p_code)
    {
      if (!m_str_start && m_current_char == '=' && (m_str_buffer == "+" || m_str_buffer == "-" || m_str_buffer == "*" || m_str_buffer == "/" || m_str_buffer == "=")) {
        m_str_buffer += m_current_char;
        m_args->push_back(m_str_buffer);
        m_str_buffer = "";
        continue;
      }

      if (!m_str_start && m_current_char == '{' || m_current_char == '}') {
        m_str_buffer += m_current_char;
        m_args->push_back(m_str_buffer);
        m_str_buffer = "";
        continue;
      }

      if (!m_str_start && m_current_char == '(' || m_current_char == ')') {
        m_tuple_start = !m_tuple_start; 
        m_args->push_back(m_str_buffer); 
        m_str_buffer = "";
        continue; 
      }

      if (m_current_char == '"')
      { m_str_start = !m_str_start; m_str_buffer += '"'; continue; }

      // if (!m_str_start && (m_current_char == '[' || m_current_char == ']')) {
      //   if (m_current_char == '[') {
      //     printf("Not Pushing\n");
      //     m_str_buffer += '[';
      //     m_block_start = true;
      //   } else {
      //     printf("Pushing\n");
      //     m_str_buffer += "]";
      //     m_args->push_back(m_str_buffer);
      //     m_str_buffer = "";
      //     m_block_start = false;
      //   }
      // }

      if (!m_block_start && !m_str_start && !m_tuple_start && (m_current_char == ' ' || m_current_char == '\n'))
      {
        if (m_current_char == '\n') {
          m_args->push_back(m_str_buffer);
          m_str_buffer = "";

          m_args->push_back("⌂");

          continue;
        }
        m_args->push_back(m_str_buffer);
        m_str_buffer = "";
        continue;
      }

      m_str_buffer += m_current_char;
    }

    // for (std::string &s : m_args)
    // {
    //   std::string temp_str = "";
    //   for (char &m_char : s)
    //   {
    //     if (m_char == '\n')
    //       continue;
    //     temp_str += m_char;
    //   }
    //   s = temp_str;
    // }
  }
}
