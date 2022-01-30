#ifndef CLASS_NCINE_RENDERBATCHER
#define CLASS_NCINE_RENDERBATCHER

#include <nctl/Array.h>
#include <nctl/UniquePtr.h>

namespace ncine {

class RenderCommand;

/// A class that batches render commands together
class RenderBatcher
{
  public:
	RenderBatcher();

	void collectInstances(const nctl::Array<RenderCommand *> &srcQueue, nctl::Array<RenderCommand *> &destQueue);
	void createBatches(const nctl::Array<RenderCommand *> &srcQueue, nctl::Array<RenderCommand *> &destQueue);
	void reset();

  private:
	static unsigned int UboMaxSize;

	struct ManagedBuffer
	{
		ManagedBuffer()
		    : size(0), freeSpace(0) {}

		unsigned int size;
		unsigned int freeSpace;
		nctl::UniquePtr<unsigned char[]> buffer;
	};

	/// Memory buffers to collect UBO data before committing it
	/*! \note It is a RAM buffer and cannot be handled by the `RenderBuffersManager` */
	nctl::Array<ManagedBuffer> buffers_;

	RenderCommand *collectCommands(nctl::Array<RenderCommand *>::ConstIterator start, nctl::Array<RenderCommand *>::ConstIterator end, nctl::Array<RenderCommand *>::ConstIterator &nextStart);

	unsigned char *acquireMemory(unsigned int bytes);
	void createBuffer(unsigned int size);
};

}

#endif
