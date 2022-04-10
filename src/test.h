#pragma once
#include <stdio.h>

struct Tester
{
	Tester(int value) : value(value) {}
	Tester(const Tester& test) : value(test.value) { printf("Copy constructor\n"); }
	Tester(Tester&& test)
	{
		value = test.value;
		test.value = 0;
		printf("Move constructor\n");
	}
	~Tester()
	{
		printf("Destructor\n");
	}

	void operator=(const Tester& test) { printf("Copy operator\n"); }
	void operator=(Tester&& test) { printf("Move operator\n"); }

	int value = 0;
};

Tester test();