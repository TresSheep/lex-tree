#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>

class var : public statement
{
 public:
  var(std::string name, std::string type);
  ~var();
  
  void generate_code(builder& b) override;

 private:
  std::string m_name;
  std::string m_type;
};
