#ifndef TESTS_RANDOMIZED_HPP_
#define TESTS_RANDOMIZED_HPP_
//---------------------------------------------------------------------------
#include "ScriptLanguage.hpp"
#include "Environment.hpp"
#include "ExpressionParser.hpp"
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012, 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
namespace tests {
//---------------------------------------------------------------------------
using namespace harriet;
//---------------------------------------------------------------------------
TEST(ExternalSort, ComplexSmall) {
   Environment env;
   auto expression = ExpressionParser::parse("5+3", env);
   auto result = expression->evaluate(env);
   ASSERT_EQ(reinterpret_cast<IntegerValue*>(result.get())->result, 8);
}
//---------------------------------------------------------------------------
} // end of namespace tests
//---------------------------------------------------------------------------
#endif
