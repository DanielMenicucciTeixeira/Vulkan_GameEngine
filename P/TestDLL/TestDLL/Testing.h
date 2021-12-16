#pragma once

#include <cstdlib>

#define TEST_API __declspec(dllexport)

extern "C" TEST_API float Random();

extern "C" TEST_API void Sort(int a[], int length);

extern "C" TEST_API const char* SendHello();

extern "C" TEST_API const char* MakeString(const char* s1, const char* s2);

extern "C" TEST_API float Move();