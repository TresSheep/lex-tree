#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

class builder
{
 public:
  builder();
  ~builder();

  void add_token(std::string name, std::string content);
  
  void save();
  
 private:
  std::stringstream m_lexer_cpp_stream;
  std::stringstream m_lexer_h_stream;
  std::stringstream m_ast_inl_stream;
};
