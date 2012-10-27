#include "Expression.hpp"
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
//
//
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
void Variable::print(ostream& stream) const
{
   stream << identifier << " ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> Variable::evaluate(Environment& environment) const
{
   const Value& result = environment.read(identifier);
   switch(result.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(reinterpret_cast<const IntegerValue&>(result).result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(reinterpret_cast<const FloatValue&>(result).result);
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(reinterpret_cast<const BoolValue&>(result).result);
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(reinterpret_cast<const StringValue&>(result).result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(reinterpret_cast<const VectorValue&>(result).result);
      default:                                     throw;
   }
}
//---------------------------------------------------------------------------
void IntegerValue::print(ostream& stream) const
{
   stream << result << "i ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::evaluate(Environment& /*environment*/) const
{
   return make_unique<IntegerValue>(result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeAdd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result + reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result + reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).add(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '+'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeSub(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result - reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result - reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).sub(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '-'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeMul(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result * reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result * reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).mul(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '*'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeDiv(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result / reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result / reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).div(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '/'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeMod(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: if(reinterpret_cast<const IntegerValue*>(&rhs)->result==0) return make_unique<IntegerValue>(0); else return make_unique<IntegerValue>(this->result % reinterpret_cast<const IntegerValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '%'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeExp(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(static_cast<int32_t>(pow(this->result, reinterpret_cast<const IntegerValue*>(&rhs)->result)));
      case scriptlanguage::VariableType::TFloat:   return make_unique<IntegerValue>(static_cast<float>(pow(this->result, reinterpret_cast<const FloatValue*>(&rhs)->result)));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '^'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeAnd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result & reinterpret_cast<const IntegerValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '&'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeOr (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result | reinterpret_cast<const IntegerValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '|'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeGt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result > reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result > reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '>'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeLt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result < reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result < reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '<'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeGeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result >= reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result >= reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '>='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeLeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result >= reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result <= reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '<='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeEq (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result == reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result == reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '=='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeNeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result != reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result != reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '!='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeInv(const Environment& /*env*/) const
{
                                                   return make_unique<IntegerValue>(-this->result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> IntegerValue::computeCast(const Environment& /*env*/, scriptlanguage::VariableType resultType) const
{
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result);
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result!=0);
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(to_string(this->result));
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(this->result, this->result, this->result));
      default:                                     throw scriptlanguage::Exception{"invalid cast targete: '" + scriptlanguage::typeToName(resultType) + "'"};
   }
}
//---------------------------------------------------------------------------
void FloatValue::print(ostream& stream) const
{
   stream << result << "f ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::evaluate(Environment& /*environment*/) const
{
   return make_unique<FloatValue>(result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeAdd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<FloatValue>(this->result + reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result + reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).add(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '+'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeSub(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<FloatValue>(this->result - reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result - reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).sub(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '-'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeMul(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<FloatValue>(this->result * reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result * reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).mul(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '*'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeDiv(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<FloatValue>(this->result / reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result / reinterpret_cast<const FloatValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(reinterpret_cast<const FloatValue*>(&rhs)->result).div(this->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '/'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeMod(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<FloatValue>(static_cast<int32_t>(this->result) % reinterpret_cast<const IntegerValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '%'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeExp(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(static_cast<float>(pow(this->result, reinterpret_cast<const IntegerValue*>(&rhs)->result)));
      case scriptlanguage::VariableType::TFloat:   return make_unique<IntegerValue>(static_cast<float>(pow(this->result, reinterpret_cast<const FloatValue*>(&rhs)->result)));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '^'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeGt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result > reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result > reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '>'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeLt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result < reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result < reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '<'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeGeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result >= reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result >= reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '>='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeLeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result >= reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result <= reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '<='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeEq (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result == reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result == reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '=='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeNeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<BoolValue>(this->result != reinterpret_cast<const IntegerValue*>(&rhs)->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<BoolValue>(this->result != reinterpret_cast<const FloatValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '!='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeInv(const Environment& /*env*/) const
{
                                                   return make_unique<FloatValue>(-this->result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> FloatValue::computeCast(const Environment& /*env*/, scriptlanguage::VariableType resultType) const
{
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result);
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result!=0);
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(to_string(this->result));
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(this->result, this->result, this->result));
      default: throw scriptlanguage::Exception{"invalid cast targete: '" + scriptlanguage::typeToName(resultType) + "'"};
   }
}
//---------------------------------------------------------------------------
void BoolValue::print(ostream& stream) const
{
   stream << (result?scriptlanguage::kTrue:scriptlanguage::kFalse) << " ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::evaluate(Environment& /*environment*/) const
{
   return make_unique<BoolValue>(result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeAnd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result & reinterpret_cast<const BoolValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '&'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeOr (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result | reinterpret_cast<const BoolValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '|'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeEq (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result == reinterpret_cast<const BoolValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '=='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeNeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result != reinterpret_cast<const BoolValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '!='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeNot(const Environment& /*env*/) const
{
                                                   return make_unique<BoolValue>(!this->result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> BoolValue::computeCast(const Environment& /*env*/, scriptlanguage::VariableType resultType) const
{
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result);
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result);
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(this->result?scriptlanguage::kTrue:scriptlanguage::kFalse);
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(Vector3<float>(this->result, this->result, this->result));
      default:                                     throw scriptlanguage::Exception{"invalid cast targete: '" + scriptlanguage::typeToName(resultType) + "'"};
   }
}
//---------------------------------------------------------------------------
void StringValue::print(ostream& stream) const
{
   stream << "\"" << result << "\" ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::evaluate(Environment& /*environment*/) const
{
   return make_unique<StringValue>(result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeAdd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<StringValue>(this->result + reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '+'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeGt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result > reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '>'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeLt (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result < reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '<'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeGeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result >= reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '>='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeLeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result <= reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '<='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeEq (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result == reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '=='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeNeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TString:    return make_unique<BoolValue>(this->result != reinterpret_cast<const StringValue*>(&rhs)->result);
      default:                                       throw scriptlanguage::Exception{"invalid input for binary operator '!='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> StringValue::computeCast(const Environment& /*env*/, scriptlanguage::VariableType resultType) const
{
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(to_number<int32_t>(this->result));
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(to_number<float>(this->result));
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result==scriptlanguage::kTrue || this->result=="0");
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(this->result);
      case scriptlanguage::VariableType::TVector:  {auto v=make_unique<VectorValue>(Vector3<float>(0)); istringstream is(this->result); is >> v->result; return ::move(v);}
      default:                                     throw scriptlanguage::Exception{"invalid cast targete: '" + scriptlanguage::typeToName(resultType) + "'"};
   }
}
//---------------------------------------------------------------------------
void VectorValue::print(ostream& stream) const
{
   stream << result;
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::evaluate(Environment& /*environment*/) const
{
   return make_unique<VectorValue>(result);
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeAdd(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<VectorValue>(Vector3<float>(this->result).add(reinterpret_cast<const IntegerValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TFloat:   return make_unique<VectorValue>(Vector3<float>(this->result).add(reinterpret_cast<const FloatValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(this->result + reinterpret_cast<const VectorValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '+'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeSub(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<VectorValue>(Vector3<float>(this->result).sub(reinterpret_cast<const IntegerValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TFloat:   return make_unique<VectorValue>(Vector3<float>(this->result).sub(reinterpret_cast<const FloatValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(this->result - reinterpret_cast<const VectorValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '-'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeMul(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<VectorValue>(Vector3<float>(this->result).mul(reinterpret_cast<const IntegerValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TFloat:   return make_unique<VectorValue>(Vector3<float>(this->result).mul(reinterpret_cast<const FloatValue*>(&rhs)->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '*'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeDiv(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TInteger: return make_unique<VectorValue>(Vector3<float>(this->result).div(reinterpret_cast<const IntegerValue*>(&rhs)->result));
      case scriptlanguage::VariableType::TFloat:   return make_unique<VectorValue>(Vector3<float>(this->result).div(reinterpret_cast<const FloatValue*>(&rhs)->result));
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '/'"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeEq (const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TVector:  return make_unique<BoolValue>(this->result == reinterpret_cast<const VectorValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '=='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeNeq(const Value& rhs, const Environment& /*env*/) const
{
   switch(rhs.getResultType()) {
      case scriptlanguage::VariableType::TVector:  return make_unique<BoolValue>(this->result != reinterpret_cast<const VectorValue*>(&rhs)->result);
      default:                                     throw scriptlanguage::Exception{"invalid input for binary operator '!='"};
   }
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeInv(const Environment& /*env*/) const
{
                                                   return make_unique<VectorValue>(Vector3<float>(this->result).inverse());
}
//---------------------------------------------------------------------------
unique_ptr<Value> VectorValue::computeCast(const Environment& /*env*/, scriptlanguage::VariableType resultType) const
{
   switch(resultType) {
      case scriptlanguage::VariableType::TInteger: return make_unique<IntegerValue>(this->result.x);
      case scriptlanguage::VariableType::TFloat:   return make_unique<FloatValue>(this->result.x);
      case scriptlanguage::VariableType::TBool:    return make_unique<BoolValue>(this->result.x!=0);
      case scriptlanguage::VariableType::TString:  return make_unique<StringValue>(this->result.toString());
      case scriptlanguage::VariableType::TVector:  return make_unique<VectorValue>(this->result);
      default:                                     throw scriptlanguage::Exception{"invalid cast targete: '" + scriptlanguage::typeToName(resultType) + "'"};
   }
}
//---------------------------------------------------------------------------
void VoidValue::print(ostream& stream) const
{
   stream << " void ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> VoidValue::evaluate(Environment& /*environment*/) const
{
   throw;
}
//---------------------------------------------------------------------------
void UnaryOperator::print(ostream& stream) const
{
   stream << " ( " << getSign();
   child->print(stream);
   stream << " ) ";
}
//---------------------------------------------------------------------------
void UnaryOperator::addChild(unique_ptr<Expression> child)
{
   this->child = ::move(child);
}
//---------------------------------------------------------------------------
unique_ptr<Value> UnaryMinusOperator::evaluate(Environment& environment) const
{
   return child->evaluate(environment)->computeInv(environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> NotOperator::evaluate(Environment& environment) const
{
   return child->evaluate(environment)->computeNot(environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> CastOperator::evaluate(Environment& environment) const
{
   return child->evaluate(environment)->computeCast(environment, getCastType());
}
//---------------------------------------------------------------------------
void BinaryOperator::addChildren(unique_ptr<Expression> lhsChild, unique_ptr<Expression> rhsChild)
{
   assert(lhs==nullptr && rhs==nullptr);
   lhs = ::move(lhsChild);
   rhs = ::move(rhsChild);
}
//---------------------------------------------------------------------------
void BinaryOperator::print(ostream& stream) const
{
   stream << " ( ";
   lhs->print(stream);
   stream << getSign();
   rhs->print(stream);
   stream << " ) ";
}
//---------------------------------------------------------------------------
unique_ptr<Value> AssignmentOperator::evaluate(Environment& environment) const
{
   if(lhs->getExpressionType() != ExpressionType::TVariable)
      throw scriptlanguage::Exception("need variable as left hand side of assignment operator");

   environment.update(reinterpret_cast<Variable*>(lhs.get())->getIdentifier(), rhs->evaluate(environment));
   return lhs->evaluate(environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> PlusOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeAdd(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> MinusOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeSub(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> MultiplicationOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeMul(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> DivisionOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeDiv(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> ModuloOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeMod(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> ExponentiationOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeExp(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> AndOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeAnd(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> OrOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeOr (*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> GreaterOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeGt (*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> LessOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeLt (*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> GreaterEqualOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeGeq(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> LessEqualOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeLeq(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> EqualOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeEq(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
unique_ptr<Value> NotEqualOperator::evaluate(Environment& environment) const
{
   return lhs->evaluate(environment)->computeNeq(*rhs->evaluate(environment), environment);
}
//---------------------------------------------------------------------------
FunctionOperator::FunctionOperator(const string& functionName, uint32_t functionIdentifier, vector<unique_ptr<Expression>>& arguments)
: functionName(functionName)
, functionIdentifier(functionIdentifier)
, arguments(::move(arguments))
{
}
//---------------------------------------------------------------------------
unique_ptr<Value> FunctionOperator::evaluate(Environment& environment) const
{
   // build arguments
   vector<unique_ptr<Value>> evaluetedArguments;
   auto function = environment.getFunction(functionIdentifier);
   for(uint32_t i=0; i<arguments.size(); i++) {
      auto result = arguments[i]->evaluate(environment);
      if(result->getResultType() != function->getArgumentType(i))
         throw scriptlanguage::Exception{"type missmatch in function '" + function->getName() + "' for argument '" + to_string(i) + "' unable to convert '" + scriptlanguage::typeToName(result->getResultType()) + "' to '" + scriptlanguage::typeToName(function->getArgumentType(i)) + "'"};
      evaluetedArguments.push_back(::move(result));
   }

   // call function
   return function->execute(evaluetedArguments, environment);
}
//---------------------------------------------------------------------------
void FunctionOperator::print(ostream& stream) const
{
   stream << " " << functionName << " id:" << functionIdentifier << endl;
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
