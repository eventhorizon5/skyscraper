/*
 * Code originally from here:
 * https://stackoverflow.com/questions/40514362/why-stdvector-does-not-have-a-release-method/59127415
 *
 * License: CC BY-SA 4.0
 * https://creativecommons.org/licenses/by-sa/4.0/
 */

#ifndef _ALLOC_ALLOCATOR_H
#define _ALLOC_ALLOCATOR_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>

// The requirements for the allocator where taken from Howard Hinnant tutorial:
// https://howardhinnant.github.io/allocator_boilerplate.html

namespace Alloc {

template <typename T>
struct MyAllocation
{
    size_t Size = 0;
    std::unique_ptr<T> Ptr;

    MyAllocation();
    MyAllocation(MyAllocation && other) noexcept;
};

// This allocator keep ownership of the last allocate(n)
template <typename T>
class MyAllocator
{
public:
    using value_type = T;

private:
    // This is the actual allocator class that will be shared
    struct Allocator
    {
        [[nodiscard]] T* allocate(std::size_t n);

        void deallocate(T* p, std::size_t n);

        MyAllocation<T> Current;
        bool CurrentDeallocated = false;

        int total_bytes = 0;
    };
public:
    MyAllocator();

    template<class U>
    MyAllocator(const MyAllocator<U> &rhs) noexcept;
    MyAllocator(const MyAllocator &rhs) noexcept;

public:
    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
    MyAllocation<T> release();

public:
    // This is the instance of the allocator that will be shared
    std::shared_ptr<Allocator> m_allocator;
};

// We assume allocators of different types are never compatible
template <class T, class U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) { return false; }

// We assume allocators of different types are never compatible
template <class T, class U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) { return true; }

}

#endif
