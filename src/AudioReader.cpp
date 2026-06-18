#include "AudioReader.h"

AudioReader::AudioReader(int SIP, int SR): Samples_In_Package(SIP), Sample_Rate(SR)
{
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError)
        throw std::exception("Pa_Initialize failed");

	PaStreamParameters inputParams = {};
    inputParams.device = Pa_GetDefaultInputDevice();
    if (inputParams.device == paNoDevice) {
        throw std::exception("No device");
    }
    inputParams.channelCount = NUM_CHANNELS;
    inputParams.sampleFormat = SAMPLE_FORMAT;
    inputParams.suggestedLatency = Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;

    err = Pa_OpenStream(
        &stream,
        &inputParams,
        nullptr,
        Sample_Rate,
        Samples_In_Package,
        paClipOff,
        nullptr,
        nullptr
    );

    if (err != paNoError)
    {
        throw std::exception(Pa_GetErrorText(err));
    }

    err = Pa_StartStream(stream);
    if(err != paNoError) 
    {
        throw std::exception(Pa_GetErrorText(err));
    }
}

AudioReader::~AudioReader()
{
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    stream = nullptr;
    Pa_Terminate();
}

void AudioReader::SetBuffer(RingBuffer<std::complex<double>>* RB)
{
    output = RB;
}

void AudioReader::ReadLoop()
{
    if (output == nullptr) return;

    std::vector<int> tempBuffer(Samples_In_Package * NUM_CHANNELS);
    std::vector<std::complex<double>> compBuffer(Samples_In_Package * NUM_CHANNELS);
    while (1) {
        Pa_ReadStream(stream, tempBuffer.data(), Samples_In_Package);
        for (int i = 0; i < compBuffer.size();i++)
        {
            compBuffer[i] = tempBuffer[i];
        }
        output->push(compBuffer);
    }

}
