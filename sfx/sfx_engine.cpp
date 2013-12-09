#include "sfx_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cstring>

#include "sfx_isource.h"
#include "sfx_source_point.h"
#include "sfx_sound_wav.h"
#include "sfx_sound_ogg.h"

#include <sfx/openal/aldlist.h>

#include <log/logger.h>

#include <util/utils.h>

namespace am {
namespace sfx {

	SfxEngine *SfxEngine::sEngine = nullptr;

	_DeviceName::_DeviceName(const char *name, bool defaultDevice) :
		deviceName(name),
		defaultDevice(defaultDevice)
	{
	}
	_SourceId::_SourceId() :
		id(-1),
		source(nullptr)
	{
	}
	_SourceId::_SourceId(ALuint id) :
		id(id),
		source(nullptr)
	{
	}
	bool _SourceId::isStreaming() const
	{
		if (source == nullptr)
		{
			return false;
		}
		return source->getSound()->isStream();
	}
	float _SourceId::getGain() const
	{
		if (source == nullptr)
		{
			return -1.0f;
		}
		return source->calcGain();
	}

	SfxEngine::SfxEngine() :
		mInited(false),
		mSourcePoolPos(0),
		// OGG
        mOggLoaded(false),
		mOggHandle(nullptr),
		mOggClear(nullptr),
		mOggRead(nullptr),
		mOggPcmTotal(nullptr),
		mOggRawTotal(nullptr),
		mOggInfo(nullptr),
		mOggComment(nullptr),
		mOggOpenCallbacks(nullptr),
		mBackgroundMusic(nullptr),
		mBackgroundMusicSource(nullptr)
	{
		mListener.setEnabled(true);
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
		::ALDeviceList *pDeviceList = new ::ALDeviceList();
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

		const char *deviceName = nullptr;
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
			ALCcontext *pContext = alcCreateContext(device, nullptr);
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
				// Also the number of sources we can have doesn't directly relate to the number of 
				// sources that we can play at once.
				break;
			}
			mSourcePool.push_back(SourceId(source));
		}

		mBackgroundMusicSource = new SourcePoint();
		// Background music is always played at the same location as the listener.
		mBackgroundMusicSource->setSourceRelative(true);

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
	
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);

		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			alDeleteSources(1, &mSourcePool[i].id);
		}
		mSourcePool.clear();

		if (mBackgroundMusicSource)
		{
			mBackgroundMusicSource = nullptr;
		}
		if (mBackgroundMusic)
		{
			mBackgroundMusic = nullptr;
		}

		mInited = false;
	}

	ISound *SfxEngine::loadSound(const char *filename)
	{
		std::string fileStr(filename);

		auto find = mSoundMap.find(fileStr);
		if (find != mSoundMap.end())
		{
			return find->second;
		}

		std::stringstream ss;
		if (fileStr[0] == '/')
		{
			ss << "data" << fileStr;
		}
		else
		{
			ss << "data/sounds/" << fileStr;
		}
		
		std::string ext = getExtension(fileStr.c_str());
		try
		{
			ISound *result = nullptr;
			if (ext.compare("wav") == 0)
			{
				result = new SoundWav(ss.str().c_str());
			}
			else if (ext.compare("ogg") == 0)
			{
				result = new SoundOgg(ss.str().c_str());
			}
			if (result)
			{
				mSoundMap[fileStr] = result;
			}
			return result;
		}
		catch (const char *error)
		{
			std::stringstream ss;
			ss << "Error loading sound '" << filename << "': " << error;
			am_log("SFXERR", ss);
		}
		return nullptr;
	}
	ISound *SfxEngine::loadStream(const char *filename, int numBuffers)
	{
		std::string fileStr(filename);
		std::stringstream ss;
		if (fileStr[0] == '/')
		{
			ss << "data" << fileStr;
		}
		else
		{
			ss << "data/sounds/" << fileStr;
		}

		std::string ext = getExtension(filename);
		if (ext.compare("wav") == 0)
		{
			SoundWav *wav = new SoundWav();
			if (wav->loadStream(ss.str().c_str(), numBuffers))
			{
				return wav;
			}
			delete wav;
		}
		else if (ext.compare("ogg") == 0)
		{
			SoundOgg *ogg = new SoundOgg();
			if (ogg->loadStream(ss.str().c_str(), numBuffers))
			{
				return ogg;
			}
			delete ogg;
		}
		return nullptr;
	}

	bool SfxEngine::oggLoaded() const
	{
		return mOggLoaded;
	}

	void SfxEngine::displayInUse()
	{
		printf("In use: ");
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].source)
			{
				printf("%u ", i);
			}
		}
		printf("\n");
	}

	void SfxEngine::update()
	{
		for (size_t i = 0; i < mInactiveSources.size(); i++)
		{
			mInactiveSources[i]->recalc();
			if (!mInactiveSources[i]->isOutOfRange())
			{
				mInactiveSources[i]->play();
				mInactiveSources.erase(mInactiveSources.begin() + i);
				i--;
			}
		}
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].source)
			{
				mSourcePool[i].source->recalc();
				mSourcePool[i].source->update();
			}
		}
	}

	void SfxEngine::setBackgroundMusic(ISound *sound)
	{
		if (mBackgroundMusic)
		{
			mBackgroundMusicSource->stop();
		}
		mBackgroundMusic = sound;
		mBackgroundMusicSource->setSound(mBackgroundMusic);
		mBackgroundMusicSource->play();
	}
	void SfxEngine::setBackgroundMusic(const char *filename)
	{
		ISound *bgm = loadStream(filename);
		if (bgm)
		{
			setBackgroundMusic(bgm);
		}
	}
	ISound *SfxEngine::getBackgroundMusic() const
	{
		return mBackgroundMusic;
	}

	Listener &SfxEngine::getListener()
	{
		return mListener;
	}

	void SfxEngine::addInactiveSource(ISource *source)
	{
		if (!source)
		{
			return;
		}
		size_t index = findInactiveSource(source);
		if (index != -1)
		{
			return;
		}
		mInactiveSources.push_back(source);
	}
	void SfxEngine::removeInactiveSource(ISource *source)
	{
		if (!source)
		{
			return;
		}
		size_t index = findInactiveSource(source);
		if (index == -1)
		{
			return;
		}
		mInactiveSources.erase(mInactiveSources.begin() + index);
	}
	size_t SfxEngine::findInactiveSource(ISource *source)
	{
		if (!source)
		{
			return -1;
		}
		for (size_t i = 0; i < mInactiveSources.size(); i++)
		{
			if (mInactiveSources[i].get() == source)
			{
				return i;
			}
		}
		return -1;
	}

	int SfxEngine::getNumSources() const
	{
		return static_cast<int>(mSourcePool.size());
	}

	bool SfxEngine::getSource(ALuint &result, ISource *forSource)
	{
		if (mSourcePool.empty())
		{
			return false;
		}
		int poolIndex = nextPoolIndex();
		if (mSourcePool[poolIndex].source != nullptr)
		{
			mSourcePool[poolIndex].source->stopOutOfRange();
		}
		result = mSourcePool[poolIndex].id;
		mSourcePool[poolIndex].source = forSource;
		return true;
	}
	void SfxEngine::releaseSource(ALuint source)
	{
		for (size_t i = 0; i < mSourcePool.size(); i++)
		{
			if (mSourcePool[i].id == source)
			{
				mSourcePool[i].source = nullptr;
				break;
			}
		}
	}

	SfxEngine *SfxEngine::getEngine()
	{
		if (sEngine == nullptr)
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
			sEngine = nullptr;
		}
	}

	void SfxEngine::checkError(const char *message, const char *filename)
	{
		const char *error = getErrorCode(alGetError());
		if (error)
		{
			std::stringstream ss;
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
			return nullptr;
		}
	}

	int SfxEngine::nextPoolIndex()
	{
		int initialPos = mSourcePoolPos;
		// Look for any unused sources
		while (mSourcePool[mSourcePoolPos].source != nullptr)
		{
			mSourcePoolPos++;
			if (mSourcePoolPos >= mSourcePool.size())
			{
				mSourcePoolPos = 0;
			}
			if (mSourcePoolPos == initialPos)
			{
				break;
			}
		}
		if (mSourcePool[mSourcePoolPos].source == nullptr)
		{
			return mSourcePoolPos;
		}
		// All sources are in use, so lets look for the quietest
		// non-streaming source.
		float currentGain = 0.0f;
		int pos = -1;
		for (int i = 0; i < mSourcePool.size(); i++)
		{
			if (!mSourcePool[i].isStreaming())
			{
				float gain = mSourcePool[i].getGain();
				if (gain < currentGain || pos < 0)
				{
					currentGain = gain;
					pos = i;
				}
			}
		}
		// Now we should have the quiest non-streaming sound source that we can use.
		mSourcePoolPos = pos;
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
#ifdef _ENABLE_OGG
#	ifdef _WIN32
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
                mOggLoaded = true;
				return true;
			}
		}
#	else
        mOggClear = ov_clear;
        mOggRead = ov_read;
        mOggPcmTotal = ov_pcm_total;
        mOggRawTotal = ov_raw_total;
        mOggInfo = ov_info;
        mOggComment = ov_comment;
        mOggOpenCallbacks = ov_open_callbacks;
        printf("Ogg clear? %d\n", mOggClear == nullptr);
        mOggLoaded = true;
        return true;
#   endif
#endif
		return false;
	}
	void SfxEngine::deinitOgg()
	{
#ifdef _ENABLE_OGG
#	ifdef _WIN32
		if (mOggHandle)
		{
			FreeLibrary(mOggHandle);
			mOggHandle = nullptr;
			mOggClear = nullptr;
			mOggRead = nullptr;
			mOggPcmTotal = nullptr;
			mOggInfo = nullptr;
			mOggComment = nullptr;
			mOggOpenCallbacks = nullptr;
		}
#	endif
        mOggLoaded = false;
#endif
	}

	std::string SfxEngine::getExtension(const char *filename)
	{
		if (filename == nullptr)
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
