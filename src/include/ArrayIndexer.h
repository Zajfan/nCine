#ifndef CLASS_NCINE_ARRAYINDEXER
#define CLASS_NCINE_ARRAYINDEXER

#include "IIndexer.h"
#include <nctl/Array.h>
#include "Object.h"

namespace ncine {

/// Keeps track of allocated objects in a growing only array
class ArrayIndexer : public IIndexer
{
  public:
	ArrayIndexer();
	~ArrayIndexer() override;

	unsigned int addObject(Object *object) override;
	bool removeObject(unsigned int id) override;

	Object *object(unsigned int id) const override;
	bool setObject(unsigned int id, Object *object) override;

	bool isEmpty() const override { return numObjects_ == 0; }
	unsigned int size() const override { return numObjects_; }

	void logReport() const override;

  private:
	unsigned int numObjects_;
	unsigned int nextId_;
	nctl::Array<Object *> pointers_;

	/// Deleted copy constructor
	ArrayIndexer(const ArrayIndexer &) = delete;
	/// Deleted assignment operator
	ArrayIndexer &operator=(const ArrayIndexer &) = delete;
};

}

#endif
