#ifndef _VARIABLE_ALLOCATOR_H
#define _VARIABLE_ALLOCATOR_H
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include <iostream>

#define END_ITER this->mem_map.rbegin()
#define BEG_ITER this->mem_map.begin()
#define END (*END_ITER)
#define BEG (*BEG_ITER)

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
	~memblock_t();
};

template <class _T> //check to see if type is CC or destrutible
static void check()
{
	if (!std::is_copy_constructible<_T>::value)
		throw std::invalid_argument("type given must be trivially copyable, but is not");
	if (!std::is_destructible<_T>::value)
		throw std::invalid_argument("type given must be destructible");
	else
		return;
}

template <class _Destructor_T> //get destroyer
static void destroy(_Destructor_T* ptr)
{
	delete ptr;
}

template <class _CopyC_T> //get copy_constructor
static void* copy(const _CopyC_T* rhs)
{
	return (void*)new _CopyC_T(*rhs);
}

template <class _Block> //initiate block
static memblock_t* init_memblock(_Block* rhs)
{
	check<_Block>(); //check if block is CC or destructible
	memblock_t* new_block = new memblock_t(); //new a memblock_t

	new_block->hash_code = typeid(_Block).hash_code(); //get hash code of type
	new_block->dest = (FN_dest)destroy<_Block>; //store destructor
	new_block->copy_constructor = (FN_cc)copy<_Block>; //store copy constructor
	new_block->loc = new_block->copy_constructor((void*)rhs); //get pointer to type

	return new_block;
}

class v_alloc
{
	size_t size;
	std::vector<memblock_t*> mem_map;

public:

	inline v_alloc(){ this->size = 0; }
	inline v_alloc(const v_alloc& rhs) { this->mem_map = rhs.mem_map; }

	template <class _Block> //push a [type] to the end of container
	size_t push_back(_Block* rhs)
	{
		check<_Block>(); //check to see if type is CC and destructible
		this->mem_map.push_back(init_memblock<_Block>(rhs));
		this->size += sizeof(_Block); //increase size counter
		return sizeof(_Block); //return size of block pushed
	}

	template <class _Block> //push a [type] to the front of container
	size_t  push_front(_Block* rhs)
	{
		check<_Block>();
		this->mem_map.insert(BEG, init_memblock<_Block>(rhs));
		this->size += sizeof(_Block);
		return sizeof(_Block);
	}

	template <class _Block> //pop a [type] from the end of container
	size_t pop_back(_Block* rhs)
	{
		if (END->hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;
		check<_Block>();

		_Block *p = (_Block*)(END->copy_constructor(END->loc));
		*rhs = *p;
		delete p;
		delete END;
		this->mem_map.pop_back();
		this->size -= sizeof(_Block);
		return sizeof(_Block);
	}

	template <class _Block> //pop a [type] from the front of container
	size_t pop_front(_Block* rhs)
	{
		if (BEG->hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;
		check<_Block>();

		_Block *p = (_Block*)(BEG->copy_constructor(BEG->loc));
		*rhs = *p;
		delete p;
		delete BEG;
		this->mem_map.erase(BEG_ITER);
		this->size -= sizeof(_Block);
		return sizeof(_Block);
	}

	template <class _Block> //get a [type] from index [pos]
	size_t get(size_t pos, _Block* rhs)
	{
		check<_Block>();
		if (this->mem_map[pos]->hash_code != typeid(_Block).hash_code())
			throw MATCH_ERR;
		*rhs = *(_Block*)(this->mem_map[pos]->copy_constructor(mem_map[pos]->loc));
	}

	//get pointer to a copy of [type] given index [pos]
	inline void* operator[](size_t pos) { return(this->mem_map[pos]->copy_constructor(this->mem_map[pos]->loc)); }
	
	v_alloc& operator= (const v_alloc& rhs);

	//capacity shiz
	inline size_t get_size() { return this->mem_map.size(); }
	inline size_t byte_size() { return this->size; }
	inline size_t max_size() { return this->mem_map.max_size(); }
	inline size_t element_capcity() { return this->mem_map.capacity(); }
	inline bool is_empty() { return this->mem_map.empty(); }

	inline void resize(size_t size);

	inline void reserve(size_t size) { this->mem_map.reserve(size); }
	inline void shrink_to_fit() { this->mem_map.shrink_to_fit(); }
	
	template <class _Block>
	inline size_t front(_Block* rhs) { return this->get(this->mem_map.size() - 1, rhs); }

	template <class _Block>
	inline size_t back(_Block* rhs) { return this->get(0, rhs); }
};

#undef END 
#undef BEG
#undef MATCH_ERR
#endif
