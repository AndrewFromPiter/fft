#pragma once
#include <portaudio.h>
#include <complex>
#include "RingBuffer.hpp"

#define NUM_CHANNELS (1)          // моно
#define SAMPLE_FORMAT  paInt32    

class AudioReader
{
	PaStream* stream = nullptr;
	RingBuffer<std::complex<double>>* output = nullptr;
	int Samples_In_Package , Sample_Rate;
public:
	AudioReader(int,int); // Samples_In_Package , Sample_Rate
	~AudioReader();
	void SetBuffer(RingBuffer<std::complex<double>>*);
	void ReadLoop();
};