#include "Expression.hpp"
#include "ExpressionParser.hpp"
#include "Utility.hpp"
#include "Environment.hpp"
#include "Function.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <stack>
#include <cassert>
#include <cmath>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
struct OpeningPharentesis : public Expression {
   virtual void print(ostream& stream) const {stream << " ( ";}
   virtual uint8_t priority() const {return 0;}
   virtual ExpressionType getExpressionType() const {return ExpressionType::TOpeningPharentesis;}
   virtual Associativity getAssociativity() const {throw;}
   virtual unique_ptr<Value> evaluate(Environment& /*environment*/) const {throw;}
};
//---------------------------------------------------------------------------
struct ClosingPharentesis : public Expression {
   virtual void print(ostream& stream) const {stream << " ) ";}
   virtual uint8_t priority() const {throw;}
   virtual ExpressionType getExpressionType() const {return ExpressionType::TClosingPharentesis;}
   virtual Associativity getAssociativity() const {throw;}
   virtual unique_ptr<Value> evaluate(Environment& /*environment*/) const {throw;}
};
//---------------------------------------------------------------------------
unique_ptr<Expression> ExpressionParser::parse(const string& inputString, Environment& environment)
{
   // set up data
   istringstream input(inputString);
   stack<unique_ptr<Expression>> outputStack;
   stack<unique_ptr<Expression>> operatorStack;
   scriptlanguage::skipWhiteSpace(input);
   ExpressionType lastExpressionType = ExpressionType::TOpeningPharentesis; // needed for not context free operators

   // parse input and build PRN on the fly
   while(input.good()) {
      // get next token
      auto token = parseSingleExpression(input, lastExpressionType, environment);
      if(token==nullptr)
         break;
      if(token->getExpressionType()==lastExpressionType && lastExpressionType==ExpressionType::TValue)
         throw scriptlanguage::Exception("missing operator");
      lastExpressionType = token->getExpressionType();
      scriptlanguage::skipWhiteSpace(input);

      // shunting yard algorithm -- proces tokens
      switch(token->getExpressionType()) {
         case ExpressionType::TVariable:
         case ExpressionType::TValue:
         case ExpressionType::TFunctionOperator:
            outputStack.push(::move(token));
            continue;
         case ExpressionType::TBinaryOperator:
         case ExpressionType::TUnaryOperator:
            while( (!operatorStack.empty() && operatorStack.top()->getExpressionType()!=ExpressionType::TOpeningPharentesis)
               &&  (  (token->getAssociativity()==Associativity::TLeft && token->priority()>=operatorStack.top()->priority())
                    ||(token->getAssociativity()==Associativity::TRight && token->priority()>operatorStack.top()->priority()))) {
                  auto stackToken = ::move(operatorStack.top());
                  operatorStack.pop();
                  pushToOutput(outputStack, ::move(stackToken));
               }
            operatorStack.push(::move(token));
            continue;
         case ExpressionType::TOpeningPharentesis:
            operatorStack.push(::move(token));
            continue;
         case ExpressionType::TClosingPharentesis:
            while(true) {
               // get top operator
               if(operatorStack.empty())
                  throw scriptlanguage::Exception{"parenthesis missmatch: missing '('"};
               auto stackToken = ::move(operatorStack.top());
               operatorStack.pop();
               if(stackToken->getExpressionType() == ExpressionType::TOpeningPharentesis)
                  break; else
                  pushToOutput(outputStack, ::move(stackToken));
            }
            continue;
      }
   }

   // shunting yard algorithm -- clear stack
   while(!operatorStack.empty()) {
      auto stackToken = ::move(operatorStack.top());
      operatorStack.pop();
      if(stackToken->getExpressionType() == ExpressionType::TOpeningPharentesis)
         throw scriptlanguage::Exception{"parenthesis missmatch: missing ')'"};
      pushToOutput(outputStack, ::move(stackToken));
   }

   assert(outputStack.size() == 1);
   return ::move(outputStack.top());
}
//---------------------------------------------------------------------------
unique_ptr<Expression> ExpressionParser::parseSingleExpression(istream& input, ExpressionType lastExpression, Environment& environment)
{
   // read
   scriptlanguage::skipWhiteSpace(input);
   char a = input.get();
   if(!input.good())
      return nullptr;

   // other single letter operators
   if(a == '(') return make_unique<OpeningPharentesis>();
   if(a == ')') return make_unique<ClosingPharentesis>();
   if(a == '+') return make_unique<PlusOperator>();
   if(a == '-') { if(lastExpression==ExpressionType::TBinaryOperator || lastExpression==ExpressionType::TUnaryOperator || lastExpression==ExpressionType::TOpeningPharentesis) return make_unique<UnaryMinusOperator>(); else return make_unique<MinusOperator>(); }
   if(a == '*') return make_unique<MultiplicationOperator>();
   if(a == '/') return make_unique<DivisionOperator>();
   if(a == '%') return make_unique<ModuloOperator>();
   if(a == '^') return make_unique<ExponentiationOperator>();
   if(a == '&') return make_unique<AndOperator>();
   if(a == '|') return make_unique<OrOperator>();
   if(a=='>' && input.peek()!='=') return make_unique<GreaterOperator>();
   if(a=='<' && input.peek()!='=') return make_unique<LessOperator>();
   if(a=='!' && input.peek()!='=') return make_unique<NotOperator>();
   if(a=='=' && input.peek()!='=') return make_unique<AssignmentOperator>();

   // check for string
   char b = input.get();
   if(a=='"') {
      string result;
      while(b!='"' && a!='\\') {
         if(!input.good())
            throw scriptlanguage::Exception{"unterminated string expression"};
         result.push_back(b);
         a = b;
         b = input.get();
      }
      return make_unique<StringValue>(result);
   }

   // check for two signed letters
   if(input.good()) {
      if(a=='=' && b=='=') return make_unique<EqualOperator>();
      if(a=='>' && b=='=') return make_unique<GreaterEqualOperator>();
      if(a=='<' && b=='=') return make_unique<LessEqualOperator>();
      if(a=='!' && b=='=') return make_unique<NotEqualOperator>();
      input.unget();
   } else {
      input.clear();
   }

   // check for a number
   input.unget();
   if(isdigit(a)) {
      int32_t intNum;
      input >> intNum;
      if(input.peek()=='.' && input.good()) {
         float floatNum;
         input >> floatNum;
         return make_unique<FloatValue>(floatNum+intNum);
      } else {
         return make_unique<IntegerValue>(intNum);
      }
      throw scriptlanguage::Exception{"unable to parse expression as number"};
   }

   // read the name
   string word = scriptlanguage::parseIdentifier(input);
   if(isalpha(a)) {
      // try bool
      if(word == scriptlanguage::kTrue) return make_unique<BoolValue>(true);
      if(word == scriptlanguage::kFalse) return make_unique<BoolValue>(false);

      // try cast
      if(word == scriptlanguage::kCastName)
         return parseCast(input);

      // try function
      if(environment.hasFunction(word))
         return parseFunctionHeader(word, input, environment);

      // try variable
      if(environment.isInAnyScope(word))
         return make_unique<Variable>(word);

      // error
      throw scriptlanguage::Exception{"found unkown identifier: '" + word + "'"};
   }

   throw scriptlanguage::Exception{"unable to parse expression, invaild sign '" + string(1, a) + "'"};
}
//---------------------------------------------------------------------------
void ExpressionParser::pushToOutput(stack<unique_ptr<Expression>>& workStack, unique_ptr<Expression> element) // AAA split
{
   assert(element->getExpressionType()==ExpressionType::TUnaryOperator || element->getExpressionType()==ExpressionType::TBinaryOperator);

   // unary operator
   if(element->getExpressionType() == ExpressionType::TUnaryOperator) {
      if(workStack.size()<1)
         throw scriptlanguage::Exception{"to few arguments for unaray operator " + reinterpret_cast<UnaryOperator*>(element.get())->getSign()};
      auto operand = ::move(workStack.top());
      workStack.pop();
      reinterpret_cast<UnaryOperator*>(element.get())->addChild(::move(operand));
      workStack.push(::move(element));
      return;
   }

   // binary operator
   if(element->getExpressionType() == ExpressionType::TBinaryOperator) {
      if(workStack.size()<2)
         throw scriptlanguage::Exception{"to few arguments for binary operator " + reinterpret_cast<BinaryOperator*>(element.get())->getSign()};
      auto rhs = ::move(workStack.top());
      workStack.pop();
      auto lhs = ::move(workStack.top());
      workStack.pop();
      reinterpret_cast<BinaryOperator*>(element.get())->addChildren(::move(lhs), ::move(rhs));
      workStack.push(::move(element));
      return;
   }
}
//---------------------------------------------------------------------------
unique_ptr<CastOperator> ExpressionParser::parseCast(istream& input)
{
   // extract '<', read type and extract '>'
   char a = input.get();
   if(a!='<')
      throw scriptlanguage::Exception{string("invalid cast syntax, expected '<' got '") + a + "'. usage: cast<type> value"};
   scriptlanguage::VariableType type = scriptlanguage::nameToType(scriptlanguage::readOnlyAlpha(input));
   a = input.get();
   if(a!='>')
      throw scriptlanguage::Exception{string("invalid cast syntax, expected '>' got '") + a + "'. usage: cast<type> value"};

   // create cast operator
   switch(type) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerCast>();
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatCast>();
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolCast>();
      case scriptlanguage::VariableType::TString:  return make_unique<StringCast>();
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorCast>();
      default:                                     throw scriptlanguage::Exception{string("i am tiered right now but something seems wrong")};
   }
}
//---------------------------------------------------------------------------
unique_ptr<FunctionOperator> ExpressionParser::parseFunctionHeader(const string& functionName, istream& input, Environment& environment)
{
   // parse arguments and get functions
   auto splittedArguments = splitFunctionArguments(input, functionName);
   auto possibleFunctions = environment.getFunction(functionName);
   vector<unique_ptr<Expression>> arguments;

   // check for 0 argument function
   if(splittedArguments.size()==1 && splittedArguments[0].size()==0)
      for(auto iter : possibleFunctions)
         if(iter->getArgumentCount()==0)
            return make_unique<FunctionOperator>(iter->getName(), iter->getId(), arguments);

   // convert argument strings to expressions
   for(auto& iter : splittedArguments)
      if(iter.size()==0)
         throw scriptlanguage::Exception{"in function '" + functionName + "': found empty argument"}; else
         arguments.push_back(parse(iter, environment));

   // evaluate arguments to access the type
   vector<unique_ptr<Value>> evaluatedArguments;
   for(auto& iter : arguments)
      evaluatedArguments.push_back(iter->evaluate(environment));

   // find matching function
   for(uint32_t i=0; i<evaluatedArguments.size(); i++) {
      // search matches for argument i
      vector<const Function*> simpleMatches; // matching patterns without cast
      vector<const Function*> castMatches; // matching patterns with cast
      for(auto function : possibleFunctions)
         if(function->getArgumentCount() == evaluatedArguments.size()) {
            if(function->getArgumentType(i) == evaluatedArguments[i]->getResultType())
               simpleMatches.push_back(function);
            else if(scriptlanguage::isImplicitCastPossible(function->getArgumentType(i), evaluatedArguments[i]->getResultType()))
               castMatches.push_back(function);
         }

      // refresh buffer
      if(simpleMatches.size()!=0)
         possibleFunctions = simpleMatches;
      else if(castMatches.size()!=0)
         possibleFunctions = castMatches;
      else
         possibleFunctions.clear();
   }

   // possibleFunctions contains now all callable functions -- should be exactly one
   if(possibleFunctions.size() == 1) {
      // created needed casts
      for(uint32_t i=0; i<evaluatedArguments.size(); i++)
         if(possibleFunctions[0]->getArgumentType(i) != evaluatedArguments[i]->getResultType())
            arguments[i] = scriptlanguage::createCast(::move(arguments[i]), possibleFunctions[0]->getArgumentType(i));

      // create function
      return make_unique<FunctionOperator>(possibleFunctions[0]->getName(), possibleFunctions[0]->getId(), arguments);
   }

   // no unique possible funciton => epic error msg
   string error = (possibleFunctions.size()==0?"no matching function for call to ":"ambiguous function call to ") + functionName + "(";
   for(uint32_t i=0; i<evaluatedArguments.size(); i++)
      error += scriptlanguage::typeToName(evaluatedArguments[i]->getResultType()) + (i+1==evaluatedArguments.size()?")":",");
   error += "\ncandidates are: \n";
   if(possibleFunctions.size() == 0)
      possibleFunctions = environment.getFunction(functionName);
   for(auto iter : possibleFunctions)
      error += "   " + iter->getFunctionHeader() + "\n";
   throw scriptlanguage::Exception{error};
}
//---------------------------------------------------------------------------
vector<string> ExpressionParser::splitFunctionArguments(istream& is, const string& functionName)
{
   // begin
   scriptlanguage::skipWhiteSpace(is);
   if(is.get() != '(')
      throw scriptlanguage::Exception{"expected opening parentesis '(' after function identifier: '" + functionName + "'"};

   // init
   int32_t parenthesisCount = 1;
   vector<string> result;
   string buffer;
   scriptlanguage::skipWhiteSpace(is);

   // read
   do {
      char a = is.get();
      if(a=='(') parenthesisCount++;
      if(a==')') parenthesisCount--;
      if(parenthesisCount < 0) throw scriptlanguage::Exception{"expected '(' before ')'"};

      // check if finished
      if(parenthesisCount==0) {
         result.push_back(buffer);
         return result;
      }

      // check if an argument is finished
      if(a==',' && parenthesisCount==1) {
         result.push_back(buffer);
         buffer = "";
         scriptlanguage::skipWhiteSpace(is);
      } else {
         buffer.push_back(a);
      }
   } while(is.good());

   // should not be reached
   throw scriptlanguage::Exception{"expected closing parentesis ')' after function identifier: '" + functionName + "'"};
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
