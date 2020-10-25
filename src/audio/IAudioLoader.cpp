#include "IAudioLoader.h"
#include "AudioLoaderWav.h"
#ifdef WITH_VORBIS
	#include "AudioLoaderOgg.h"
#endif
#include "FileSystem.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

IAudioLoader::IAudioLoader(const char *filename)
    : IAudioLoader(IFile::createFileHandle(filename))
{
	// Warning: Cannot call a virtual `init()` here, in the base constructor
}

IAudioLoader::IAudioLoader(nctl::UniquePtr<IFile> fileHandle)
    : fileHandle_(nctl::move(fileHandle)), bytesPerSample_(0), numChannels_(0), frequency_(0), numSamples_(0L), duration_(0.0f)
{
	// Warning: Cannot call a virtual `init()` here, in the base constructor
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

nctl::UniquePtr<IAudioLoader> IAudioLoader::createFromMemory(const char *bufferName, const unsigned char *bufferPtr, unsigned long int bufferSize)
{
	nctl::UniquePtr<IFile> fileHandle = IFile::createFromMemory(bufferName, bufferPtr, bufferSize);
	LOGI_X("Loading memory file: \"%s\" (0x%lx, %lu bytes)", fileHandle->filename(), bufferPtr, bufferSize);
	return createLoader(nctl::move(fileHandle), bufferName);
}

nctl::UniquePtr<IAudioLoader> IAudioLoader::createFromFile(const char *filename)
{
	// Creating a handle from IFile static method to detect assets file
	nctl::UniquePtr<IFile> fileHandle = IFile::createFileHandle(filename);
	LOGI_X("Loading file: \"%s\"", fileHandle->filename());
	return createLoader(nctl::move(fileHandle), filename);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

nctl::UniquePtr<IAudioLoader> IAudioLoader::createLoader(nctl::UniquePtr<IFile> fileHandle, const char *filename)
{
	if (fs::hasExtension(filename, "wav"))
		return nctl::makeUnique<AudioLoaderWav>(nctl::move(fileHandle));
#ifdef WITH_VORBIS
	else if (fs::hasExtension(filename, "ogg"))
		return nctl::makeUnique<AudioLoaderOgg>(nctl::move(fileHandle));
#endif
	else
	{
		LOGF_X("Extension unknown: \"%s\"", fs::extension(filename));
		fileHandle.reset(nullptr);
		exit(EXIT_FAILURE);
	}
}

}
