#include "ScriptLanguage.hpp"
#include "Utility.hpp"
#include "Expression.hpp"
#include "Environment.hpp"
#include "Function.hpp"
#include <sstream>
#include <ctype.h>
#include <cassert>
#include <istream>
#include <algorithm>
//---------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
bool isKeyword(const string& str)
{
   for(auto& iter : keywords)
      if(str==iter)
         return true;
   return false;
}
//---------------------------------------------------------------------------
VariableType nameToType(const string& name) throw(Exception)
{
   if(name == kVariableInteger)
      return VariableType::TInteger;
   if(name == kVariableFloat)
      return VariableType::TFloat;
   if(name == kVariableBool)
      return VariableType::TBool;
   if(name == kVariableString)
      return VariableType::TString;
   if(name == kVariableVector)
      return VariableType::TVector;
   if(name == kVariableVoid)
      return VariableType::TVoid;
   throw Exception{"invalid type name: " + name};
}
//---------------------------------------------------------------------------
const string typeToName(VariableType type) throw()
{
   switch(type) {
      case VariableType::TInteger:
         return kVariableInteger;
      case VariableType::TFloat:
         return kVariableFloat;
      case VariableType::TBool:
         return kVariableBool;
      case VariableType::TString:
         return kVariableString;
      case VariableType::TVector:
         return kVariableVector;
      case VariableType::TVoid:
         return kVariableVoid;
   }
   throw Exception{"unreachable"};
}
//---------------------------------------------------------------------------
unique_ptr<Value> createDefaultValue(VariableType type) throw()
{
   switch(type) {
      case VariableType::TInteger:
         return make_unique<IntegerValue>(0);
      case VariableType::TFloat:
         return make_unique<FloatValue>(.0f);
      case VariableType::TBool:
         return make_unique<BoolValue>(true);
      case VariableType::TString:
         return make_unique<StringValue>("");
      case VariableType::TVector:
         return make_unique<VectorValue>(Vector3<float>(0,0,0));
      case VariableType::TVoid:
         return make_unique<VoidValue>();
   }
   throw Exception{"unreachable"};
}
//---------------------------------------------------------------------------
const string parseIdentifier(istream& is) throw(Exception)
{
   skipWhiteSpace(is);

   if(!isalpha(is.peek()))
      throw Exception{"identifier can not start with: '" + string(1, is.peek()) + "'"};

   ostringstream result;
   char buffer;
   while((isalnum(is.peek()) || is.peek()=='_') && is.good()) {
      is >> buffer;
      result << buffer;
   }

   if(isKeyword(result.str()))
      throw Exception{"the keyword '" + result.str() + "' can not be used as an identifier"};
   return result.str();
}
//---------------------------------------------------------------------------
void skipWhiteSpace(istream& is) throw()
{
   while(isspace(is.peek()) && is.good())
      is.get();
}
//---------------------------------------------------------------------------
const string readOnlyAlpha(istream& is) throw()
{
   string result;
   while(isalpha(is.peek()) && is.good())
      result.push_back(is.get());
   return result;
}
//---------------------------------------------------------------------------
const string readParenthesisExpression(char openginType, char closingType, istream& is) throw(Exception)
{
   assert(closingType != openginType);
   if(is.peek() != openginType)
      throw Exception{"parenthesis expression has to start with parentesis"};
   int32_t parenthesisCount = 0;
   string result;

   do {
      // read
      char a = is.get();
      if(a==openginType) if(parenthesisCount++==0) continue;
      if(a==closingType) parenthesisCount--;
      if(parenthesisCount < 0) throw Exception{"expected '" + to_string(openginType) + "' before '" + to_string(closingType) + "'"};

      // check if finished
      if(parenthesisCount == 0)
         return result;
      result.push_back(a);
   } while(is.good());

   throw Exception{"parenthesis expression has to start with parentesis"};
}
//---------------------------------------------------------------------------
bool isImplicitCastPossible(VariableType from, VariableType to) throw()
{
   bool implicitCast[5][5] = {
      /* to\from      int     float   bool   string   vector*/
      /* int    */ {  true ,  true ,  false,  false,  false},
      /* float  */ {  true ,  true ,  false,  false,  false},
      /* bool   */ {  false,  false,  true ,  false,  false},
      /* string */ {  false,  false,  false,  true ,  false},
      /* vector */ {  false,  false,  false,  false,  true }
   };

   return implicitCast[static_cast<uint32_t>(to)][static_cast<uint32_t>(from)];
}
//---------------------------------------------------------------------------
unique_ptr<Expression> createCast(unique_ptr<Expression> expression, scriptlanguage::VariableType resultType)
{
   unique_ptr<CastOperator> result;
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: result = make_unique<IntegerCast>(); break;
      case scriptlanguage::VariableType::TFloat:   result = make_unique<FloatCast>();   break;
      case scriptlanguage::VariableType::TBool:    result = make_unique<BoolCast>();    break;
      case scriptlanguage::VariableType::TString:  result = make_unique<StringCast>();  break;
      case scriptlanguage::VariableType::TVector:  result = make_unique<VectorCast>();  break;
      default:                                     throw Exception{"unable to cast to: " + typeToName(resultType)};
   }
   result->addChild(::move(expression));
   return ::move(result);
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
