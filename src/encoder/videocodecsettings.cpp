#include "videocodecsettings.h"

VideoCodecSettings::VideoCodecSettings()
{
    m_bitrate = -1;
    m_gop = -1;
    m_qmin = -1;
    m_qmax = -1;
    m_maxQdiff = -1;
    m_coderType = static_cast<Encoder::CoderType>(-1);
    m_meCmp = -1;
    m_partitions = static_cast<Encoder::Partitions>(-1);
    m_meMethod = static_cast<Encoder::MotionEstimationAlgorithm>(-1);
    m_meSubpelQuality = -1;
    m_meRange = -1;
    m_keyintMin = -1;
    m_sceneChangeThreshold = -1;
    m_iQuantFactor = -1;
    m_bFrameStrategy = -1;
    m_qcompress = -1;
    m_maxBFrames = -1;
    m_refs = -1;
    m_directpred = static_cast<Encoder::MotionVectorPredictionMode>(-1);
    m_trellis = -1;
    m_weightedPPred = static_cast<Encoder::WeightedPredictionMethod>(-1);
    m_rcLookahead = -1;
    m_flags = static_cast<Encoder::Flags>(-1);
    m_flags2 = static_cast<Encoder::Flags2>(-1);
}

void VideoCodecSettings::setBitrate(int bitrate)
{
    m_bitrate = bitrate;
}

int VideoCodecSettings::bitrate() const
{
    return m_bitrate;
}

void VideoCodecSettings::setGopSize(int gop)
{
    m_gop = gop;
}

int VideoCodecSettings::gopSize() const
{
    return m_gop;
}

void VideoCodecSettings::setMinimumQuantizer(int qmin)
{
    m_qmin = qmin;
}

int VideoCodecSettings::minimumQuantizer() const
{
    return m_qmin;
}

void VideoCodecSettings::setMaximumQuantizer(int qmax)
{
    m_qmax = qmax;
}

int VideoCodecSettings::maximumQuantizer() const
{
    return m_qmax;
}

void VideoCodecSettings::setMaximumQuantizerDifference(int qdiff)
{
    m_maxQdiff = qdiff;
}

int VideoCodecSettings::maximumQuantizerDifference() const
{
    return m_maxQdiff;
}

void VideoCodecSettings::setCoderType(Encoder::CoderType type)
{
    m_coderType = type;
}

Encoder::CoderType VideoCodecSettings::coderType() const
{
    return m_coderType;
}

void VideoCodecSettings::setMotionEstimationComparison(int cmp)
{
    m_meCmp = cmp;
}

int VideoCodecSettings::motionEstimationComparison() const
{
    return m_meCmp;
}

void VideoCodecSettings::setPartitions(Encoder::Partitions partitions)
{
    m_partitions = partitions;
}

Encoder::Partitions VideoCodecSettings::partitions() const
{
    return m_partitions;
}

void VideoCodecSettings::setMotionEstimationMethod(Encoder::MotionEstimationAlgorithm method)
{
    m_meMethod = method;
}

Encoder::MotionEstimationAlgorithm VideoCodecSettings::motionEstimationMethod() const
{
    return m_meMethod;
}

void VideoCodecSettings::setSubpixelMotionEstimationQuality(int subpel)
{
    m_meSubpelQuality = subpel;
}

int VideoCodecSettings::subpixelMotionEstimationQuality() const
{
    return m_meSubpelQuality;
}

void VideoCodecSettings::setMotionEstimationRange(int range)
{
    m_meRange = range;
}

int VideoCodecSettings::motionEstimationRange() const
{
    return m_meRange;
}

void VideoCodecSettings::setMinimumKeyframeInterval(int interval)
{
    m_keyintMin = interval;
}

int VideoCodecSettings::minimumKeyframeInterval() const
{
    return m_keyintMin;
}

void VideoCodecSettings::setSceneChangeThreshold(int threshold)
{
    m_sceneChangeThreshold = threshold;
}

int VideoCodecSettings::sceneChangeThreshold() const
{
    return m_sceneChangeThreshold;
}

void VideoCodecSettings::setIQuantFactor(int quantFactor)
{
    m_iQuantFactor = quantFactor;
}

int VideoCodecSettings::iQuantFactor() const
{
    return m_iQuantFactor;
}

void VideoCodecSettings::setBFrameStrategy(int strategy)
{
    m_bFrameStrategy = strategy;
}

int VideoCodecSettings::bFrameStrategy()
{
    return m_bFrameStrategy;
}

void VideoCodecSettings::setQuantizerCurveCompressionFactor(int factor)
{
    m_qcompress = factor;
}

int VideoCodecSettings::quantizerCurveCompressionFactor()
{
    return m_qcompress;
}

void VideoCodecSettings::setReferenceFrameCount(int count)
{
    m_refs = count;
}

int VideoCodecSettings::referenceFrameCount() const
{
    return m_refs;
}

void VideoCodecSettings::setDirectMvPredictionMode(Encoder::MotionVectorPredictionMode mode)
{
    m_directpred = mode;
}

Encoder::MotionVectorPredictionMode VideoCodecSettings::directMvPredictionMode() const
{
    return m_directpred;
}

void VideoCodecSettings::setTrellis(int trellis)
{
    m_trellis = trellis;
}

int VideoCodecSettings::trellis() const
{
    return m_trellis;
}

void VideoCodecSettings::setRcLookahead(int value)
{
    m_rcLookahead = value;
}

int VideoCodecSettings::rcLookahead() const
{
    return m_rcLookahead;
}

void VideoCodecSettings::setFlags(Encoder::Flags flags)
{
    m_flags = flags;
}

Encoder::Flags VideoCodecSettings::flags() const
{
    return m_flags;
}

void VideoCodecSettings::setFlags2(Encoder::Flags2 flags)
{
    m_flags2 = flags;
}

Encoder::Flags2 VideoCodecSettings::flags2() const
{
    return m_flags2;
}
