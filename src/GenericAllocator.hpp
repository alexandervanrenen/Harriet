#ifndef SCRIPTLANGUAGE_GENERICALLOCATORPOLICIES_HPP
#define SCRIPTLANGUAGE_GENERICALLOCATORPOLICIES_HPP
//---------------------------------------------------------------------------
#include "GenericAllocatorPolicies.hpp"
#include <stdint.h>
#include <cassert>
//---------------------------------------------------------------------------
// Harriet Script Language
// Copyright (c) 2012 Alexander van Renen (alexandervanrenen@gmail.com)
// See the file LICENSE.txt for copying permission.
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
/// host class => a customizable fast allocator
template<class T, // class wich should be allocated
         template <class Type> class Allocator = FreeListPolicy, // allocator type
         class LockPolicy = NoLockPolicy, // should locking be used
         class InheritenceSupport = AssertOnInheritencePolicy> // 
class GenericAllocator : public Allocator<T>, LockPolicy, InheritenceSupport {
public:

   static void* operator new(std::size_t size) throw(std::bad_alloc);

   static void operator delete(void* data, std::size_t size) throw();

protected:
   ~GenericAllocator() {} // protected so no one can delete the derived class by the base ptr
};
//---------------------------------------------------------------------------
/// new operator of host class, combining the policies
template<class T, template <class> class Allocator, class LockPolicy, class InheritenceSupport>
void* GenericAllocator<T, Allocator, LockPolicy, InheritenceSupport>::operator new(std::size_t size) throw(std::bad_alloc)
{
   typename LockPolicy::Lock guard;

   if((sizeof(T)!=size))
      return InheritenceSupport::allocate(size);

   return Allocator<T>::allocate(size);
}
//---------------------------------------------------------------------------
/// delete operator of host class, combining the policies
template<class T, template <class Type> class Allocator, class LockPolicy, class InheritenceSupport>
void GenericAllocator<T, Allocator, LockPolicy, InheritenceSupport>::operator delete(void* data, std::size_t size) throw()
{
   typename LockPolicy::Lock guard;

   if((sizeof(T)!=size)) {
      InheritenceSupport::deallocate(data, size);
      return;
   }

   Allocator<T>::deallocate(data, size);
}
//---------------------------------------------------------------------------
} // end of namesapce scriptlanguage
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
