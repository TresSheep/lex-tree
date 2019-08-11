#include "ast.h"
template <typename builder>
ast<builder>::ast(std::vector<token_t> token_stream, builder& b) :
m_token_stream(token_stream), m_line(1), m_builder(b)
{
}
template <typename builder>
ast<builder>::~ast()
{
}
template <typename builder>
void ast<builder>::parse()
{
for (size_t position = 0; position < m_token_stream.size(); position++)
{
parse_main(position);

}
}
template <typename builder>
void ast<builder>::parse_main(size_t& position)
{
switch (m_token_stream[position].type)
{
default:
std::cout << "Unexpected token";
exit(0);
break;
case identifier:
{
if (m_token_stream[position+1].type == identifier)
{
if (m_token_stream[position+2].type == separator)
{
command obj(s1,s2,s4);
obj.generate_code(m_builder);
break;
}
command obj(s1,s2);
obj.generate_code(m_builder);
}

} break;
case separator:
{
std::string s1 = m_token_stream[position].content;
if (m_token_stream[position+1].type != function_start)
{
std::cout << "ERROR: Unexpected token! Line: " << m_line << "\n";
exit(0);
}
std::string s2 = m_token_stream[position+1].content;
test obj();
obj.generate_code(m_builder);

} break;

}
}