#include "create.h"

create::create(std::string obj, std::vector<size_t> args) :
  m_obj(obj), m_args(args)
{
}

create::~create()
{
}

void create::generate_code(builder& b)
{
  b.add_create(m_obj, m_args);
}
