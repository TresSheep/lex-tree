#include "case.h"

_case::_case(std::string name, std::vector<std::string> tokens_to_come_after) :
  m_name(name), m_tokens_to_come_after(tokens_to_come_after)
{
}

_case::~_case()
{
}

void _case::generate_code(builder& b)
{
}
