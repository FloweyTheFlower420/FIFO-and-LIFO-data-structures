#ifndef _STACK_PMVA_H
#define _STACK_PMVA_H
#include "v_alloc.h"

class LIFO
{
	v_alloc allocator;

public:

	template <typename type>
	size_t push(type* rhs) { return this->allocator.push_back<type>(rhs); }

	template <class type>
	size_t pop(type* rhs) { return this->allocator.pop_back<type>(rhs); }
};


#endif // _STACK_PMVA_H
