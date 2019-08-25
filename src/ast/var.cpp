#include "var.h"

var::var(std::string name, std::string type) :
  m_name(name), m_type(type)
{
}

var::~var()
{
}

void var::generate_code(builder& b)
{
  std::cout << "Test";
}
