#include "src/ExpressionParser.hpp"
#include "src/Expression.hpp"
#include "src/Environment.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file license.txt for copying permission.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
   // check arguments
   if(argc!=2) {
      cout << "usage: ./tester \"2*3*7\"" << endl;
      return 0;
   }

   // parse input and evalute the expression
   scriptlanguage::Environment env;
   auto expression = scriptlanguage::ExpressionParser::parse(argv[1], env);
   auto result = expression->evaluate(env);

   // print result and quit
   cout << "result: ";
   result->print(cout);
   cout << endl;
   return 0;
}
//---------------------------------------------------------------------------
