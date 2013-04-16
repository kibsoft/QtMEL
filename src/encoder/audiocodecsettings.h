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

#ifndef AUDIOCODECSETTINGS_H
#define AUDIOCODECSETTINGS_H

#include "../qtmel_global.h"
#include "encoderglobal.h"

class QTMELSHARED_EXPORT AudioCodecSettings
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
