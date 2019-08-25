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

}
}