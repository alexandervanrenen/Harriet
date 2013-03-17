#ifndef SCRIPTLANGUAGE_SCRIPTLANGUAGE_HPP_
#define SCRIPTLANGUAGE_SCRIPTLANGUAGE_HPP_
//---------------------------------------------------------------------------
#include <array>
#include <string>
#include <stdint.h>
#include <memory>
#include <ios>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012, 2013 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
class Value;
class Expression;
//---------------------------------------------------------------------------
/// other key words
const std::string kTriggerName = "trigger";
const std::string kEventName = "event";
const std::string kStatusName = "status";
const std::string kActionName = "action";
const std::string kActiveStatus = "active";
const std::string kInActiveStatus = "inactive";
const std::string kCastName = "cast";

/// action types
const std::string kActionLocalVariable = "local";
const std::string kActionWhile = "while";
const std::string kActionIf = "if";
const std::string kActionElse = "else";
const std::string kActionSleep = "sleep";
const std::string kActionReturn = "return";

/// variable types
const std::string kVariableInteger = "int";
const std::string kVariableFloat = "float";
const std::string kVariableBool = "bool";
const std::string kVariableString = "string";
const std::string kVariableVector = "vector";
const std::string kVariableVoid = "void";

/// triggers
const std::string kTriggerGameStart = "gamestart";
const std::string kTriggerDeadShipModule = "deadshipmodule";

/// boolean values
const std::string kTrue = "true";
const std::string kFalse = "false";

/// variable types
enum struct VariableType : uint8_t {TInteger, TFloat, TBool, TString, TVector, TVoid};

/// exceptions
struct Exception : public std::exception {
   Exception(const std::string& message) : message(message) {}
   ~Exception() throw() {}
   const std::string message;
   virtual const char* what() const throw() {return message.c_str();}
};

/// all keywords
const std::array<std::string, 18>keywords = {{ "return",
                                               "break",
                                               kActionWhile,
                                               kEventName,
                                               kStatusName,
                                               "command",
                                               "define",
                                               kActionName,
                                               kTriggerName,
                                               kActiveStatus,
                                               kInActiveStatus,
                                               kTriggerGameStart,
                                               kTriggerDeadShipModule,
                                               kActionLocalVariable,
                                               kVariableInteger,
                                               kVariableFloat,
                                               kVariableBool,
                                               kVariableString,
                                       }};

/// helper functions
bool isKeyword(const std::string& str);

VariableType nameToType(const std::string& name) throw(Exception);
const std::string typeToName(VariableType type) throw();

std::unique_ptr<Value> createDefaultValue(VariableType type) throw();

const std::string parseIdentifier(std::istream& is) throw(Exception);
void skipWhiteSpace(std::istream& is) throw();
const std::string readOnlyAlpha(std::istream& is) throw();
const std::string readParenthesisExpression(char openginType, char closingType, std::istream& is) throw(Exception); // reads stream until .. closingType (counts opening)

bool isImplicitCastPossible(VariableType from, VariableType to) throw();
std::unique_ptr<Expression> createCast(std::unique_ptr<Expression> expression, scriptlanguage::VariableType resultType);
}
//---------------------------------------------------------------------------
#endif
