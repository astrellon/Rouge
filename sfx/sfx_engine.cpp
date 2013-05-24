#include "sfx_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "sfx_source.h"
#include "sfx_sound_wav.h"
#include "sfx_sound_ogg.h"

#include "openal/aldlist.h"

#include <util/utils.h>

namespace am {
namespace sfx {

	SfxEngine *SfxEngine::sEngine = NULL;

	_DeviceName::_DeviceName(const char *name, bool defaultDevice) :
		deviceName(name),
		defaultDevice(defaultDevice)
	{
	}
	_SourceId::_SourceId() :
		id(-1),
		source(NULL)
	{
	}
	_SourceId::_SourceId(ALuint id) :
		id(id),
		source(NULL)
	{
	}
	unsigned int _SourceId::getPriority() const
	{
		if (source == NULL)
		{
			return 0u;
		}
		return source->getPriority();
	}

	SfxEngine::SfxEngine() :
		mInited(false),
		mSourcePoolPos(0),
		// OGG
		mOggHandle(NULL),
		mOggClear(NULL),
		mOggRead(NULL),
		mOggPcmTotal(NULL),
		mOggRawTotal(NULL),
		mOggInfo(NULL),
		mOggComment(NULL),
		mOggOpenCallbacks(NULL),
		mBackgroundMusic(NULL),
		mBackgroundMusicSource(NULL)
	{
	}

	void SfxEngine::init()
	{
		if (mInited)
		{
			return;
		}

		if (!initOgg())
		{
			throw("Unable to initialise OGG system");
			return;
		}
		ALDeviceList *pDeviceList = new ALDeviceList();
		if (pDeviceList && pDeviceList->GetNumDevices())
		{
			for (int i = 0; i < pDeviceList->GetNumDevices(); i++)
			{
				bool defaultDevice = i == pDeviceList->GetDefaultDevice();
				const char *deviceName = pDeviceList->GetDeviceName(i);
				mDeviceList.push_back(DeviceName(deviceName, defaultDevice));
				if (defaultDevice)
				{
					mDefaultDevice = deviceName;
				}
			}
		}

		mInited = true;
	}
	bool SfxEngine::hasInited() const
	{
		return mInited;
	}

	bool SfxEngine::createContext(int deviceIndex)
	{
		bool result = false;

		const char *deviceName = NULL;
		if (deviceIndex < 0 || deviceIndex >= static_cast<int>(mDeviceList.size()))
		{
			deviceName = mDefaultDevice.c_str();
		}
		else
		{
			deviceName = mDeviceList[deviceIndex].deviceName.c_str();
		}

		ALCdevice *device = alcOpenDevice(deviceName);
		if (device)
		{
			ALCcontext *pContext = alcCreateContext(device, NULL);
			if (pContext)
			{
				// TODO AM_LOG
				alcMakeContextCurrent(pContext);
				result = true;
			}
			else
			{
				alcCloseDevice(device);
			}
		}

		while (mSourcePool.size() < 32)
		{
			ALuint source = -1;
			alGenSources(1, &source);
			ALenum result = alGetError();
			if (result != AL_NO_ERROR)
			{
				// We're aiming for 32 sources, since that should be way more than what we need
				// however we can deal with situations where we are on hardware that doesn't
				// support that many.
				// Win7 with an integrated Realtek showed 256 sources.
				break;
			}
			mSourcePool.push_back(SourceId(source));
		}

		mBackgroundMusicSource = new Source();
		// Background music is always played at the same location as the listener.
		mBackgroundMusicSource->setSourceRelative(true);
		mBackgroundMusicSource->retain();

		return result;
	}

	const SfxEngine::DeviceList &SfxEngine::getDeviceList()
	{
		return mDeviceList;
	}
	
	void SfxEngine::deinit()
	{
		if (!mInited)
		{
			return;
		}
		ALCcontext *context = alcGetCurrentContext();
		ALCdevice *device = alcGetContextsDevice(context);
	
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);

		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			alDeleteSources(1, &mSourcePool[i].id);
		}
		mSourcePool.clear();

		if (mBackgroundMusicSource)
		{
			mBackgroundMusicSource->release();
			mBackgroundMusicSource = NULL;
		}
		if (mBackgroundMusic)
		{
			mBackgroundMusic->release();
			mBackgroundMusic = NULL;
		}

		mInited = false;
	}

	ISound *SfxEngine::loadSound(const char *filename)
	{
		string ext = getExtension(filename);
		if (ext.compare("wav") == 0)
		{
			return new SoundWav(filename);
		}
		else if (ext.compare("ogg") == 0)
		{
			return new SoundOgg(filename);
		}
		return NULL;
	}
	ISound *SfxEngine::loadStream(const char *filename, int numBuffers)
	{
		string ext = getExtension(filename);
		if (ext.compare("wav") == 0)
		{
			SoundWav *wav = new SoundWav();
			if (wav->loadStream(filename, numBuffers))
			{
				return wav;
			}
			delete wav;
		}
		else if (ext.compare("ogg") == 0)
		{
			SoundOgg *ogg = new SoundOgg();
			if (ogg->loadStream(filename, numBuffers))
			{
				return ogg;
			}
			delete ogg;
		}
		return NULL;
	}

	void SfxEngine::displayInUse()
	{
		printf("In use: ");
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].source)
			{
				printf("%d ", i);
			}
		}
		printf("\n");
	}

	void SfxEngine::update()
	{
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].source)
			{
				mSourcePool[i].source->update();
			}
		}
	}

	void SfxEngine::setBackgroundMusic(ISound *sound)
	{
		if (mBackgroundMusic)
		{
			mBackgroundMusic->release();
			mBackgroundMusicSource->stop();
		}
		mBackgroundMusic = sound;
		if (mBackgroundMusic)
		{
			mBackgroundMusic->retain();
		}
		mBackgroundMusicSource->setSound(mBackgroundMusic);
		mBackgroundMusicSource->play();
	}
	ISound *SfxEngine::getBackgroundMusic() const
	{
		return mBackgroundMusic;
	}

	Listener &SfxEngine::getListener()
	{
		return mListener;
	}

	int SfxEngine::getNumSources() const
	{
		return static_cast<int>(mSourcePool.size());
	}

	bool SfxEngine::getSource(ALuint &result, Source *forSource)
	{
		if (mSourcePool.empty())
		{
			return false;
		}
		int poolIndex = nextPoolIndex();
		result = mSourcePool[mSourcePoolPos].id;
		mSourcePool[mSourcePoolPos].source = forSource;
		return true;
	}
	void SfxEngine::releaseSource(ALuint source)
	{
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].id == source)
			{
				mSourcePool[i].source = NULL;
				break;
			}
		}
	}

	SfxEngine *SfxEngine::getEngine()
	{
		if (sEngine == NULL)
		{
			sEngine = new SfxEngine();
		}
		return sEngine;
	}
	void SfxEngine::deinitSfxEngine()
	{
		if (sEngine)
		{
			sEngine->deinit();
			delete sEngine;
			sEngine = NULL;
		}
	}

	void SfxEngine::checkError(const char *message, const char *filename)
	{
		const char *error = getErrorCode(alGetError());
		if (error)
		{
			stringstream ss;
			if (filename)
			{
				ss << filename << ": ";
			}
			if (message)
			{
				ss << message << ": ";
			}
			ss << error;
			throw(ss.str().c_str());
		}
	}

	const char *SfxEngine::getErrorCode(ALenum errorCode)
	{
		switch (errorCode)
		{
		case AL_INVALID_NAME:
			return "Invalid name";
		case AL_INVALID_ENUM:
			return "Invalid enum";
		case AL_INVALID_VALUE:
			return "Invalid value";
		case AL_INVALID_OPERATION:
			return "Invalid operation";
		case AL_OUT_OF_MEMORY:
			return "Out of memory";
		default:
			return NULL;
		}
	}

	int SfxEngine::nextPoolIndex()
	{
		int initialPos = mSourcePoolPos;
		unsigned int findPriority = 0;
		while (mSourcePool[mSourcePoolPos].getPriority() > findPriority)
		{
			mSourcePoolPos++;
			if (mSourcePoolPos >= mSourcePool.size())
			{
				mSourcePoolPos = 0;
			}
			// If the position is the same, then we've looped around
			// the pool, so find a higher priority sound.
			if (mSourcePoolPos == initialPos)
			{
				findPriority++;
			}
		}
		return mSourcePoolPos;
	}

	// OGG Functions
	SfxEngine::OggClear SfxEngine::getOggClear() const
	{
		return mOggClear;
	}
	SfxEngine::OggRead SfxEngine::getOggRead() const
	{
		return mOggRead;
	}
	SfxEngine::OggPcmTotal SfxEngine::getOggPcmTotal() const
	{
		return mOggPcmTotal;
	}
	SfxEngine::OggRawTotal SfxEngine::getOggRawTotal() const
	{
		return mOggRawTotal;
	}
	SfxEngine::OggInfo SfxEngine::getOggInfo() const
	{
		return mOggInfo;
	}
	SfxEngine::OggComment SfxEngine::getOggComment() const
	{
		return mOggComment;
	}
	SfxEngine::OggOpenCallbacks SfxEngine::getOggOpenCallbacks() const
	{
		return mOggOpenCallbacks;
	}

	bool SfxEngine::initOgg()
	{
		// Try and load Vorbis DLLs (VorbisFile.dll will load ogg.dll and vorbis.dll)
		mOggHandle = LoadLibrary("vorbisfile.dll");
		if (mOggHandle)
		{
			mOggClear = (OggClear)GetProcAddress(mOggHandle, "ov_clear");
			mOggRead = (OggRead)GetProcAddress(mOggHandle, "ov_read");
			mOggPcmTotal = (OggPcmTotal)GetProcAddress(mOggHandle, "ov_pcm_total");
			mOggRawTotal = (OggRawTotal)GetProcAddress(mOggHandle, "ov_raw_total");
			mOggInfo = (OggInfo)GetProcAddress(mOggHandle, "ov_info");
			mOggComment = (OggComment)GetProcAddress(mOggHandle, "ov_comment");
			mOggOpenCallbacks = (OggOpenCallbacks)GetProcAddress(mOggHandle, "ov_open_callbacks");

			if (mOggClear && mOggRead && mOggPcmTotal && mOggInfo &&
				mOggRawTotal && mOggComment && mOggOpenCallbacks)
			{
				return true;
			}
		}
		return false;
	}
	void SfxEngine::deinitOgg()
	{
		if (mOggHandle)
		{
			FreeLibrary(mOggHandle);
			mOggHandle = NULL;
			mOggClear = NULL;
			mOggRead = NULL;
			mOggPcmTotal = NULL;
			mOggInfo = NULL;
			mOggComment = NULL;
			mOggOpenCallbacks = NULL;
		}
	}

	string SfxEngine::getExtension(const char *filename)
	{
		if (filename == NULL)
		{
			return "";
		}
		size_t len = strlen(filename);
		size_t i = len - 1;
		for (; i != -1; i--)
		{
			if (filename[i] == '.')
			{
				return am::util::Utils::toLowerCase(filename + i + 1);
			}
		}
		return "";
	}

}
}
