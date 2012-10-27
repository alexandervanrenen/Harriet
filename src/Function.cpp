#include "Function.hpp"
#include "Expression.hpp"
#include "Environment.hpp"
#include "Utility.hpp"
#include <cassert>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
Function::Function(const string& name, uint32_t id, function<unique_ptr<Value>(vector<unique_ptr<Value>>&, Environment&)> func, vector<scriptlanguage::VariableType> argumentTypes, scriptlanguage::VariableType resultType)
: name(name)
, id(id)
, func(func)
, resultType(resultType)
{
   for(auto iter : argumentTypes)
      arguments.push_back(make_pair(iter, string("")));
}
//---------------------------------------------------------------------------
Function::~Function()
{
}
//---------------------------------------------------------------------------
unique_ptr<Value> Function::execute(vector<unique_ptr<Value>>& argv, Environment& env) const
{
   // execute build in function
   assert(argv.size() == arguments.size());
   return func(argv, env);
}
//---------------------------------------------------------------------------
scriptlanguage::VariableType Function::getResultType() const
{
   return resultType;
}
//---------------------------------------------------------------------------
uint32_t Function::getArgumentCount() const
{
   return arguments.size();
}
//---------------------------------------------------------------------------
scriptlanguage::VariableType Function::getArgumentType(uint32_t index) const
{
   assert(arguments.size()>index);
   return arguments[index].first;
}
//---------------------------------------------------------------------------
const string& Function::getName() const
{
   return name;
}
//---------------------------------------------------------------------------
const string Function::getFunctionHeader() const
{
   string result = name + "(";
   for(uint32_t i=0; i<arguments.size(); i++)
      result += scriptlanguage::typeToName(arguments[i].first) + " " + arguments[i].second + (i+1==arguments.size()?")":",");
   return result;
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
