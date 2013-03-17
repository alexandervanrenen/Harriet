#include "Harriet.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file license.txt for copying permission.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
   // check arguments
   if(argc!=2) {
      cout << "usage: ./calculator \"2*3*7\"" << endl;
      return 0;
   }

   // parse input and evaluate the expression
   auto result = scriptlanguage::evaluate(argv[1]);
   scriptlanguage::Value& val = *result;

   // print result and quit
   cout << "result: " << val << endl;
   return 0;
}
//---------------------------------------------------------------------------
