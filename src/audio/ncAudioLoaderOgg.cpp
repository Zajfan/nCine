#include <cstdlib> // for exit()
#include <cstring>
#include "ncAudioLoaderOgg.h"
#include "ncServiceLocator.h"

#ifdef __ANDROID__
#include "ncAssetFile.h" // for ncAssetFile::sType()

size_t asset_read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	ncAssetFile *pAssetFile = static_cast<ncAssetFile *>(datasource);
	return pAssetFile->Read(ptr, size * nmemb);
}

int asset_seek(void *datasource, ogg_int64_t offset, int whence)
{
	ncAssetFile *pAssetFile = static_cast<ncAssetFile *>(datasource);
	return pAssetFile->Seek(offset, whence);
}

int asset_close(void *datasource)
{
	ncAssetFile *pAssetFile = static_cast<ncAssetFile *>(datasource);
	pAssetFile->Close();
	return 0;
}

long asset_tell(void *datasource)
{
	ncAssetFile *pAssetFile = static_cast<ncAssetFile *>(datasource);
	return pAssetFile->Tell();
}

ov_callbacks oggCallbacks = { asset_read, asset_seek, asset_close, asset_tell };
#endif

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

ncAudioLoaderOgg::ncAudioLoaderOgg(const char *pFilename)
	: ncIAudioLoader(pFilename)
{
	Init();
}

ncAudioLoaderOgg::ncAudioLoaderOgg(ncIFile *pFileHandle)
	: ncIAudioLoader(pFileHandle)
{
	Init();
}

ncAudioLoaderOgg::~ncAudioLoaderOgg()
{
	ov_clear(&m_oggFile);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

long ncAudioLoaderOgg::Read(char *pBuffer, int iBufSize) const
{
	static int iBitStream = 0;
	long lBytes;
	long int lBufSeek = 0;

	do
	{
		// Read up to a buffer's worth of decoded sound data
#ifdef __ANDROID__
		lBytes = ov_read(&m_oggFile, pBuffer + lBufSeek, iBufSize - lBufSeek, &iBitStream);
#else
		// 0 - little endian, 2 - 16bit, 1 - signed
		lBytes = ov_read(&m_oggFile, pBuffer + lBufSeek, iBufSize - lBufSeek, 0, 2, 1, &iBitStream);
#endif

		if (lBytes < 0)
		{
			ov_clear(&m_oggFile);
			ncServiceLocator::Logger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoaderOgg::Read - Error decoding at bitstream %d", iBitStream);
			exit(EXIT_FAILURE);
		}

		// Reset the static variable at the end of a decoding process
		if (lBytes <= 0)
		{
			iBitStream = 0;
		}

		lBufSeek += lBytes;
	}
	while (lBytes > 0 && iBufSize - lBufSeek > 0);

	return lBufSeek;
}

void ncAudioLoaderOgg::Rewind() const
{
	ov_raw_seek(&m_oggFile, 0);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void ncAudioLoaderOgg::Init()
{
	vorbis_info *pInfo;

	ncServiceLocator::Logger().Write(ncILogger::LOG_INFO, (const char *)"ncAudioLoaderOgg::Init - Loading \"%s\"", m_pFileHandle->Filename());

	// File is closed by ov_clear()
	m_pFileHandle->SetCloseOnExit(false);

#ifdef __ANDROID__
	if (m_pFileHandle->Type() == ncAssetFile::sType())
	{
		m_pFileHandle->Open(ncIFile::MODE_FD | ncIFile::MODE_READ);

		if (ov_open_callbacks(m_pFileHandle, &m_oggFile, NULL, 0, oggCallbacks) != 0)
		{
			ncServiceLocator::Logger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoaderOgg::Init - Cannot open \"%s\" with ov_open_callbacks()", m_pFileHandle->Filename());
			m_pFileHandle->Close();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		m_pFileHandle->Open(ncIFile::MODE_READ | ncIFile::MODE_BINARY);

		if (ov_open(m_pFileHandle->Ptr(), &m_oggFile, NULL, 0) != 0)
		{
			ncServiceLocator::Logger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoaderOgg::Init - Cannot open \"%s\" with ov_open()", m_pFileHandle->Filename());
			m_pFileHandle->Close();
			exit(EXIT_FAILURE);
		}
	}
#else
	if (ov_fopen(m_pFileHandle->Filename(), &m_oggFile) != 0)
	{
		ncServiceLocator::Logger().Write(ncILogger::LOG_FATAL, (const char *)"ncAudioLoaderOgg::Init - Cannot open \"%s\" with ov_fopen()", m_pFileHandle->Filename());
		exit(EXIT_FAILURE);
	}
#endif

	// Get some information about the OGG file
	pInfo = ov_info(&m_oggFile, -1);

	m_iBytesPerSample = 2; // Ogg is always 16 bits
	m_iChannels = pInfo->channels;
	m_iFrequency = pInfo->rate;

	m_ulNumSamples = ov_pcm_total(&m_oggFile, -1);
	m_fDuration = float(ov_time_total(&m_oggFile, -1));

	ncServiceLocator::Logger().Write(ncILogger::LOG_INFO, (const char *)"ncAudioLoaderOgg::Init - duration: %.2f, channels: %d, frequency: %d", m_fDuration, m_iChannels, m_iFrequency);
}
