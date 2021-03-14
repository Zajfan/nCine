#ifndef GTEST_HASHSETLIST_STRING_H
#define GTEST_HASHSETLIST_STRING_H

#include <nctl/HashSetList.h>
#include <nctl/HashSetListIterator.h>
#include <nctl/String.h>
#include "gtest/gtest.h"

namespace {

const unsigned int Capacity = 32;
const unsigned int Size = 6;
const char *Keys[Size] = { "A", "a", "B", "C", "AB", "BA" };

void initHashSet(nctl::HashSetList<nctl::String> &strHashset)
{
	for (unsigned int i = 0; i < Size; i++)
		strHashset.insert(Keys[i]);
}

void printHashSet(nctl::HashSetList<nctl::String> &strHashset)
{
	unsigned int n = 0;

	for (nctl::HashSetList<nctl::String>::ConstIterator i = strHashset.begin(); i != strHashset.end(); ++i)
		printf("[%u] hash: %u, key: %s\n", n++, i.hash(), i.key().data());
	printf("\n");
}

unsigned int calcSize(const nctl::HashSetList<nctl::String> &strHashset)
{
	unsigned int length = 0;

	for (typename nctl::HashSetList<nctl::String>::ConstIterator i = strHashset.begin(); i != strHashset.end(); ++i)
		length++;

	return length;
}

void assertHashSetsAreEqual(const nctl::HashSetList<nctl::String> &strHashset1, const nctl::HashSetList<nctl::String> &strHashset2)
{
	nctl::HashSetList<nctl::String>::ConstIterator strHashset1It = strHashset1.begin();
	nctl::HashSetList<nctl::String>::ConstIterator strHashset2It = strHashset2.begin();
	while (strHashset1It != strHashset1.end())
	{
		ASSERT_EQ(strHashset1It.key(), strHashset2It.key());

		strHashset1It++;
		strHashset2It++;
	}
}

}

#endif
