#include "allocator.hpp"

void
Allocator::makeAllocator(size_t maxSize) // аллоцирование памяти
{
    if (ptr != nullptr){ // проверка на повторный вызов и освобождение памяти
        delete[] ptr;
        ptr = nullptr;
    }

    if (maxSize == 0){ // при выделении нулевой памяти, возвращаем аллокатор (по умолчанию указатель уже на nullptr)
        return;
    }
    
    offset = 0;
    max_size = maxSize;
    ptr = new char[maxSize]; 
}

char*
Allocator::alloc(size_t size)
{
    if (size == 0){ // проверка на попытку выделить нулевой блок памяти
        return nullptr;
    }

    if (offset + size > max_size){ // проверка на возможность выделить память
        return nullptr;
    }
    
    char* mem = ptr + offset;

    offset += size;

    return mem;
}

void
Allocator::reset() // сброс указателя для переиспользования памяти
{
    offset = 0;
}

Allocator::~Allocator() // деструктор
{
    delete[] ptr;    
}
