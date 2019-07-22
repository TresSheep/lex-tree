#pragma once

#include "builder.h"

class expression
{
 public:
  virtual void generate_code(builder& b) = 0;
};
