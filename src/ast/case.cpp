#include "case.h"

_case::_case(std::vector<std::string> args, std::vector<statement*> body) :
  m_args(args), m_body(body)
{
}

_case::~_case()
{
}

void _case::generate_code(builder& b)
{
  //b.add_case(m_args);
  
  for (statement* s : m_body)
  {
    s->generate_code(b);
  }

  for (statement* s : m_body)
    delete s;
}
