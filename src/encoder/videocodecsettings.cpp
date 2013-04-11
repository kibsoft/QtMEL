#include "videocodecsettings.h"

VideoCodecSettings::VideoCodecSettings()
{
    m_bitrate = -1;
    m_gop = -1;
    m_qmin = -1;
    m_qmax = -1;
    m_maxQdiff = -1;
    m_coderType = static_cast<EncoderGlobal::CoderType>(-1);
    m_meCmp = -1;
    m_partitions = static_cast<EncoderGlobal::Partitions>(-1);
    m_meMethod = static_cast<EncoderGlobal::MotionEstimationAlgorithm>(-1);
    m_meSubpelQuality = -1;
    m_meRange = -1;
    m_keyintMin = -1;
    m_sceneChangeThreshold = -1;
    m_iQuantFactor = -1.0f;
    m_bFrameStrategy = -1;
    m_qcompress = -1.0f;
    m_maxBFrames = -1;
    m_refs = -1;
    m_directpred = static_cast<EncoderGlobal::MotionVectorPredictionMode>(-1);
    m_trellis = -1;
    m_weightedPPred = static_cast<EncoderGlobal::WeightedPredictionMethod>(-1);
    m_rcLookahead = -1;
    m_flags = static_cast<EncoderGlobal::Flags>(-1);
    m_flags2 = static_cast<EncoderGlobal::Flags2>(-1);
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

void VideoCodecSettings::setCoderType(EncoderGlobal::CoderType type)
{
    m_coderType = type;
}

EncoderGlobal::CoderType VideoCodecSettings::coderType() const
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

void VideoCodecSettings::setPartitions(EncoderGlobal::Partitions partitions)
{
    m_partitions = partitions;
}

EncoderGlobal::Partitions VideoCodecSettings::partitions() const
{
    return m_partitions;
}

void VideoCodecSettings::setMotionEstimationMethod(EncoderGlobal::MotionEstimationAlgorithm method)
{
    m_meMethod = method;
}

EncoderGlobal::MotionEstimationAlgorithm VideoCodecSettings::motionEstimationMethod() const
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

void VideoCodecSettings::setIQuantFactor(float quantFactor)
{
    m_iQuantFactor = quantFactor;
}

float VideoCodecSettings::iQuantFactor() const
{
    return m_iQuantFactor;
}

void VideoCodecSettings::setBFrameStrategy(int strategy)
{
    m_bFrameStrategy = strategy;
}

int VideoCodecSettings::bFrameStrategy() const
{
    return m_bFrameStrategy;
}

void VideoCodecSettings::setQuantizerCurveCompressionFactor(float factor)
{
    m_qcompress = factor;
}

float VideoCodecSettings::quantizerCurveCompressionFactor() const
{
    return m_qcompress;
}

void VideoCodecSettings::setMaximumBFrames(int max)
{
    m_maxBFrames = max;
}

int VideoCodecSettings::maximumBFrames() const
{
    return m_maxBFrames;
}

void VideoCodecSettings::setReferenceFrameCount(int count)
{
    m_refs = count;
}

int VideoCodecSettings::referenceFrameCount() const
{
    return m_refs;
}

void VideoCodecSettings::setDirectMvPredictionMode(EncoderGlobal::MotionVectorPredictionMode mode)
{
    m_directpred = mode;
}

EncoderGlobal::MotionVectorPredictionMode VideoCodecSettings::directMvPredictionMode() const
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

void VideoCodecSettings::setPFramePredictionAnalysisMethod(EncoderGlobal::WeightedPredictionMethod method)
{
    m_weightedPPred = method;
}

EncoderGlobal::WeightedPredictionMethod VideoCodecSettings::pFramePredictionAnalysisMethod() const
{
    return m_weightedPPred;
}

void VideoCodecSettings::setRcLookahead(int value)
{
    m_rcLookahead = value;
}

int VideoCodecSettings::rcLookahead() const
{
    return m_rcLookahead;
}

void VideoCodecSettings::setFlags(EncoderGlobal::Flags flags)
{
    m_flags = flags;
}

EncoderGlobal::Flags VideoCodecSettings::flags() const
{
    return m_flags;
}

void VideoCodecSettings::setFlags2(EncoderGlobal::Flags2 flags)
{
    m_flags2 = flags;
}

EncoderGlobal::Flags2 VideoCodecSettings::flags2() const
{
    return m_flags2;
}
