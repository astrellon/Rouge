#include "sfx_sound_wav.h"

#include "sfx_isource.h"

#include <iostream>
#include <sstream>
#include <cstring>

namespace am {
namespace sfx {

	SoundWav::_WAVEHeader::_WAVEHeader() :
		audioFormat(0),
		numChannels(0),
		sampleRate(0),
		byteRate(0),
		blockAlign(0),
		bitsPerSample(0),
		size(0),
		reserved(0),
		channelMask(0)
	{
	}

	SoundWav::SoundWav() :
		ISound(),
		mFileStream(nullptr),
		mData(nullptr)
	{
		mBufferTime = 500;
	}
		SoundWav::SoundWav(const char *filename, bool toStream) :
		ISound(),
		mFileStream(nullptr),
		mData(nullptr)
	{
		mBufferTime = 500;
		if (toStream && !loadStream(filename))
		{
			throw ("Error loading stream");
		}
		else if (!toStream && !loadSound(filename))
		{
			throw ("Error loading sound");
		}
	}
	SoundWav::~SoundWav()
	{
		if (mBuffers)
		{
			alDeleteBuffers(mNumBuffers, mBuffers);
			mBuffers = nullptr;
		}
		if (mData)
		{
			delete mData;
			mData = nullptr;
		}
		if (mFileStream)
		{
			fclose(mFileStream);
			mFileStream = nullptr;
		}
	}

	void SoundWav::update()
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

	void SoundWav::setBufferTime(ALuint ms)
	{
		mBufferTime = ms;
	}

	bool SoundWav::loadSound(const char *filename)
	{
		mFilename = "";
		mNumBuffers = 1;
		mNumBuffersInUse = 1;
		mBuffers = new ALuint[1];
		// Generate an AL Buffer
		alGenBuffers( 1, mBuffers );
		
		mStream = false;

		mFileStream = nullptr;
		//Local Declarations
		RIFFHeader riffHeader;
		WAVEHeader waveHeader;
		
		try 
		{
			mFileStream = fopen(filename, "rb");
			if (!mFileStream)
			{
				throw (filename);
			}

			ALuint dataSize = 0;
			parseHeaders(riffHeader, waveHeader, dataSize);
			mBufferByteSize = dataSize;
			mFrequency = waveHeader.sampleRate;

			//Allocate memory for data
			mData = new unsigned char[mBufferByteSize];
			if (!mData)
			{
				std::stringstream ss;
				ss << "Unable to allocate sound buffer of size " << mBufferByteSize << " for " << filename;
				throw (ss.str().c_str());
			}
 
			fillBuffer(mBuffers[0]);

			delete mData;
			mData = nullptr;
			//errorCheck();
			//clean up and return true if successful
			fclose(mFileStream);
			mFilename = filename;
			return true;
		} 
		catch(std::string error) 
		{
			//our catch statement for if we throw a string
			std::cerr << error << " : trying to load "
			<< filename << std::endl;
			//clean up memory if wave loading fails
			if (mFileStream != nullptr)
			{
				fclose(mFileStream);
			}
			//return false to indicate the failure to load wave
			return false;
		}
		return true;
	}

	bool SoundWav::loadStream(const char *filename, int numBuffers)
	{
		mFilename = "";
		mNumBuffers = numBuffers;
		// Generate an AL Buffer
		mBuffers = new ALuint[numBuffers];
		alGenBuffers( numBuffers, mBuffers );

		mStream = true;

		mFileStream = nullptr;
		//Local Declarations
		RIFFHeader riffHeader;
		WAVEHeader waveHeader;
		
		try 
		{
			mFileStream = fopen(filename, "rb");
			if (!mFileStream)
			{
				throw (filename);
			}

			ALuint dataSize = 0;
			parseHeaders(riffHeader, waveHeader, dataSize);

			mBufferByteSize = static_cast<ALuint>(static_cast<float>(waveHeader.byteRate) * static_cast<float>(mBufferTime) / 1000.0f);

			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferByteSize -= (mBufferByteSize % waveHeader.blockAlign);
			
			//Allocate memory for data
			mData = new unsigned char[mBufferByteSize];
 
			mNumBuffersInUse = numBuffers;
			for (int i = 0; i < numBuffers; i++)
			{
				if (!fillBuffer(mBuffers[i]))
				{
					mNumBuffersInUse = i;
					break;
				}
			}

			//errorCheck();
			//clean up and return true if successful
			mFilename = filename;
			return true;
		} 
		catch(std::string error) 
		{
			//our catch statement for if we throw a string
			std::cerr << error << " : trying to load "
			<< filename << std::endl;
			//clean up memory if wave loading fails
			if (mFileStream != nullptr)
			{
				fclose(mFileStream);
			}
			//return false to indicate the failure to load wave
			return false;
		}
		return true;
	}

	bool SoundWav::getALFormat(long numChannels, long bitsPerSample, long channelMask, ALenum &format)
	{
		ALenum temp = AL_INVALID;
		if (numChannels == 1 && 
			(channelMask == SPEAKER_FRONT_CENTER ||
			(channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT)) ||
			(channelMask == 0))) 
		{
			switch (bitsPerSample)
			{
			case 4:		temp = alGetEnumValue("AL_FORMAT_MONO_IMA4");		break;
			case 8:		temp = alGetEnumValue("AL_FORMAT_MONO8");			break;
			case 16:	temp = alGetEnumValue("AL_FORMAT_MONO16");			break;
			}
		} 
		else if (numChannels == 2 && 
			(channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT) ||
			channelMask == 0))
		{
			switch (bitsPerSample)
			{
			case 4:		temp = alGetEnumValue("AL_FORMAT_STEREO_IMA4");		break;
			case 8:		temp = alGetEnumValue("AL_FORMAT_STEREO8");			break;
			case 16:	temp = alGetEnumValue("AL_FORMAT_STEREO16");		break;
			}	
		}
		else if (numChannels == 4 && bitsPerSample == 16)
		{
			temp = alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if (numChannels == 2 && bitsPerSample == 16 && channelMask == (SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT))
		{
			temp = alGetEnumValue("AL_FORMAT_REAR16");
		}
		else if (numChannels == 4 && bitsPerSample == 16 && channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT))
		{
			temp = alGetEnumValue("AL_FORMAT_QUAD16");
		}
		else if (numChannels == 6 && bitsPerSample == 16 && channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT))
		{
			temp = alGetEnumValue("AL_FORMAT_51CHN16");
		}
		else if (numChannels == 7 && bitsPerSample== 16 && channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_BACK_CENTER))
		{
			temp = alGetEnumValue("AL_FORMAT_61CHN16");
		}
		else if (numChannels == 8 && bitsPerSample == 16 && channelMask == (SPEAKER_FRONT_LEFT|SPEAKER_FRONT_RIGHT|SPEAKER_FRONT_CENTER|SPEAKER_LOW_FREQUENCY|SPEAKER_BACK_LEFT|SPEAKER_BACK_RIGHT|SPEAKER_SIDE_LEFT|SPEAKER_SIDE_RIGHT))
		{
			temp = alGetEnumValue("AL_FORMAT_71CHN16");
		}

		if (temp != AL_INVALID)
		{
			format = temp;
			return true;
		}
		return false;
	}

	void SoundWav::parseHeaders(SoundWav::RIFFHeader &riffHeader, SoundWav::WAVEHeader &waveHeader, ALuint &dataSize)
	{
		RIFFChunk dataChunk;
		// Read in the first chunk into the struct
		fread(&riffHeader, sizeof(RIFFHeader), 1, mFileStream);
		if (strncmp(riffHeader.chunkID, "RIFF", 4) != 0 ||
			strncmp(riffHeader.format, "WAVE", 4) != 0)
		{
			throw ("Invalid RIFF or WAVE Header");
		}

		bool parsedFmtChunk = false;
		while(!feof(mFileStream))
		{
			fread(&dataChunk, sizeof(RIFFChunk), 1, mFileStream);

			// fmt RIFF Chunk contains the data about the WAVE header.
			if (strncmp(dataChunk.name, "fmt ", 4) == 0)
			{
				parsedFmtChunk = true;
				// This can rand from 16 to 40 bytes long depending on the WAVE file.
				fread(&waveHeader, dataChunk.size, 1, mFileStream);
				if(!getALFormat(waveHeader.numChannels, waveHeader.bitsPerSample, waveHeader.channelMask, mALFormat))
				{
					throw ("Unable to get WAVE format");
				}
				continue;
			}
			else if (strncmp(dataChunk.name, "data", 4) == 0)
			{
				break;
			}
			// Other chunks are not supported yet, such as cue points.
			fseek(mFileStream, dataChunk.size, SEEK_CUR);
		}

		if (!parsedFmtChunk)
		{
			throw ("No WAVE format chunk found");
		}

		// data RIFF Chunk contains the size of the PCM data.
		if (strncmp(dataChunk.name, "data", 4) != 0)
		{
			throw ("Invalid data chunk");
		}

		mFrequency = waveHeader.sampleRate;
		mChannels = waveHeader.numChannels;
		dataSize = dataChunk.size;
	}

	bool SoundWav::fillBuffer(ALuint buffer)
	{
		// Read in the sound data into the soundData variable
		size_t bytesRead = fread(mData, 1, mBufferByteSize, mFileStream);
		if (bytesRead == 0)
		{
			return false;
		}

		//now we put our data into the openAL buffer and
		//check for success
		alBufferData(buffer, mALFormat, (void*)mData, bytesRead, mFrequency);
		return true;
	}

}
}
