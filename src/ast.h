#pragma once

#include "lexer.h"
#include "builder.h"
#include "ast/expression.h"
#include "ast/function.h"
#include "ast/statement.h"
#include "ast/case.h"
#include "ast/create.h"
#include "ast/if.h"
#include "ast/var.h"
#include <memory>

typedef struct block
{
  statement* stmnt;
  size_t nested_pos;
  std::vector<statement*> statements;
  bool taken_care_of;
} block_t;

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
  std::vector<statement*> m_exprs;
  std::unique_ptr<expression> m_expression;
};
