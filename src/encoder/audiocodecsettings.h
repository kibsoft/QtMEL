#ifndef AUDIOCODECSETTINGS_H
#define AUDIOCODECSETTINGS_H

#include "../qvacl_global.h"
#include "encoderglobal.h"

class QVACLSHARED_EXPORT AudioCodecSettings
{
public:
    AudioCodecSettings();

    void setSampleFormat(EncoderGlobal::SampleFormat format);
    EncoderGlobal::SampleFormat sampleFormat() const;

    void setBitrate(int bitrate);
    int bitrate() const;

    void setSampleRate(int rate);
    int sampleRate() const;

    void setChannelCount(int count);
    int channelCount() const;

private:
    EncoderGlobal::SampleFormat m_sampleFormat;
    int m_bitrate;
    int m_sampleRate;
    int m_channelCount;
};

#endif // AUDIOCODECSETTINGS_H
