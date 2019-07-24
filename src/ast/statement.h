#pragma once

#include "builder.h"

class statement
{
 public:
  virtual void generate_code(builder& b) = 0;
};
