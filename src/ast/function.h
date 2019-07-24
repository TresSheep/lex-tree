#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>

class function
{
 public:
  function(std::string name, std::vector<statement> body);
  ~function();
  void generate_code(builder& b);

 private:
  std::string m_name;
  std::vector<statement> m_body;
};
