#include "gtest_hashmaplist.h"
#include "test_movable.h"

namespace {

class HashMapListMovableTest : public ::testing::Test
{
  public:
	HashMapListMovableTest()
	    : hashmap_(Capacity) {}

  protected:
	nctl::HashMapList<int, Movable, nctl::FixedHashFunc<int>> hashmap_;
};

#if !TEST_MOVABLE_ONLY
TEST_F(HashMapListMovableTest, SubscriptLValue)
{
	Movable movable(Movable::Construction::INITIALIZED);

	ASSERT_EQ(hashmap_.find(0), nullptr);
	hashmap_[0] = movable;
	hashmap_[0].printAndAssert();

	ASSERT_NE(hashmap_.find(0), nullptr);
	ASSERT_EQ(movable.size(), hashmap_[0].size());
	ASSERT_NE(movable.data(), nullptr);
}
#endif

TEST_F(HashMapListMovableTest, SubscriptRValue)
{
	Movable movable(Movable::Construction::INITIALIZED);
	const unsigned int newSize = movable.size();
	const int *newData = movable.data();

	ASSERT_EQ(hashmap_.find(0), nullptr);
	hashmap_[0] = nctl::move(movable);
	hashmap_[0].printAndAssert();

	ASSERT_NE(hashmap_.find(0), nullptr);
	ASSERT_EQ(hashmap_[0].size(), newSize);
	ASSERT_EQ(hashmap_[0].data(), newData);
	ASSERT_EQ(movable.size(), 0);
	ASSERT_EQ(movable.data(), nullptr);
}

#if !TEST_MOVABLE_ONLY
TEST_F(HashMapListMovableTest, InsertLValue)
{
	Movable movable(Movable::Construction::INITIALIZED);

	ASSERT_EQ(hashmap_.find(0), nullptr);
	hashmap_.insert(0, movable);
	hashmap_[0].printAndAssert();

	ASSERT_NE(hashmap_.find(0), nullptr);
	ASSERT_EQ(movable.size(), hashmap_[0].size());
	ASSERT_NE(movable.data(), nullptr);
}
#endif

TEST_F(HashMapListMovableTest, InsertRValue)
{
	Movable movable(Movable::Construction::INITIALIZED);
	const unsigned int newSize = movable.size();
	const int *newData = movable.data();

	ASSERT_EQ(hashmap_.find(0), nullptr);
	hashmap_.insert(0, nctl::move(movable));
	hashmap_[0].printAndAssert();

	ASSERT_NE(hashmap_.find(0), nullptr);
	ASSERT_EQ(hashmap_[0].size(), newSize);
	ASSERT_EQ(hashmap_[0].data(), newData);
	ASSERT_EQ(movable.size(), 0);
	ASSERT_EQ(movable.data(), nullptr);
}

TEST_F(HashMapListMovableTest, Emplace)
{
	ASSERT_EQ(hashmap_.find(0), nullptr);
	hashmap_.emplace(0, Movable::Construction::INITIALIZED);
	hashmap_[0].printAndAssert();

	ASSERT_NE(hashmap_.find(0), nullptr);
}

TEST_F(HashMapListMovableTest, MoveConstruction)
{
	Movable movable(Movable::Construction::INITIALIZED);
	const unsigned int newSize = movable.size();
	const int *newData = movable.data();

	hashmap_[0] = nctl::move(movable);
	hashmap_[0].printAndAssert();
	printf("Creating a new hashmap with move construction\n");
	nctl::HashMapList<int, Movable, nctl::FixedHashFunc<int>> newHashmap(nctl::move(hashmap_));
	newHashmap[0].printAndAssert();

	ASSERT_EQ(newHashmap[0].size(), newSize);
	ASSERT_EQ(newHashmap[0].data(), newData);
}

TEST_F(HashMapListMovableTest, MoveAssignmentOperator)
{
	Movable movable(Movable::Construction::INITIALIZED);
	const unsigned int newSize = movable.size();
	const int *newData = movable.data();

	hashmap_[0] = nctl::move(movable);
	hashmap_[0].printAndAssert();
	printf("Creating a new hashmap with the move assignment operator\n");
	nctl::HashMapList<int, Movable, nctl::FixedHashFunc<int>> newHashmap(Capacity);
	newHashmap = nctl::move(hashmap_);
	newHashmap[0].printAndAssert();

	ASSERT_EQ(newHashmap[0].size(), newSize);
	ASSERT_EQ(newHashmap[0].data(), newData);
}

TEST_F(HashMapListMovableTest, Rehash)
{
	Movable movable(Movable::Construction::INITIALIZED);
	hashmap_[0] = nctl::move(movable);
	hashmap_[0].printAndAssert();
	const float loadFactor = hashmap_.loadFactor();
	printf("Original size: %u, capacity: %u, load factor: %f\n", hashmap_.size(), Capacity, hashmap_.loadFactor());

	printf("Doubling capacity by rehashing\n");
	hashmap_.rehash(Capacity * 2);
	printf("New size: %u,capacity: %u, load factor: %f\n", hashmap_.size(), Capacity * 2, hashmap_.loadFactor());

	ASSERT_EQ(hashmap_.size(), 1);
	ASSERT_FLOAT_EQ(hashmap_.loadFactor(), loadFactor * 0.5f);
}

}
