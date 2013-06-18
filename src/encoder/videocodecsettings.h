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

#ifndef VIDEOCODECSETTINGS_H
#define VIDEOCODECSETTINGS_H

#include "../qtmel_global.h"
#include "encoderglobal.h"

class QTMELSHARED_EXPORT VideoCodecSettings
{
public:
    VideoCodecSettings();

    /*!
     * \brief The average bitrate. It is equal to the bit_rate parameter in AVCodecContext.
     */
    void setBitrate(int bitrate);
    int bitrate() const;

    /*!
     * \brief The number of pictures in a group of pictures. It is equal to the gop_size parameter in AVCodecContext.
     */
    void setGopSize(int gop);
    int gopSize() const;

    /*!
     * \brief Minimum quantizer. It is equal to the qmin parameter in AVCodecContext.
     */
    void setMinimumQuantizer(int qmin);
    int minimumQuantizer() const;

    /*!
     * \brief Maximum quantizer. It is equal to the qmax parameter in AVCodecContext.
     */
    void setMaximumQuantizer(int qmax);
    int maximumQuantizer() const;

    /*!
     * \brief Maximum quantizer difference between frames. It is equal to the max_qdiff parameter in AVCodecContext.
     */
    void setMaximumQuantizerDifference(int qdiff);
    int maximumQuantizerDifference() const;

    /*!
     * \brief It is equal to the coder_type parameter in AVCodecContext.
     */
    void setCoderType(EncoderGlobal::CoderType type);
    EncoderGlobal::CoderType coderType() const;

    /*!
     * \brief Motion estimation comparison function. It is equal to the me_cmp parameter in AVCodecContext.
     */
    void setMotionEstimationComparison(int cmp);
    int motionEstimationComparison() const;

    /*!
     * \brief Macroblock subpartition sizes to consider - p8x8, p4x4, b8x8, i8x8, i4x4. It is equal to the partitions parameter in AVCodecContext.
     */
    void setPartitions(EncoderGlobal::Partitions partitions);
    EncoderGlobal::Partitions partitions() const;

    /*!
     * \brief Motion estimation algorithm used for video coding. It is equal to the me_method parameter in AVCodecContext.
     */
    void setMotionEstimationMethod(EncoderGlobal::MotionEstimationAlgorithm method);
    EncoderGlobal::MotionEstimationAlgorithm motionEstimationMethod() const;

    /*!
     * \brief It is equal to the me_subpel_quality parameter in AVCodecContext.
     */
    void setSubpixelMotionEstimationQuality(int subpel);
    int subpixelMotionEstimationQuality() const;

    /*!
     * \brief Maximum motion estimation search range in subpel units If 0 then no limit. It is equal to the me_range parameter in AVCodecContext.
     */
    void setMotionEstimationRange(int range);
    int motionEstimationRange() const;

    /*!
     * \brief Minimum GOP size. It is equal to the keyint_min parameter in AVCodecContext.
     */
    void setMinimumKeyframeInterval(int interval);
    int minimumKeyframeInterval() const;

    /*!
     * \brief Scene change detection threshold 0 is default, larger means fewer detected scene changes.
     * It is equal to the scenechange_threshold parameter in AVCodecContext.
     */
    void setSceneChangeThreshold(int threshold);
    int sceneChangeThreshold() const;

    /*!
     * \brief It is equal to the i_quant_factor parameter in AVCodecContext.
     */
    void setIQuantFactor(float quantFactor);
    float iQuantFactor() const;

    /*!
     * \brief It is equal to the b_frame_strategy parameter in AVCodecContext.
     */
    void setBFrameStrategy(int strategy);
    int bFrameStrategy() const;

    /*!
     * \brief Amount of qscale change between easy & hard scenes (0.0-1.0). It is equal to the qcompress parameter in AVCodecContext.
     */
    void setQuantizerCurveCompressionFactor(float factor);
    float quantizerCurveCompressionFactor() const;

    /*!
     * \brief Maximum number of B-frames between non-B-frames. It is equal to the max_b_frames parameter in AVCodecContext.
     */
    void setMaximumBFrames(int max);
    int maximumBFrames() const;

    /*!
     * \brief Number of reference frames. It is equal to the refs parameter in AVCodecContext.
     */
    void setReferenceFrameCount(int count);
    int referenceFrameCount() const;

    /*!
     * \brief Constant quantization parameter rate control method. It is equal to the cqp parameter in AVCodecContext.
     */
    void setConstantQuantizerMode(int cqp);
    int constantQuantizerMode() const;

    /*!
     * \brief It is equal to the directpred parameter in AVCodecContext.
     */
    void setDirectMvPredictionMode(EncoderGlobal::MotionVectorPredictionMode mode);
    EncoderGlobal::MotionVectorPredictionMode directMvPredictionMode() const;

    /*!
     * \brief It is equal to the trellis parameter in AVCodecContext.
     */
    void setTrellis(int trellis);
    int trellis() const;

    /*!
     * \brief Explicit P-frame weighted prediction analysis method. It is equal to the weighted_p_pred parameter in AVCodecContext.
     */
    void setPFramePredictionAnalysisMethod(EncoderGlobal::WeightedPredictionMethod method);
    EncoderGlobal::WeightedPredictionMethod pFramePredictionAnalysisMethod() const;

    /*!
     * \brief Number of frames for frametype and ratecontrol lookahead. It is equal to the rc_lookahead parameter in AVCodecContext.
     */
    void setRcLookahead(int value);
    int rcLookahead() const;

    /*!
     * \brief It is equal to the flags parameter in AVCodecContext.
     */
    void setFlags(EncoderGlobal::Flags flags);
    EncoderGlobal::Flags flags() const;

    /*!
     * \brief It is equal to the flags2 parameter in AVCodecContext.
     */
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
    int m_cqp; /*! Constant quantization parameter rate control method. */
    EncoderGlobal::MotionVectorPredictionMode m_directpred; /*!< Direct MV prediction mode. */
    int m_trellis; /*!< Trellis RD quantization. */
    EncoderGlobal::WeightedPredictionMethod m_weightedPPred; /*!< Explicit P-frame weighted prediction analysis method. */
    int m_rcLookahead; /*!< RC lookahead Number of frames for frametype and ratecontrol lookahead. */
    EncoderGlobal::Flags m_flags; /*!< */
    EncoderGlobal::Flags2 m_flags2; /*!< */
};

#endif // VIDEOCODECSETTINGS_H
