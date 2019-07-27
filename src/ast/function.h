#pragma once

#include "builder.h"
#include "statement.h"
#include <vector>
#include <memory>

class function
{
 public:
  function(std::string name, std::vector<std::unique_ptr<statement>> body);
  ~function();
  void generate_code(builder& b);

 private:
  std::string m_name;
  std::vector<std::unique_ptr<statement>> m_body;
};
