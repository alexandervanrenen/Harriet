#ifndef SCRIPTLANGUAGE_UTILITY_HPP_
#define SCRIPTLANGUAGE_UTILITY_HPP_
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
/// creates a unique_ptr *vote for adding this to std*
template<class T, class... Arg>
std::unique_ptr<T> make_unique(Arg&& ...args)
{
   return std::unique_ptr<T>(new T(std::forward<Arg>(args)...));
}
//---------------------------------------------------------------------------
/// string --> number
template<class Number>
Number to_number(const std::string& str)
{
   Number num;
   std::istringstream stream(str);
   stream >> num;
   return num;
}
//---------------------------------------------------------------------------
} // end of namespace scriptlanguage
//---------------------------------------------------------------------------
#endif
