#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>

class _if : public statement
{
 public:
  _if(std::string lhs, std::string type, std::string rhs, std::vector<statement*>& body);
  ~_if();
  
  void generate_code(builder& b) override;

 private:
  std::string m_lhs;
  std::string m_type;
  std::string m_rhs;
  std::vector<statement*>& m_body;
};
