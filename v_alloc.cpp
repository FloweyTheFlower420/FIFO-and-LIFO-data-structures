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

memblock_t::~memblock_t()
{
	if (this->loc != NULL && this->dest != NULL)
		this->dest(this->loc);
}

size_t v_alloc::pop_back_dump() 
{
	(this->mem_map.end() - 1)->dest((this->mem_map.end() - 1)->loc);
	this->mem_map.pop_back();
	return 0;
}

size_t v_alloc::pop_front_dump()
{
	(this->mem_map.begin() - 1)->dest((this->mem_map.begin() - 1)->loc);
	this->mem_map.erase(this->mem_map.begin());
	return 0;
}

v_alloc& v_alloc::operator= (v_alloc& rhs)
{
	this->mem_map = rhs.mem_map;
	return *this;
}

void v_alloc::resize(size_t size)
{
	if (this->mem_map.size() <= size)
		this->mem_map.resize(size);
	else
	{
		size_t pos = this->mem_map.size() - 1;
		while (pos > size)
		{
			this->pop_back_dump();
			pos--;
		}
		this->mem_map.resize(size);
	}
}