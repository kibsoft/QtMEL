/****************************************************************************
**
** QtMEL is a Qt Media Encoding Library that allows to encode video and audio streams
** Copyright (C) 2013 Kirill Bukaev(aka KIBSOFT).
** Contact: Kirill Bukaev (support@kibsoft.ru)
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

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
