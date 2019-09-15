#include "v_alloc.h"

void remove_memblock(memblock_t* mem)
{
	mem->dest(mem->loc);
}

memblock_t::memblock_t()
{
	this->loc = NULL;
	this->dest = NULL;
	this->copy_constructor = NULL;
	hash_code = 0;
}

memblock_t::memblock_t(const memblock_t& rhs)
{
	if (!(rhs.dest == NULL || rhs.loc == NULL))
	{
		this->loc = rhs.copy_constructor(rhs.loc);
		this->copy_constructor = rhs.copy_constructor;
		this->dest = rhs.dest;
		this->hash_code = rhs.hash_code;
	}
	else
	{
		this->loc = NULL;
		this->copy_constructor = rhs.copy_constructor;
		this->dest = rhs.dest;
		this->hash_code = rhs.hash_code;
	}
}

v_alloc::v_alloc()
{
	this->size = 0;
}

v_alloc::~v_alloc()
{
	size_t pos = 0;
	while (pos < this->mem_map.size())
	{
		remove_memblock(&this->mem_map[pos]);
		pos++;
	}

	this->mem_map.clear();
}

void* v_alloc::operator[](size_t pos)
{
	return(this->mem_map[pos].copy_constructor(this->mem_map[pos].loc));
}