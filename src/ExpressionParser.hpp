#ifndef SCRIPTLANGUAGE_EXPRESSIONPARSER_HPP_
#define SCRIPTLANGUAGE_EXPRESSIONPARSER_HPP_
//---------------------------------------------------------------------------
#include "ScriptLanguage.hpp"
#include "Expression.hpp"
#include <memory>
#include <string>
#include <ios>
#include <stack>
#include <vector>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
class Environment;
//---------------------------------------------------------------------------
class ExpressionParser {
public:
   /// convert a simple string to a expression
   static std::unique_ptr<Expression> parse(const std::string& input, Environment& environment);

protected:
   /// get next token
   static std::unique_ptr<Expression> parseSingleExpression(std::istream& input, ExpressionType lastExpression, Environment& environment);

   /// append token to output
   static void pushToOutput(std::stack<std::unique_ptr<Expression>>& workStack, std::unique_ptr<Expression> element);

   /// cast parsing
   static std::unique_ptr<CastOperator> parseCast(std::istream& is);

   /// function call parsing
   static std::unique_ptr<FunctionOperator> parseFunctionHeader(const std::string& functionName, std::istream& is, Environment& environment);
   static std::vector<std::string> splitFunctionArguments(std::istream& is, const std::string& functionName);
};
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
#endif
