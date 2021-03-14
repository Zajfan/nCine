#ifndef GTEST_HASHMAP_CSTRING_H
#define GTEST_HASHMAP_CSTRING_H

#include <nctl/HashMap.h>
#include <nctl/HashMapIterator.h>
#include <nctl/String.h>
#include "gtest/gtest.h"

namespace {

const unsigned int Capacity = 32;
const unsigned int Size = 6;
const char *Keys[Size] = { "A", "a", "B", "C", "AB", "BA" };
const char *Values[Size] = { "AAAA", "aaaa", "BBBB", "CCCC", "ABABABAB", "BABABABA" };
/// A new set of C-style string keys, same in content but different in memory address
const unsigned int MaxLength = 3;
char KeysCopy[Size][MaxLength];

void initHashMap(nctl::HashMap<const char *, const char *> &cstrHashmap)
{
	for (unsigned int i = 0; i < Size; i++)
	{
		cstrHashmap[Keys[i]] = Values[i];
		strncpy(KeysCopy[i], Keys[i], 5);
	}
}

void printHashMap(nctl::HashMap<const char *, const char *> &cstrHashmap)
{
	unsigned int n = 0;

	for (nctl::HashMap<const char *, const char *>::ConstIterator i = cstrHashmap.begin(); i != cstrHashmap.end(); ++i)
		printf("[%u] hash: %u, key: %s, value: %s\n", n++, i.hash(), i.key(), i.value());
	printf("\n");
}

unsigned int calcSize(const nctl::HashMap<const char *, const char *> &cstrHashmap)
{
	unsigned int length = 0;

	for (typename nctl::HashMap<const char *, const char *>::ConstIterator i = cstrHashmap.begin(); i != cstrHashmap.end(); ++i)
		length++;

	return length;
}

}

#endif
