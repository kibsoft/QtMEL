#ifndef VIDEOCODECSETTINGS_H
#define VIDEOCODECSETTINGS_H

#include "encoderglobal.h"

class VideoCodecSettings
{
public:
    VideoCodecSettings();

    void setBitrate(int bitrate);
    int bitrate() const;

    void setGopSize(int gop);
    int gopSize() const;

    void setMinimumQuantizer(int qmin);
    int minimumQuantizer() const;

    void setMaximumQuantizer(int qmax);
    int maximumQuantizer() const;

    void setMaximumQuantizerDifference(int qdiff);
    int maximumQuantizerDifference() const;

    void setCoderType(EncoderGlobal::CoderType type);
    EncoderGlobal::CoderType coderType() const;

    void setMotionEstimationComparison(int cmp);
    int motionEstimationComparison() const;

    void setPartitions(EncoderGlobal::Partitions partitions);
    EncoderGlobal::Partitions partitions() const;

    void setMotionEstimationMethod(EncoderGlobal::MotionEstimationAlgorithm method);
    EncoderGlobal::MotionEstimationAlgorithm motionEstimationMethod() const;

    void setSubpixelMotionEstimationQuality(int subpel);
    int subpixelMotionEstimationQuality() const;

    void setMotionEstimationRange(int range);
    int motionEstimationRange() const;

    void setMinimumKeyframeInterval(int interval);
    int minimumKeyframeInterval() const;

    void setSceneChangeThreshold(int threshold);
    int sceneChangeThreshold() const;

    void setIQuantFactor(float quantFactor);
    float iQuantFactor() const;

    void setBFrameStrategy(int strategy);
    int bFrameStrategy() const;

    void setQuantizerCurveCompressionFactor(float factor);
    float quantizerCurveCompressionFactor() const;

    void setMaximumBFrames(int max);
    int maximumBFrames() const;

    void setReferenceFrameCount(int count);
    int referenceFrameCount() const;

    void setDirectMvPredictionMode(EncoderGlobal::MotionVectorPredictionMode mode);
    EncoderGlobal::MotionVectorPredictionMode directMvPredictionMode() const;

    void setTrellis(int trellis);
    int trellis() const;

    void setPFramePredictionAnalysisMethod(EncoderGlobal::WeightedPredictionMethod method);
    EncoderGlobal::WeightedPredictionMethod pFramePredictionAnalysisMethod() const;

    void setRcLookahead(int value);
    int rcLookahead() const;

    void setFlags(EncoderGlobal::Flags flags);
    EncoderGlobal::Flags flags() const;

    void setFlags2(EncoderGlobal::Flags2 flags);
    EncoderGlobal::Flags2 flags2() const;

private:
    int m_bitrate; /*!< The average bitrate. */
    int m_gop; /*!< The number of pictures in a group of pictures. */
    int m_qmin; /*!< Minimum quantizer. */
    int m_qmax; /*!< Maximum  quantizer. */
    int m_maxQdiff; /*!< Maximum quantizer difference between frames. */
    EncoderGlobal::CoderType m_coderType; /*!< Coder type. */
    int m_meCmp; /*!< Motion estimation comparison function. */
    EncoderGlobal::Partitions m_partitions; /*!< Macroblock subpartition sizes to consider - p8x8, p4x4, b8x8, i8x8, i4x4. */
    EncoderGlobal::MotionEstimationAlgorithm m_meMethod; /*!< Motion estimation algorithm used for video coding. */
    int m_meSubpelQuality; /*!< Subpel ME quality. */
    int m_meRange; /*!< Maximum motion estimation search range in subpel units If 0 then no limit. */
    int m_keyintMin; /*!< Minimum GOP size. */
    int m_sceneChangeThreshold; /*!< Scene change detection threshold 0 is default, larger means fewer detected scene changes. */
    float m_iQuantFactor ; /*!< qscale factor between P and I-frames. */
    int m_bFrameStrategy  ; /*!<  */
    float m_qcompress; /*!< Amount of qscale change between easy & hard scenes (0.0-1.0). */
    int m_maxBFrames; /*!< Maximum number of B-frames between non-B-frames. */
    int m_refs; /*!< Number of reference frames. */
    EncoderGlobal::MotionVectorPredictionMode m_directpred; /*!< Direct MV prediction mode. */
    int m_trellis; /*!< Trellis RD quantization. */
    EncoderGlobal::WeightedPredictionMethod m_weightedPPred; /*!< Explicit P-frame weighted prediction analysis method. */
    int m_rcLookahead; /*!< RC lookahead Number of frames for frametype and ratecontrol lookahead. */
    EncoderGlobal::Flags m_flags; /*!< */
    EncoderGlobal::Flags2 m_flags2; /*!< */
};

#endif // VIDEOCODECSETTINGS_H
