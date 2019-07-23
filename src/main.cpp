#include <iostream>
#include <string>
#include "lexer.h"
#include "builder.h"
#include "ast.h"

int main(int argc, char** argv)
{
  std::cout << "LEX-TREE VER 1.0\n\n";

  std::string filename;
  for (int i = 0; i < argc; i++)
  {
    filename = argv[i];
  }

  lexer l(filename);
  builder b;
  ast stx(l.get_token_stream(), b);
  stx.parse();

  b.save();
  
  return 0;
}
