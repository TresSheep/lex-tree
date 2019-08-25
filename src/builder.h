#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

typedef struct case_arg
{
  std::vector<std::string> call_stack;
  std::string create;
} case_arg_t;

typedef struct same_arg
{
  // Maybe make a vector of args
  case_arg_t* arg1;
  case_arg_t* arg2;
  size_t index;
} same_arg_t;

class builder
{
 public:
  builder();
  ~builder();

  void add_token(std::string name, std::string content);
  void add_function_start(std::string name);
  void add_function_end();

  void add_case(std::vector<std::string> args);

  void add_create(std::string obj, std::vector<size_t> args);
  
  void save();
  
 private:
  std::stringstream m_lexer_cpp_stream;
  std::stringstream m_lexer_h_stream;
  std::stringstream m_ast_inl_stream;
  std::stringstream m_ast_functions;
  std::string       m_create;
  std::string       m_current_function;
  std::vector<std::string> m_functions;

  std::vector<case_arg_t*> m_case_args;
};
