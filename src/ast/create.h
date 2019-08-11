#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>

class create : public statement
{
 public:
  create(std::string obj, std::vector<size_t> args);
  ~create();
  
  void generate_code(builder& b) override;

 private:
  std::string m_obj;
  std::vector<size_t> m_args;
};
