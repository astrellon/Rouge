#include "sfx_isound.h"

namespace am {
namespace sfx {

	const int ISound::LUA_ID = 0x30;
	const char *ISound::LUA_TABLENAME = "am_sfx_Sound";

	ISound::ISound() :
		mStream(false),
		mNumBuffers(0),
		mBufferTime(0),
		mBufferByteSize(0),
		mStreamSource(-1)
	{
	}
	ISound::~ISound()
	{
		if (mNumBuffers > 0)
		{
			alDeleteBuffers(mNumBuffers, mBuffers);
			mNumBuffers = 0;
		}
	}

	ALuint ISound::getNumBuffers() const
	{
		return mNumBuffers;
	}

	void ISound::update()
	{

	}

	ALuint ISound::getStreamSource() const
	{
		return mStreamSource;
	}

	bool ISound::isStream() const
	{
		return mStream;
	}

	ALuint ISound::getBufferTime() const
	{
		return mBufferByteSize;
	}

	ALuint ISound::getBufferByteSize()
	{
		return mBufferByteSize;
	}

	void ISound::attachSource(ALuint source)
	{
		if (source != -1)
		{
			alSourceQueueBuffers(source, mNumBuffersInUse, mBuffers);
		}
		if (mStream)
		{
			mStreamSource = source;
		}
	}
	void ISound::detachSource(ALuint source)
	{
		if (source != -1)
		{
			alSourceUnqueueBuffers(source, mNumBuffersInUse, mBuffers);
		}
		if (mStreamSource == source)
		{
			mStreamSource = -1;
		}
	}

	const char *ISound::getFilename() const
	{
		return mFilename.c_str();
	}

}
}
