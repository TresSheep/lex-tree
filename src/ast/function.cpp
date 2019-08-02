#include "function.h"

function::function(std::string name, std::vector<std::unique_ptr<statement>> body) :
  m_name(name), m_body(std::move(body))
{
}

function::~function()
{
}

void function::generate_code(builder& b)
{
  b.add_function_start(m_name);

  for (auto& part : m_body)
    part->generate_code(b);

  b.add_function_end();
}
