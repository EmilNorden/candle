#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include <type_traits>

class PoolAllocator
{
public:
    static void initialize(std::size_t size)
    {
        //m_memory = 
    }
    static void* Alloc(std::size_t size);
    static void Deallocate(void*);
private:
    static char *m_memory;    
};

#endif