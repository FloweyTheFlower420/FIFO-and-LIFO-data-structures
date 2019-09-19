#include <iostream>
#include "stack.h"
#include "queue.h"
//test code

class foo {
public:
	foo() {};
	foo(const foo& rhs) {
		std::cout << "hi from foo! (cc invoked)\n";
	}
	~foo()
	{
		std::cout << "bye from foo! (dest invoked)\n";
	}
};

class bar {
public:
	bar() {};
	bar(const bar& rhs) {
		std::cout << "hi from bar! (cc invoked)\n";
	}
	~bar()
	{
		std::cout << "bye from bar! (dest invoked)\n";
	}
};

int main()
{
	LIFO stack;
	FIFO queue;
	foo* test_0 = new foo();
	bar* test_1 = new bar();
	std::cout << "pushing...\n\n";
	stack.push(test_0);
	stack.push(test_1);

	std::cout << "\npopping...\n\n";


	stack.pop(test_1);
	stack.pop(test_0);
	std::cout << "\nenqueue...\n\n";

	queue.enqueue(test_0);
	queue.enqueue(test_1);

	std::cout << "\ndequeue...\n\n";

	queue.dequeue(test_0);
	queue.dequeue(test_1);

	std::cout << "\ndeleteing allocated memory\n\n";
	delete test_0;
	delete test_1;
	return 0;
}


