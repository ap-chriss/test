#ifndef _WAV_FILE_H_
#define _WAV_FILE_H_

typedef struct
{
    char chunkID[4];
    UINT32 chunkDataSize;
    char riffType[4];

}RIFF_TYPE_CHUNK;

typedef struct
{
    char chunkID[4];
    UINT32 chunkDataSize;
    UINT16 compressionCode;
    UINT16 nChannels;
    UINT32 sampleRate;
    UINT32 averageBytesPerSecond;
    UINT16 blockAlign;
    UINT16 bitsPerSample;

}WAVE_FORMAT_CHUNK;

typedef struct
{
    char chunkID[4];
    UINT32 chunkDataSize;

}DATA_CHUNK;

#endif