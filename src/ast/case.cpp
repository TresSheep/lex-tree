#include "case.h"

_case::_case(std::vector<std::string> args, std::vector<std::string> tokens_to_come_after, bool parent) :
  m_args(args), m_tokens_to_come_after(tokens_to_come_after), m_parent(parent)
{
}

_case::~_case()
{
}

void _case::generate_code(builder& b)
{
  b.add_case(m_args, m_tokens_to_come_after, m_parent);
}
