#include "if.h"

_if::_if(std::string lhs, std::string type, std::string rhs, std::vector<statement*>& body) :
  m_lhs(lhs), m_type(type), m_rhs(rhs), m_body(body)
{
}

_if::~_if()
{
}

void _if::generate_code(builder& b)
{
  for (statement* s : m_body)
    s->generate_code(b);
  
  for (statement* s : m_body)
    delete s;
}
