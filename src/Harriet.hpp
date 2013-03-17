#include "Expression.hpp"
#include "Environment.hpp"
#include <memory>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file license.txt for copying permission.
//---------------------------------------------------------------------------
namespace harriet {
//---------------------------------------------------------------------------
/// Creates an expression from the given string in the given environment. 
std::unique_ptr<Expression> parse(const std::string& input);
std::unique_ptr<Expression> parse(const std::string& input, Environment& environment);

/// Parses the input and directly evaluates it.
std::unique_ptr<Value> evaluate(const std::string& input);
std::unique_ptr<Value> evaluate(const std::string& input, Environment& environment);

/// Parses the input and directly evaluates it as an integer.
int32_t evaluateAsInteger(const std::string& input);
int32_t evaluateAsInteger(const std::string& input, Environment& environment);

/// Parses the input and directly evaluates it as a float.
float evaluateAsFloat(const std::string& input);
float evaluateAsFloat(const std::string& input, Environment& environment);

/// Parses the input and directly evaluates it as a string.
const std::string evaluateAsString(const std::string& input);
const std::string evaluateAsString(const std::string& input, Environment& environment);

/// Parses the input and directly evaluates it as an vector.
const Vector3<float> evaluateAsVector(const std::string& input);
const Vector3<float> evaluateAsVector(const std::string& input, Environment& environment);
//---------------------------------------------------------------------------
} // end of namespace harriet
//---------------------------------------------------------------------------
