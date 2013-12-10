#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include <AL/al.h>

#include <base/imanaged.h>

namespace am {
namespace sfx {

	class Source;

    class sound_error : public std::runtime_error
    {
    public:
        explicit sound_error(const char *message) :
            std::runtime_error(message) 
        {
        }

    };

	class ISound : public base::IManaged 
	{
	public:

		enum SpeakerInfo
		{
			SPEAKER_FRONT_LEFT             = 0x1,
			SPEAKER_FRONT_RIGHT            = 0x2,
			SPEAKER_FRONT_CENTER           = 0x4,
			SPEAKER_LOW_FREQUENCY          = 0x8,
			SPEAKER_BACK_LEFT              = 0x10,
			SPEAKER_BACK_RIGHT             = 0x20,
			SPEAKER_FRONT_LEFT_OF_CENTER   = 0x40,
			SPEAKER_FRONT_RIGHT_OF_CENTER  = 0x80,
			SPEAKER_BACK_CENTER            = 0x100,
			SPEAKER_SIDE_LEFT              = 0x200,
			SPEAKER_SIDE_RIGHT             = 0x400,
			SPEAKER_TOP_CENTER             = 0x800,
			SPEAKER_TOP_FRONT_LEFT         = 0x1000,
			SPEAKER_TOP_FRONT_CENTER       = 0x2000,
			SPEAKER_TOP_FRONT_RIGHT        = 0x4000,
			SPEAKER_TOP_BACK_LEFT          = 0x8000,
			SPEAKER_TOP_BACK_CENTER        = 0x10000,
			SPEAKER_TOP_BACK_RIGHT         = 0x20000
		};
		
		ISound();
		virtual ~ISound();

		virtual ALuint getNumBuffers() const;

		virtual void update();

		virtual bool isStream() const;

		virtual bool loadSound(const char *filename) = 0;
		virtual bool loadStream(const char *filename, int buffers = 4) = 0;
		virtual const char *getFilename() const;

		virtual void attachSource(ALuint source);
		virtual void detachSource(ALuint source);
		virtual ALuint getStreamSource() const;

		virtual void setBufferTime(ALuint ms) = 0;
		virtual ALuint getBufferTime() const;

		virtual ALuint getBufferByteSize();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		bool mStream;
		std::string mFilename;
		ALuint *mBuffers;
		ALushort mNumBuffers;
		ALushort mNumBuffersInUse;
		unsigned short mChannels;
		ALuint mBufferTime;
		ALuint mBufferByteSize;
		ALenum mALFormat;
		ALsizei mFrequency;
		ALuint mStreamSource;

	};

}
}
