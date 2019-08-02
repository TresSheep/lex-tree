#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

typedef struct case_arg
{
  std::vector<std::string> call_stack;
  std::vector<std::string> after;
  case_arg* parent;
} case_arg_t;

class builder
{
 public:
  builder();
  ~builder();

  void add_token(std::string name, std::string content);
  void add_function_start(std::string name);
  void add_function_end();

  void add_case(std::vector<std::string> args, std::vector<std::string> tokens_to_come_after, bool child);
  
  void save();
  
 private:
  std::stringstream m_lexer_cpp_stream;
  std::stringstream m_lexer_h_stream;
  std::stringstream m_ast_inl_stream;
  std::stringstream m_ast_functions;

  std::vector<case_arg_t*> m_case_args;
};
