#include "audiocodecsettings.h"

AudioCodecSettings::AudioCodecSettings()
{
    m_sampleFormat = EncoderGlobal::NoSampleFormat;
    m_bitrate = -1;
    m_sampleRate = -1;
    m_channelCount = -1;
}

void AudioCodecSettings::setSampleFormat(EncoderGlobal::SampleFormat format)
{
    m_sampleFormat = format;
}

EncoderGlobal::SampleFormat AudioCodecSettings::sampleFormat() const
{
    return m_sampleFormat;
}

void AudioCodecSettings::setBitrate(int bitrate)
{
    m_bitrate = bitrate;
}

int AudioCodecSettings::bitrate() const
{
    return m_bitrate;
}

void AudioCodecSettings::setSampleRate(int rate)
{
    m_sampleRate = rate;
}

int AudioCodecSettings::sampleRate() const
{
    return m_sampleRate;
}

void AudioCodecSettings::setChannelCount(int count)
{
    m_channelCount = count;
}

int AudioCodecSettings::channelCount() const
{
    return m_channelCount;
}
