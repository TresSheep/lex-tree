#include "function.h"

function::function(std::string name, std::vector<statement*> body) :
  m_name(name), m_body(body)
{
}

function::~function()
{
}

void function::generate_code(builder& b)
{
  b.add_function_start(m_name);

  for (statement* part : m_body)
    part->generate_code(b);

  for (statement* part : m_body)
    delete part;

  b.add_function_end();
}
