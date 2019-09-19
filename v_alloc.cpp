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
	if (!(rhs.copy_constructor == NULL || rhs.loc == NULL))
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

memblock_t::~memblock_t()
{
	if (this->loc != NULL && this->dest != NULL)
		this->dest(this->loc);
	this->loc = NULL;
}

v_alloc& v_alloc::operator= (const v_alloc& rhs)
{
	size_t pos = 0;
	memblock_t* new_t;
	while (pos < rhs.mem_map.size())
	{
		new_t = new memblock_t(*rhs.mem_map[pos]);
		this->mem_map.push_back(new_t);
	}
	return *this;
}