#ifndef ENCODERGLOBAL_H
#define ENCODERGLOBAL_H

#include <QFlags>

namespace Encoder {

enum VideoCodec {
    MPEG1 = 0x0001, /*!< MPEG-1 video */
    MPEG2, /*!< MPEG-2 video */
    H261 = 0x0004, /*!< H.261 */
    H263, /*!< H.263 / H.263-1996, H.263+ / H.263-1998 / H.263 version 2 */
    RV10, /*!< RealVideo 1.0 */
    RV20, /*!< RealVideo 2.0 */
    MJPEG, /*!< Motion JPEG */
    LJPEG = 0x000A, /*!< Lossless JPEG */
    JPEGLS = 0x000C, /*!< JPEG-LS */
    MPEG4, /*!< MPEG-4 part 2 */
    RAW, /*!< Raw video */
    MSMPEG4V2 = 0x0010, /*!< MPEG-4 part 2 Microsoft variant version 2 */
    MSMPEG4V3, /*!< MPEG-4 part 2 Microsoft variant version 3 */
    WMV1, /*!< Windows Media Video 7 */
    WMV2, /*!< Windows Media Video 8 */
    H263P, /*!< H.263+ / H.263-1998 / H.263 version 2 */
    FLV1 = 0x0015, /*!< FLV / Sorenson Spark / Sorenson H.263 (Flash Video) */
    SVQ1, /*!< Sorenson Vector Quantizer 1 / Sorenson Video 1 / SVQ1 */
    DVVIDEO = 0x0018, /*!< DV (Digital Video) */
    HUFFYUV, /*!< HuffYUV */
    H264 = 0x001B, /*!< H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10 */
    THEORA = 0x001E, /*!< Theora */
    ASV1, /*!< ASUS V1 */
    ASV2, /*!< ASUS V2 */
    FFV1, /*!< FFmpeg video codec #1 */
    CLJR = 0x0024, /*!< Cirrus Logic AccuPak */
    ROQ = 0x0026, /*!< RoQ video */
    MSVIDEO1 = 0x002E, /*!< Microsoft Video 1 */
    ZLIB = 0x0036, /*!< LCL (LossLess Codec Library) ZLIB */
    QTRLE, /*!< QuickTime Animation (RLE) video */
    SNOW, /*!< Snow */
    PNG = 0x003E, /*!< PNG (Portable Network Graphics) image */
    PPM, /*!< PPM (Portable PixelMap) image */
    PBM, /*!< PBM (Portable BitMap) image */
    PGM, /*!< PGM (Portable GrayMap) image */
    PGMYUV, /*!< PGMYUV (Portable GrayMap YUV) image */
    PAM, /*!< PAM (Portable AnyMap) image */
    FFVHUFF, /*!< Huffyuv FFmpeg variant */
    BMP = 0x004F, /*!< BMP (Windows and OS/2 bitmap) */
    ZMBV = 0x0052, /*!< Zip Motion Blocks Video */
    FLASHSV = 0x0057, /*!< Flash Screen Video v1 */
    CAVS, /*!< Chinese AVS (Audio Video Standard) (AVS1-P2, JiZhun profile) */
    JPEG2000, /*!< JPEG 2000 */
    TARGA = 0x005E, /*!< Truevision Targa image */
    TIFF = 0x0061, /*!< TIFF image */
    GIF, /*!< GIF (Graphics Interchange Format) */
    DNXHD = 0x0065, /*!< VC3/DNxHD */
    SGI = 0x0067, /*!< SGI image */
    AMV = 0x006D, /*!< AMV Video */
    PCX = 0x006F, /*!< PC Paintbrush PCX image */
    SUNRAST, /*!< Sun Rasterfile image */
    DIRAC = 0x0078, /*!< Dirac */
    V210 = 0x0083, /*!< Uncompressed 4:2:2 10-bit */
    DPX, /*!< DPX image */
    FLASHSV2 = 0x0087, /*!< Flash Screen Video v2 */
    R210 = 0x0089, /*!< Uncompressed RGB 10-bit */
    VP8 = 0x0090, /*!< On2 VP8 */
    A64_MULTI = 0x0093, /*!< Multicolor charset for Commodore 64 */
    A64_MULTI5, /*!< Multicolor charset for Commodore 64, extended with 5th color */
    R10K, /*!< AJA Kona 10-bit RGB Codec */
    PRORES = 0x0098 /*!< Apple ProRes (iCodec Pro) */
};

enum AudioCodec {
    PCM_S16LE = 0x10000, /*!< PCM signed 16-bit little-endian */
    PCM_S16BE, /*!< PCM signed 16-bit big-endian */
    PCM_U16LE, /*!< PCM unsigned 16-bit little-endian */
    PCM_U16BE, /*!< PCM unsigned 16-bit big-endian */
    PCM_S8, /*!< PCM signed 8-bit */
    PCM_U8, /*!< PCM unsigned 8-bit */
    PCM_MULAW, /*!< PCM mu-law / G.711 mu-law */
    PCM_ALAW, /*!< PCM A-law / G.711 A-law */
    PCM_S32LE, /*!< PCM signed 32-bit little-endian */
    PCM_S32BE, /*!< PCM signed 32-bit big-endian */
    PCM_U32LE, /*!< PCM unsigned 32-bit little-endian */
    PCM_U32BE, /*!< PCM unsigned 32-bit big-endian */
    PCM_S24LE, /*!< PCM signed 24-bit little-endian */
    PCM_S24BE, /*!< PCM signed 24-bit big-endian */
    PCM_U24LE, /*!< PCM unsigned 24-bit little-endian */
    PCM_U24BE, /*!< PCM unsigned 24-bit big-endian */
    PCM_S24DAUD, /*!< PCM D-Cinema audio signed 24-bit */
    PCM_S16LE_PLANAR = 0x10012, /*!< PCM signed 16-bit little-endian planar */
    PCM_F32BE = 0x10014, /*!< PCM 32-bit floating point big-endian */
    PCM_F32LE, /*!< PCM 32-bit floating point little-endian */
    PCM_F64BE, /*!< PCM 64-bit floating point big-endian */
    PCM_F64LE, /*!< PCM 64-bit floating point little-endian */
    ADPCM_IMA_QT = 0x11000, /*!< ADPCM IMA QuickTime */
    ADPCM_IMA_WAV, /*!< ADPCM IMA WAV */
    ADPCM_MS = 0x11006, /*!< ADPCM Microsoft */
    ADPCM_ADX = 0x11009, /*!< SEGA CRI ADX ADPCM */
    ADPCM_G726 = 0x1100B, /*!< G.726 ADPCM */
    ADPCM_SWF = 0x1100D, /*!< ADPCM Shockwave Flash */
    ADPCM_YAMAHA, /*!< ADPCM Yamaha */
    ADPCM_G722 = 0x1101C, /*!< G.722 ADPCM */
    AMR_NB = 0x12000, /*!< AMR-NB (Adaptive Multi-Rate NarrowBand) */
    AMR_WB, /*!< AMR-WB (Adaptive Multi-Rate WideBand) */
    RA_144 = 0x13000, /*!< RealAudio 1.0 (14.4K) */
    ROQ_DPCM = 0x14000, /*!< DPCM id RoQ */
    MP2 = 0x15000, /*!< MP2 (MPEG audio layer 2) */
    MP3, /*!< MP3 (MPEG audio layer 3) */
    AAC, /*!< AAC (Advanced Audio Coding) */
    AC3, /*!< ATSC A/52A (AC-3) */
    DTS, /*!< DCA (DTS Coherent Acoustics) */
    VORBIS, /*!< Vorbis */
    WMAV1 = 0x15007, /*!< Windows Media Audio 1 */
    WMAV2, /*!< Windows Media Audio 2 */
    SONIC = 0x1500C, /*!< Sonic */
    SONIC_LS, /*!< Sonic lossless */
    FLAC, /*!< FLAC (Free Lossless Audio Codec) */
    ALAC = 0x15012, /*!< ALAC (Apple Lossless Audio Codec) */
    GSM = 0x15014, /*!< Gsm */
    GSM_MS = 0x15020, /*!< GSM Microsoft variant */
    NELLYMOSER = 0x15024, /*!< Nellymoser Asao */
    SPEEX = 0x15026, /*!< Speex */
    EAC3 = 0x1502B /*!< ATSC A/52B (AC-3, E-AC-3) */
};

enum CoderType {
    Vlc = 0,
    Ac,
    Raw,
    Rle,
    Deflate
};

enum Partition {
    I4x4 = 0x001, /*!< Analyze i4x4 */
    I8x8 = 0x002, /*!< Analyze i8x8 (requires 8x8 transform) */
    P8x8 = 0x010, /*!< Analyze p16x8, p8x16 and p8x8 */
    P4x4 = 0x020, /*!< Analyze p8x4, p4x8, p4x4 */
    B8x8 = 0x100 /*!< Analyze b16x8, b8x16 and b8x8 */
};
Q_DECLARE_FLAGS(Partitions, Partition)

enum MotionEstimationAlgorithm {
    Zero = 1,
    Full,
    Log,
    Phods,
    Epzs,
    X1,
    Hex,
    Umh,
    Iter,
    Tesa
};

enum MotionVectorPredictionMode {
    NoMvpm = 0,
    SpatialMode,
    TemporalMode,
    AutoMode
};

enum WeightedPredictionMethod {
    NoWpm = 0,
    FastBlindMethod, /*!< One reference duplicate with -1 offset. */
    SmartMethod /*!< Full fade detection analysis. */
};

enum Flag {
    QScale = 0x0002,
    Mv4 = 0x0004, /*!< Use four motion vector by macroblock (mpeg4). */
    QuarterPixel = 0x0010,
    Gmc = 0x0020,
    Mv0 = 0x0040,
    Part = 0x0080, /*!< Use data partitioning. */
    InputPreserved = 0x0100,
    Pass1 = 0x0200,
    Pass2 = 0x0400,
    ExternHuff = 0x1000,
    Gray = 0x2000,
    EmuEdge = 0x4000,
    Psnr = 0x8000,
    Truncated = 0x00010000,
    NormalizeAqp = 0x00020000,
    InterlacedDct = 0x00040000,
    LowDelay = 0x00080000, /*!< Force low delay. */
    AltScan = 0x00100000,
    GlobalHeader = 0x00400000,
    BitExact = 0x00800000,
    ACPred = 0x01000000,
    H263pUmv = 0x02000000,
    CbpRateDistortion = 0x04000000, /*!< Use rate distortion optimization for Coded Block Pattern. */
    QpRd = 0x08000000,
    H263pAiv = 0x00000008,
    Obmc = 0x00000001, /*!< Use overlapped block motion compensation (h263+). */
    LoopFilter = 0x00000800, /*!< Use loop filter. */
    H263pSpliceStruct = 0x10000000,
    InterlacedMe = 0x20000000,
    SvcdScanOffset = 0x40000000,
    ClosedGop = 0x80000000
};
Q_DECLARE_FLAGS(Flags, Flag)

enum Flag2 {
    Fast = 0x00000001,
    StrictGop = 0x00000002,
    NoOutput = 0x00000004,
    LocalHeader = 0x00000008,
    BPyramid = 0x00000010,
    WPred = 0x00000020,
    MixedRefs = 0x00000040,
    Dct8x8 = 0x00000080,
    FastPSkip = 0x00000100,
    Aud = 0x00000200,
    Brdo = 0x00000400,
    IntraVlc = 0x00000800,
    MemcOnly = 0x00001000,
    DropFrameTimecode = 0x00002000,
    SkipRd = 0x00004000,
    Chunks = 0x00008000,
    NonLinearQuant = 0x00010000,
    BitReservoir = 0x00020000,
    MbTree = 0x00040000,
    Psy = 0x00080000,
    SSim = 0x00100000,
    IntraRefresh = 0x00200000
};
Q_DECLARE_FLAGS(Flags2, Flag2)

}

Q_DECLARE_OPERATORS_FOR_FLAGS(Encoder::Partitions)
Q_DECLARE_OPERATORS_FOR_FLAGS(Encoder::Flags)
Q_DECLARE_OPERATORS_FOR_FLAGS(Encoder::Flags2)

#endif // ENCODERGLOBAL_H
