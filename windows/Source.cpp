#include <type_traits>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "variable_allocator.h"

#define END this->mem_map.rbegin()
#define BEG this->mem_map.begin()

template <class _T>
static void check()
{
	if (!std::is_copy_constructible<_T>::value)
		throw std::invalid_argument("type given must be trivially copyable, but is not");
	if (!std::is_destructible<_T>::value)
		throw std::invalid_argument("type given must be destructible");
	else
		return;
}

template <class _Destructor_T>
static void destroy(_Destructor_T* ptr)
{
	delete ptr;
}

template <class _CopyC_T>
static void* copy(const _CopyC_T* rhs)
{
	return (void*)new _CopyC_T(*rhs);
}

template <class _Block>
static memblock_t init_memblock(_Block* rhs)
{
	check<_Block>();
	memblock_t new_block;

	new_block.hash_code = typeid(_Block).hash_code(); //get hash code of type
	new_block.dest = (FN_dest)destroy<_Block>; //store destructor
	new_block.copy_constructor = (FN_cc)copy<_Block>; //store copy constructor
	new_block.loc = new_block.copy_constructor((void*)rhs); //get pointer to type

	return new_block;
}

static void remove_memblock(memblock_t* mem)
{
	mem->dest(mem->loc);
}

memeblock_t::memblock_t()
{
	this->loc = NULL;
	this->dest = NULL;
	this->copy_constructor = NULL;
	hash_code = 0;
}