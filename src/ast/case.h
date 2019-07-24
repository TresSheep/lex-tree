#pragma once

#include "builder.h"
#include "expression.h"
#include <vector>

class _case : public expression
{
 public:
  _case(std::string name, std::vector<std::string> tokens_to_come_after);
  ~_case();
  void generate_code(builder& b) override;

 private:
  std::string m_name;
  std::vector<std::string> m_tokens_to_come_after;
};
