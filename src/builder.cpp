#include "builder.h"

builder::builder()
{
}

builder::~builder()
{
}

void builder::save(std::string filename)
{
  std::ofstream stream(filename);
  
  stream.close();
}
