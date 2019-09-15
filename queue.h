#ifndef _QUEUE_PMVA_H
#define _QUEUE_PMVA_H
#include "v_alloc.h"

class FIFO
{
	v_alloc allocator;

public:
	template <typename type>
	size_t enqueue(type* rhs) { return allocator.push_back(rhs); }

	template <typename type>
	size_t dequeue(type* rhs) { return allocator.pop_front(rhs); }
};

class DE_FIFO
{
	v_alloc allocator;

	template <typename type>
	size_t push_back(type* rhs) { return allocator.push_back(rhs); }

	template <typename type>
	size_t push_front(type* rhs) { return allocator.push_front(rhs); }

	template <typename type>
	size_t pop_back(type* rhs) { return allocator.pop_back(rhs); }

	template <typename type>
	size_t pop_front(type* rhs) { return allocator.pop_front(rhs); }
};

#endif // !_QUEUE_PMVA_H
