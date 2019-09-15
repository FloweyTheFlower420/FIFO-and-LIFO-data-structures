#ifndef _VARIABLE_ALLOCATOR_H
#define _VARIABLE_ALLOCATOR_H
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <typeinfo>

#define END this->mem_map.rbegin()
#define BEG this->mem_map.begin()
#define MATCH_ERR std::invalid_argument("type given does not match the one stored")
typedef void (*FN_dest)(void*);
typedef void* (*FN_cc)(void*);

struct memblock_t
{
	void* loc;
	FN_dest dest;
	FN_cc copy_constructor;
	size_t hash_code;

	memblock_t();
	memblock_t(const memblock_t& rhs);

};

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

class v_alloc
{
	size_t size;
	std::vector<memblock_t> mem_map;

public:

	v_alloc();
	~v_alloc();

	template <class _Block>
	size_t push_back(_Block* rhs)
	{
		check<_Block>();
		this->mem_map.push_back(init_memblock<_Block>(rhs));
		this->size += sizeof(_Block);
		return sizeof(_Block);
	}

	template <class _Block>
	size_t  push_front(_Block* rhs)
	{
		check<_Block>();
		this->mem_map.insert(BEG, init_memblock<_Block>(rhs));
		this->size += sizeof(_Block);
		return sizeof(_Block);
	}

	template <class _Block>
	size_t pop_back(_Block* rhs)
	{
		if (END->hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;
		check<_Block>();

		rhs = (_Block*)(END->copy_constructor(END->loc));
		END->dest(END->loc);
		this->mem_map.pop_back();
		return sizeof(_Block);
	}

	template <class _Block>
	size_t pop_front(_Block* rhs)
	{
		if (BEG->hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;

		check<_Block>();

		rhs = (_Block*)(BEG->copy_constructor(BEG->loc));
		BEG->dest(BEG->loc);
		this->mem_map.erase(BEG);
		return sizeof(_Block);
	}

	template <class _Block>
	size_t get(size_t pos, _Block* rhs)
	{
		check<_Block>();

		memblock_t block = this->mem_map[pos];
		if (block.hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;
		rhs = (_Block*)(block.copy_constructor(block.loc));
	}

	void* operator[] (size_t pos);
};

#undef END 
#undef BEG
#undef MATCH_ERR
#endif