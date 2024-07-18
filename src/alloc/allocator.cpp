/*
 * Code originally from here:
 * https://stackoverflow.com/questions/40514362/why-stdvector-does-not-have-a-release-method/59127415
 *
 * License: CC BY-SA 4.0
 * https://creativecommons.org/licenses/by-sa/4.0/
 */

#include "allocator.h"

namespace Alloc {

template <typename T>
MyAllocation<T>::MyAllocation()
{

}

template <typename T>
MyAllocation<T>::MyAllocation(MyAllocation && other) noexcept : Ptr(std::move(other.Ptr)), Size(other.Size)
{
	other.Size = 0;
}

template <typename T>
[[nodiscard]] T* MyAllocator<T>::Allocator::allocate(std::size_t n)
{
	T *ret = new T[n];
	total_bytes += n;

	if (!(Current.Ptr == nullptr || CurrentDeallocated))
	{
		// Actually release the ownership of the Current unique pointer
		Current.Ptr.release();
	}

	Current.Ptr.reset(ret);
	Current.Size = n;
	CurrentDeallocated = false;
	return ret;
}

template <typename T>
void MyAllocator<T>::Allocator::deallocate(T* p, std::size_t n)
{
	(void)n;
	if (Current.Ptr.get() == p)
	{
		CurrentDeallocated = true;
		return;
	}

	delete[] p;
	total_bytes -= n;
}

template <typename T>
MyAllocator<T>::MyAllocator() : m_allocator(std::make_shared<Allocator>())
{
	std::cout << "MyAllocator()" << std::endl;
}

template <typename T>
template <class U>
MyAllocator<T>::MyAllocator(const MyAllocator<U> &rhs) noexcept
{
	std::cout << "MyAllocator(const MyAllocator<U> &rhs)" << std::endl;
	// Just assume it's a allocator of the same type. This is needed in
	// MSVC STL library because of debug proxy allocators
	// https://github.com/microsoft/STL/blob/master/stl/inc/vector
	m_allocator = reinterpret_cast<const MyAllocator<T> &>(rhs).m_allocator;
}

template <typename T>
MyAllocator<T>::MyAllocator(const MyAllocator &rhs) noexcept : m_allocator(rhs.m_allocator)
{
	std::cout << "MyAllocator(const MyAllocator &rhs)" << std::endl;
}

template <typename T>
T* MyAllocator<T>::allocate(std::size_t n)
{
	std::cout << "allocate(" << n << ")" << std::endl;
	return m_allocator->allocate(n);
}

template <typename T>
void MyAllocator<T>::deallocate(T* p, std::size_t n)
{
	std::cout << "deallocate(\"" << p << "\", " << n << ")" << std::endl;
	std::cout << "total left: " << m_allocator->total_bytes << std::endl;
	return m_allocator->deallocate(p, n);
}

template <typename T>
MyAllocation<T> MyAllocator<T>::release()
{
	if (!m_allocator->CurrentDeallocated)
		throw std::runtime_error("Can't release the ownership if the current pointer has not been deallocated by the container");

	return std::move(m_allocator->Current);
}

// Instantiate the class for the supported template type parameters
template class MyAllocator<int>;
template class MyAllocator<long>;

}
