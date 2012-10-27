#ifndef SCRIPTLANGUAGE_GENERICALLOCATOR_HPP
#define SCRIPTLANGUAGE_GENERICALLOCATOR_HPP
//---------------------------------------------------------------------------
#include <stdint.h>
#include <cassert>
#include <memory>
#include <cstdlib>
//---------------------------------------------------------------------------
// Utilities - generic allocator policies
// Alexander van Renen 2012
//---------------------------------------------------------------------------
namespace scriptlanguage {
//---------------------------------------------------------------------------
/// policy class  => use this policy for non thread safe allocators
struct NoLockPolicy {
   struct Lock {
      Lock() {};
      ~Lock() {};
   };
protected:
   ~NoLockPolicy() {}
};
//---------------------------------------------------------------------------
/// policy class  => use this policy for thread safe allocators
struct SimpleLockPolicy {
   SimpleLockPolicy() {} // AAA implement
protected:
   ~SimpleLockPolicy() {}
};
//---------------------------------------------------------------------------
/// policy class => if the allocator is used for a derived class std::new is used
struct UseNewOnInheritencePolicy {
   static void* allocate(std::size_t size) {return ::operator new(size);}
   static void deallocate(void* data, std::size_t /*size*/) {::operator delete(data);}
protected:
   ~UseNewOnInheritencePolicy() {}
};
//---------------------------------------------------------------------------
/// policy class => if the allocator is used for a derived class an assert
struct AssertOnInheritencePolicy {
   static void* allocate(std::size_t /*size*/) {assert(false); return NULL;}
   static void deallocate(void* /*data*/, std::size_t /*size*/) {assert(false);}
protected:
   ~AssertOnInheritencePolicy() {}
};
//---------------------------------------------------------------------------
/// policy class  => use normal allocator
template<class T>
class StdAllocatorPolicy {
public:
   static void* allocate(std::size_t size) {return ::operator new(size);}
   static void deallocate(void* data, std::size_t size) {::operator delete(data);}
protected:
   ~StdAllocatorPolicy() {}
};
//---------------------------------------------------------------------------
/// policy class  => fixed allocator with a list of free memory chunks
template<class T>
class FreeListPolicy {
public:
   static void* allocate(std::size_t /*size*/)
   {
      if(nextFreeElement != NULL) {
         void* result = nextFreeElement;
         nextFreeElement = nextFreeElement->next;
         return result;
      }

      if(positionInCurrentChunk >= Chunk::chunkSize) {
         Chunk* lastChunk = currentChunk;
         currentChunk = new Chunk();
         lastChunk->next = currentChunk;
         positionInCurrentChunk = 0;
      }

      return currentChunk->mem + (positionInCurrentChunk++*sizeof(T));
   }

   static void deallocate(void* data, std::size_t /*size*/)
   {
      static_cast<FreeElement*>(data)->next = nextFreeElement;
      nextFreeElement = static_cast<FreeElement*>(data);
   }
protected:
   ~FreeListPolicy() {}
private:
   struct Chunk
   {
      Chunk()
      {
         mem = new uint8_t[chunkSize*sizeof(T)];
         next = NULL;
      }
      ~Chunk()
      {
         delete [] mem;
         if(next!=NULL)
            delete next;
      }
      static const uint64_t chunkSize = 64;
      uint8_t* mem;
      Chunk* next;
   };

   struct FreeElement
   {
      FreeElement* next;
   };

   static FreeElement* nextFreeElement;
   static Chunk firstChunk;
   static Chunk* currentChunk;
   static uint32_t positionInCurrentChunk;
};
//---------------------------------------------------------------------------
template<class T>
typename FreeListPolicy<T>::Chunk FreeListPolicy<T>::firstChunk;
//---------------------------------------------------------------------------
template<class T>
typename FreeListPolicy<T>::Chunk* FreeListPolicy<T>::currentChunk = &firstChunk;
//---------------------------------------------------------------------------
template<class T>
typename FreeListPolicy<T>::FreeElement* FreeListPolicy<T>::nextFreeElement = NULL;
//---------------------------------------------------------------------------
template<class T>
uint32_t FreeListPolicy<T>::positionInCurrentChunk;
//---------------------------------------------------------------------------
} // end of namesapce scriptlanguage
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
