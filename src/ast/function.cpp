#include "function.h"

function::function(std::string name, std::vector<statement> body) :
  m_name(name), m_body(body)
{
}

function::~function()
{
}

void function::generate_code(builder& b)
{
}
