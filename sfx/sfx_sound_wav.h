#pragma once

#include <vector>
#include <string>

#include "sfx_isound.h"

namespace am {
namespace sfx {

	class SoundWav : public ISound {
	public:

		SoundWav();
		SoundWav(const char *filename, bool toStream = false);
		~SoundWav();

		/*
		 * Struct that holds the RIFF data of the Wave file.
		 * The RIFF data is the meta data information that holds,
		 * the ID, size and format of the wave file
		 */
		typedef struct {
			char chunkID[4];
			long chunkSize;//size not including chunkSize or chunkID
			char format[4];
		} RIFFHeader;

		typedef struct
		{
			char name[4];
			unsigned long size;
		} RIFFChunk;

		typedef struct _WAVEHeader {
			unsigned short audioFormat;
			unsigned short numChannels;
			unsigned long sampleRate;
			unsigned long byteRate;
			unsigned short blockAlign;
			unsigned short bitsPerSample;
			unsigned short size;
			unsigned short reserved;
			unsigned long channelMask;
			char GUID[16];

			_WAVEHeader();
		} WAVEHeader;

		virtual void update();

		virtual bool loadSound(const char *filename);
		virtual bool loadStream(const char *filename, int buffers = 4);

		static bool getALFormat(long numChannels, long bitsPerSample, long channelMask, ALenum &format);

		virtual void setBufferTime(ALuint ms);

	protected:

		FILE *mFileStream;
		unsigned char *mData;

		bool fillBuffer(ALuint buffer);
		void parseHeaders(RIFFHeader &riffHeader, WAVEHeader &waveHeader, ALuint &dataSize);
		
	};

}
}
