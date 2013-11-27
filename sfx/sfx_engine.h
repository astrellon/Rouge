#pragma once

#include <vector>
#include <string>
using namespace std;

#include <base/handle.h>

#include <AL/al.h>

#ifdef _WIN32
#   include <Windows.h>
#endif
#include <sfx/ogg/Vorbis/vorbisfile.h>

#include <sfx/sfx_listener.h>
#include <sfx/sfx_isound.h>
#include <sfx/sfx_source_point.h>

namespace am {
namespace sfx {

	typedef struct _DeviceName {
		string deviceName;
		bool defaultDevice;

		_DeviceName(const char *name, bool defaultDevice = false);
	} DeviceName;

	typedef struct _SourceId {
		ALuint id;
		ISource *source;

		_SourceId();
		_SourceId(ALuint id);

		bool isStreaming() const;
		float getGain() const;
	} SourceId;

	class SfxEngine {
	public:
		SfxEngine();

		typedef vector<DeviceName> DeviceList;
		typedef vector<SourceId> SourcePool;
		typedef map<string, am::base::Handle<ISound> > SoundMap;

		void init();
		bool hasInited() const;
		bool createContext(int deviceIndex = -1);
		void deinit();

		int getNumSources() const;
		bool getSource(ALuint &source, ISource *forSource);
		void releaseSource(ALuint source);

		const DeviceList &getDeviceList();

		void update();

		void displayInUse();

		ISound *loadSound(const char *filename);
		ISound *loadStream(const char *filename, int numBuffers = 4);

		void setBackgroundMusic(ISound *bgm);
		void setBackgroundMusic(const char *filename);
		ISound *getBackgroundMusic() const;

		Listener &getListener();

		void addInactiveSource(ISource *source);
		void removeInactiveSource(ISource *source);

		bool oggLoaded() const;

		static SfxEngine *getEngine();
		static void deinitSfxEngine();

		static const char *getErrorCode(ALenum errorCode);
		static void checkError(const char *message = nullptr, const char *filename = nullptr);

		static string getExtension(const char *filename);

	protected:
		bool mInited;
		DeviceList mDeviceList;
		string mDefaultDevice;

		int mSourcePoolPos;
		SourcePool mSourcePool;

		int nextPoolIndex();

		am::base::Handle<SourcePoint> mBackgroundMusicSource;
		am::base::Handle<ISound> mBackgroundMusic;

		SoundMap mSoundMap;

		Listener mListener;

		typedef vector< am::base::Handle<ISource> > SourceList;
		SourceList mInactiveSources;

		size_t findInactiveSource(ISource *source);

		static SfxEngine *sEngine;

		// OGG Specific variables/functions
	public:
		// function pointers
		typedef int (*OggClear)(OggVorbis_File *vf);
		typedef long (*OggRead)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
		typedef ogg_int64_t (*OggPcmTotal)(OggVorbis_File *vf,int i);
		typedef ogg_int64_t (*OggRawTotal)(OggVorbis_File *vf,int i);
		typedef vorbis_info * (*OggInfo)(OggVorbis_File *vf,int link);
		typedef vorbis_comment * (*OggComment)(OggVorbis_File *vf,int link);
		typedef int (*OggOpenCallbacks)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);

		OggClear getOggClear() const;
		OggRead getOggRead() const;
		OggPcmTotal getOggPcmTotal() const;
		OggRawTotal getOggRawTotal() const;
		OggInfo getOggInfo() const;
		OggComment getOggComment() const;
		OggOpenCallbacks getOggOpenCallbacks() const;

	protected:
		// Function Pointer Variables
		OggClear mOggClear;
		OggRead	mOggRead;
		OggPcmTotal mOggPcmTotal;
		OggRawTotal mOggRawTotal;
		OggInfo	mOggInfo;
		OggComment mOggComment;
		OggOpenCallbacks mOggOpenCallbacks;

#ifdef _WIN32
		HINSTANCE mOggHandle;
#endif
		
		bool initOgg();
		void deinitOgg();
		// -- End OGG

	};

}
}
