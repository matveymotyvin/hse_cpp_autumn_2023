#pragma once

#include <iostream>

class Allocator
{
public:
    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();
    // Это деструктор!
    ~Allocator();
        
    size_t max_size; // размер памяти
    size_t offset; // смещение от начала
    char * ptr = nullptr; // указатель на выделяемую память, по умолчанию память не выделена
};
