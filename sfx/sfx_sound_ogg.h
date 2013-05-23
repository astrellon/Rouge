#pragma once

#include <vector>
#include <string>
using namespace std;

#include "sfx_isound.h"

#include <al.h>
#include <sfx/ogg/Vorbis/vorbisfile.h>

namespace am {
namespace sfx {

	class Source;
	class SfxEngine;

	class SoundOgg : public ISound {
	public:
		
		SoundOgg();
		SoundOgg(const char *filename, bool toStream = false);
		virtual ~SoundOgg();

		virtual bool loadSound(const char *filename);
		virtual bool loadStream(const char *filename, int buffers = 4);

		virtual void setBufferTime(ALuint ms);

		virtual void update();

		static bool getALFormat(long numChannels, ALenum &format);

	protected:
		
		OggVorbis_File *mOggFile;
		char *mData;

		bool fillBuffer(ALuint bufferId);
		unsigned long decodeOggVorbis(char *pDecodeBuffer, unsigned long ulBufferSize);
		void swap(short &s1, short &s2);

		bool parseHeaders(FILE *file, SfxEngine *engine);

		static void initCallbacks();
		static ov_callbacks sOggCallbacks;
		static bool sOggCallbacksInited;
		static size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
		static int ov_seek_func(void *datasource, ogg_int64_t offset, int whence);
		static int ov_close_func(void *datasource);
		static long ov_tell_func(void *datasource);

	};

}
}
