#include "sfx_sound_ogg.h"

#include "sfx_engine.h"

namespace am {
namespace sfx {

	ov_callbacks SoundOgg::sOggCallbacks;
	bool SoundOgg::sOggCallbacksInited = false;

	SoundOgg::SoundOgg() :
		ISound(),
		mOggFile(NULL),
		mData(NULL)
	{
		initCallbacks();
	}
	SoundOgg::SoundOgg(const char *filename, bool toStream) :
		ISound(),
		mOggFile(NULL),
		mData(NULL)
	{
		if (toStream)
		{
			loadStream(filename);
		}
		else
		{
			loadSound(filename);
		}
	}
	SoundOgg::~SoundOgg()
	{
		if (mOggFile)
		{
			SfxEngine::getEngine()->getOggClear()(mOggFile);
			delete mOggFile;
			mOggFile = NULL;
		}
		if (mData)
		{
			delete mData;
			mData = NULL;
		}
	}

	bool SoundOgg::loadSound(const char *filename)
	{
		SfxEngine *engine = SfxEngine::getEngine();
		FILE *file = fopen(filename, "rb");
		if (!file)
		{
			return false;
		}

		mNumBuffers = 1;
		mNumBuffersInUse = 1;
		mBuffers = new ALuint[1];
		alGenBuffers(1, mBuffers);
		mOggFile = new OggVorbis_File();
		mStream = false;

		if (parseHeaders(file, engine))
		{
			// TODO Determine exact reason for *4.
			// Current hypothesis is that it's the number of samples, so to translate
			// that to bytes you times by the number of channels times the size of a short (16bit).
			// But this is unconfirmed.
			mBufferByteSize = static_cast<ALuint>(engine->getOggPcmTotal()(mOggFile, -1)) * 4;
			mData = new char[mBufferByteSize];
			if (!mData)
			{
				throw ("Unable to create data buffer");
			}
			fillBuffer(mBuffers[0]);

			delete mData;
			mData = NULL;

			delete mOggFile;
			mOggFile = NULL;

			fclose(file);
			return true;
		}
		return false;
	}
	bool SoundOgg::loadStream(const char *filename, int numBuffers)
	{
		SfxEngine *engine = SfxEngine::getEngine();
		FILE *file = fopen(filename, "rb");
		if (!file)
		{
			return false;
		}

		mNumBuffers = numBuffers;
		mNumBuffersInUse = 1;
		mBuffers = new ALuint[numBuffers];
		alGenBuffers(numBuffers, mBuffers);
		SfxEngine::checkError();
		mStream = true;

		mOggFile = new OggVorbis_File();

		if (parseHeaders(file, engine))
		{
			if (mChannels == 1)
			{
				mBufferByteSize = mFrequency >> 1;
				mBufferByteSize -= mBufferByteSize % 2;
			}
			else
			{
				mBufferByteSize = mFrequency * (mChannels >> 1);
				mBufferByteSize -= mBufferByteSize % (mChannels << 1);
			}

			mData = new char[mBufferByteSize];
			if (!mData)
			{
				throw ("Unable to allocate data buffer");
			}
			SfxEngine::checkError();
			mNumBuffersInUse = numBuffers;
			for (int i = 0; i < numBuffers; i++)
			{
				if (!fillBuffer(mBuffers[i]))
				{
					mNumBuffersInUse = i;
					break;
				}
			}
			SfxEngine::checkError();
			return true;
		}
		return false;
	}

	void SoundOgg::update()
	{
		if (!mStream || (mStream && mStreamSource == -1))
		{
			return;
		}
		ALint buffersProcessed = 0;
		alGetSourcei(mStreamSource, AL_BUFFERS_PROCESSED, &buffersProcessed);

		mNumBuffersInUse -= buffersProcessed;
		// For each processed buffer, remove it from the Source Queue, read next chunk of audio
		// data from disk, fill buffer with new data, and add it to the Source Queue
		while (buffersProcessed)
		{
			// Remove the Buffer from the Queue.  (uiBuffer contains the Buffer ID for the unqueued Buffer)
			ALuint buffer = 0;
			alSourceUnqueueBuffers(mStreamSource, 1, &buffer);

			// Read more audio data (if there is any)
			//pWaveLoader->ReadWaveData(WaveID, pData, ulBufferSize, &ulBytesWritten);
			if (fillBuffer(buffer))
			{
				// Queue Buffer on the Source
				alSourceQueueBuffers(mStreamSource, 1, &buffer);
				mNumBuffersInUse++;
			}

			buffersProcessed--;
		}
	}

	void SoundOgg::setBufferTime(ALuint ms)
	{
	}

	bool SoundOgg::getALFormat(long numChannels, ALenum &format)
	{
		if (numChannels == 1)
		{
			format = AL_FORMAT_MONO16;
			return true;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			//ulBufferSize = ulFrequency >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			//ulBufferSize -= (ulBufferSize % 2);
		}
		else if (numChannels == 2)
		{
			format = AL_FORMAT_STEREO16;
			return true;
			// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
			//ulBufferSize = ulFrequency;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			//ulBufferSize -= (ulBufferSize % 4);
		}
		else if (numChannels == 4)
		{
			format = alGetEnumValue("AL_FORMAT_QUAD16");
			return true;
			// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
			//ulBufferSize = ulFrequency * 2;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			//ulBufferSize -= (ulBufferSize % 8);
		}
		else if (numChannels == 6)
		{
			format = alGetEnumValue("AL_FORMAT_51CHN16");
			return true;
			// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
			//ulBufferSize = ulFrequency * 3;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			//ulBufferSize -= (ulBufferSize % 12);
		}
		return false;
	}

	unsigned long SoundOgg::decodeOggVorbis(char *pDecodeBuffer, unsigned long ulBufferSize)
	{
		int current_section;
		long lDecodeSize;
		unsigned long ulSamples;
		short *pSamples;

		unsigned long ulBytesDone = 0;
		SfxEngine::OggRead oggRead = SfxEngine::getEngine()->getOggRead();
		while (1)
		{
			lDecodeSize = oggRead(mOggFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
			if (lDecodeSize > 0)
			{
				ulBytesDone += lDecodeSize;

				if (ulBytesDone >= ulBufferSize)
					break;
			}
			else
			{
				break;
			}
		}

		// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
		// however 6-Channels files need to be re-ordered
		if (mChannels == 6)
		{		
			pSamples = (short*)pDecodeBuffer;
			for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6)
			{
				// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
				// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
				swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
				swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
				swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
			}
		}

		return ulBytesDone;
	}

	bool SoundOgg::parseHeaders(FILE *file, SfxEngine *engine)
	{
		vorbis_info	*psVorbisInfo;
		if (engine->getOggOpenCallbacks()(file, mOggFile, NULL, 0, sOggCallbacks) == 0)
		{
			psVorbisInfo = engine->getOggInfo()(mOggFile, -1);
			
			if (psVorbisInfo)
			{
				mFrequency = psVorbisInfo->rate;
				mChannels = psVorbisInfo->channels;
				
				bool formatResult = getALFormat(mChannels, mALFormat);
				if (formatResult)
				{
					return true;
				}
			}
		}
		return false;
	}

	void SoundOgg::swap(short &s1, short &s2)
	{
		short sTemp = s1;
		s1 = s2;
		s2 = sTemp;
	}

	bool SoundOgg::fillBuffer(ALuint bufferId)
	{
		unsigned long bytesRead = decodeOggVorbis(mData, mBufferByteSize);
		if (bytesRead == 0)
		{
			return false;
		}

		alBufferData(bufferId, mALFormat, mData, bytesRead, mFrequency);
		return true;
	}

	size_t SoundOgg::ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		return fread(ptr, size, nmemb, (FILE*)datasource);
	}

	int SoundOgg::ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
	{
		return fseek((FILE*)datasource, (long)offset, whence);
	}

	int SoundOgg::ov_close_func(void *datasource)
	{
		return fclose((FILE*)datasource);
	}

	long SoundOgg::ov_tell_func(void *datasource)
	{
		return ftell((FILE*)datasource);
	}

	void SoundOgg::initCallbacks()
	{
		if (sOggCallbacksInited)
		{
			return;
		}
		sOggCallbacks.close_func = ov_close_func;
		sOggCallbacks.read_func = ov_read_func;
		sOggCallbacks.seek_func = ov_seek_func;
		sOggCallbacks.tell_func = ov_tell_func;
		sOggCallbacksInited = true;
	}
}
}
