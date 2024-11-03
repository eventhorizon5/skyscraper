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
VMAllocation<T>::VMAllocation()
{

}

template <typename T>
VMAllocation<T>::VMAllocation(VMAllocation && other) noexcept : Ptr(std::move(other.Ptr)), Size(other.Size)
{
	other.Size = 0;
}

template <typename T>
[[nodiscard]] T* VMAllocator<T>::Allocator::allocate(std::size_t n)
{
	T *ret = new T[n];
	total_bytes += n;
	allocations += 1;

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
void VMAllocator<T>::Allocator::deallocate(T* p, std::size_t n)
{
	(void)n;
	if (Current.Ptr.get() == p)
	{
		CurrentDeallocated = true;
		return;
	}

	delete[] p;
	total_bytes -= n;
	deallocations += 1;
}

template <typename T>
VMAllocator<T>::VMAllocator() : m_allocator(std::make_shared<Allocator>())
{
	std::cout << "VMAllocator()" << std::endl;
}

template <typename T>
template <class U>
VMAllocator<T>::VMAllocator(const VMAllocator<U> &rhs) noexcept
{
	std::cout << "VMAllocator(const VMAllocator<U> &rhs)" << std::endl;
	// Just assume it's a allocator of the same type. This is needed in
	// MSVC STL library because of debug proxy allocators
	// https://github.com/microsoft/STL/blob/master/stl/inc/vector
	m_allocator = reinterpret_cast<const VMAllocator<T> &>(rhs).m_allocator;
}

template <typename T>
VMAllocator<T>::VMAllocator(const VMAllocator &rhs) noexcept : m_allocator(rhs.m_allocator)
{
	std::cout << "VMAllocator(const VMAllocator &rhs)" << std::endl;
}

template <typename T>
unsigned int VMAllocator<T>::GetAllocations() noexcept
{
	return m_allocator->allocations;
}

template <typename T>
unsigned int VMAllocator<T>::GetDeallocations() noexcept
{
	return m_allocator->deallocations;
}

template <typename T>
T* VMAllocator<T>::allocate(std::size_t n)
{
	std::cout << "allocate(" << n << ")" << std::endl;
	return m_allocator->allocate(n);
}

template <typename T>
void VMAllocator<T>::deallocate(T* p, std::size_t n)
{
	std::cout << "deallocate(\"" << p << "\", " << n << ")" << std::endl;
	std::cout << "total left: " << m_allocator->total_bytes << std::endl;
	return m_allocator->deallocate(p, n);
}

template <typename T>
VMAllocation<T> VMAllocator<T>::release()
{
	if (!m_allocator->CurrentDeallocated)
		throw std::runtime_error("Can't release the ownership if the current pointer has not been deallocated by the container");

	return std::move(m_allocator->Current);
}

// Instantiate the class for the supported template type parameters
template class VMAllocator<int>;
template class VMAllocator<long>;

}
