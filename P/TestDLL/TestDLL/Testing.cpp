#include "pch.h"
#include "Testing.h"
#include <algorithm>
#include <string>

TEST_API float Random() {
	//return number between 0 and 9
	return (float) (rand() % 10);
}

TEST_API void Sort(int a[], int length) {
	std::sort(a, a + length);
}

TEST_API const char* SendHello() {
	return "hello world";
}

TEST_API const char* MakeString(const char* s1, const char* s2) {
	//make tmp literal string so arguments are not overridden
	std::string* tmp = nullptr;
	if(s1 == nullptr) {
		return s2;
	} else if(s2 == nullptr) {
		return s1;
	} else {
		tmp->push_back(*s1);
		tmp->push_back(*s2);
		return (const char*) tmp;
	}
}

TEST_API float Move() {
	return 5.0f;
}
