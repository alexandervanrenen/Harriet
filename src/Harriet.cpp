#include "Harriet.hpp"
#include "Environment.hpp"
#include "Expression.hpp"
#include "ExpressionParser.hpp"
#include "Utility.hpp"
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
unique_ptr<Expression> parse(const string& input)
{
    Environment environment;
    return ExpressionParser::parse(input, environment);
}
//---------------------------------------------------------------------------
unique_ptr<Expression> parse(const string& input, Environment& environment)
{
    return ExpressionParser::parse(input, environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> evaluate(const string& input)
{
    Environment environment;
    return ExpressionParser::parse(input, environment)->evaluate(environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> evaluate(const string& input, Environment& environment)
{
    return ExpressionParser::parse(input, environment)->evaluate(environment);
}
//---------------------------------------------------------------------------
int32_t evaluateAsInteger(const string& input)
{
    Environment environment;
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto integerResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TInteger); // TODO: why not use a castToIntegerMethode ?
    return reinterpret_cast<IntegerValue*>(integerResultValue.get())->result;
}
//---------------------------------------------------------------------------
int32_t evaluateAsInteger(const string& input, Environment& environment)
{
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto integerResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TInteger);
    return reinterpret_cast<IntegerValue*>(integerResultValue.get())->result;
}
//---------------------------------------------------------------------------
float evaluateAsFloat(const string& input)
{
    Environment environment;
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto floatResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TFloat);
    return reinterpret_cast<FloatValue*>(floatResultValue.get())->result;
}
//---------------------------------------------------------------------------
float evaluateAsFloat(const string& input, Environment& environment)
{
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto floatResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TFloat);
    return reinterpret_cast<FloatValue*>(floatResultValue.get())->result;
}
//---------------------------------------------------------------------------
const string evaluateAsString(const string& input)
{
    Environment environment;
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto stringResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TString);
    return reinterpret_cast<StringValue*>(stringResultValue.get())->result;
}
//---------------------------------------------------------------------------
const string evaluateAsString(const string& input, Environment& environment)
{
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto stringResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TString);
    return reinterpret_cast<StringValue*>(stringResultValue.get())->result;
}
//---------------------------------------------------------------------------
const Vector3<float> evaluateAsVector(const string& input)
{
    Environment environment;
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto vectorResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TVector);
    return reinterpret_cast<VectorValue*>(vectorResultValue.get())->result;
}
//---------------------------------------------------------------------------
const Vector3<float> evaluateAsVector(const string& input, Environment& environment)
{
    auto resultValue = ExpressionParser::parse(input, environment)->evaluate(environment);
    auto vectorResultValue = resultValue->computeCast(environment, scriptlanguage::VariableType::TVector);
    return reinterpret_cast<VectorValue*>(vectorResultValue.get())->result;
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
