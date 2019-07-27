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
}
