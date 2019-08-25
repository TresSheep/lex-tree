#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>

class _case : public statement
{
 public:
  _case(std::vector<std::string> args, std::vector<statement*> body);
  ~_case();
  
  void generate_code(builder& b) override;

 private:
  std::vector<std::string> m_args;
  std::vector<statement*>  m_body;
};
