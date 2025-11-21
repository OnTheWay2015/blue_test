#include "stdafx.h"

#include <vector>


int com(const void* a, const void* b)
{
	const std::string* aa = (const std::string*)(a);
	const std::string* bb = (const std::string*)(b);
	return *aa < *bb;
}

void test_sort()
{
	std::vector<std::string> strs;
	strs.push_back("abc");
	strs.push_back("acc");


	std::qsort(&strs[0],strs.size(), sizeof(std::string), com);

}



