#pragma once

#include "lexer.h"
#include "builder.h"
#include "ast/expression.h"
#include <memory>

class ast
{
 public:
  ast(std::vector<token_t> token_stream, builder& b);
  ~ast();

  void parse();

 private:
  std::vector<token_t> m_token_stream;
  size_t m_line;
  builder& m_builder;
  std::vector<std::unique_ptr<expression>> m_exprs;
};
