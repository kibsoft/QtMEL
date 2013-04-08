#ifndef ENCODERGLOBAL_H
#define ENCODERGLOBAL_H

#include <QFlags>

namespace EncoderGlobal {

enum VideoCodec {
    DEFAULT_VIDEO_CODEC = 0x0000,
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
    DEFAULT_AUDIO_CODEC = 0x0000,
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

enum PixelFormat {
    NONE = -1,
    YUV420P, /*!< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples) */
    YUYV422, /*!< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr */
    RGB24, /*!< packed RGB 8:8:8, 24bpp, RGBRGB... */
    BGR24, /*!< packed RGB 8:8:8, 24bpp, BGRBGR... */
    YUV422P, /*!< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples) */
    YUV444P, /*!< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples) */
    YUV410P, /*!< planar YUV 4:1:0, 9bpp, (1 Cr & Cb sample per 4x4 Y samples) */
    YUV411P, /*!< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) */
    GRAY8, /*!< Y , 8bpp. */
    MONOWHITE, /*!< Y , 1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb. */
    MONOBLACK, /*!< Y , 1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb. */
    PAL8, /*!< 8 bit with PIX_FMT_RGB32 palette */
    YUVJ420P, /*!< planar YUV 4:2:0, 12bpp, full scale (JPEG) */
    YUVJ422P, /*!< planar YUV 4:2:2, 16bpp, full scale (JPEG) */
    YUVJ444P, /*!< planar YUV 4:4:4, 24bpp, full scale (JPEG) */
    XVMC_MPEG2_MC, /*!< XVideo Motion Acceleration via common packet passing. */
    XVMC_MPEG2_IDCT,
    UYVY422, /*!< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1 */
    UYYVYY411, /*!< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3 */
    BGR8, /*!< packed RGB 3:3:2, 8bpp, (msb)2B 3G 3R(lsb) */
    BGR4, /*!< packed RGB 1:2:1 bitstream, 4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits */
    BGR4_BYTE, /*!< packed RGB 1:2:1, 8bpp, (msb)1B 2G 1R(lsb) */
    RGB8, /*!< packed RGB 3:3:2, 8bpp, (msb)2R 3G 3B(lsb) */
    RGB4, /*!< packed RGB 1:2:1 bitstream, 4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits */
    RGB4_BYTE, /*!< packed RGB 1:2:1, 8bpp, (msb)1R 2G 1B(lsb) */
    NV12, /*!< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V) */
    NV21, /*!< as above, but U and V bytes are swapped */
    ARGB, /*!< packed ARGB 8:8:8:8, 32bpp, ARGBARGB... */
    RGBA, /*!< packed RGBA 8:8:8:8, 32bpp, RGBARGBA... */
    ABGR, /*!< packed ABGR 8:8:8:8, 32bpp, ABGRABGR... */
    BGRA, /*!< packed BGRA 8:8:8:8, 32bpp, BGRABGRA... */
    GRAY16BE, /*!< Y , 16bpp, big-endian. */
    GRAY16LE, /*!< Y , 16bpp, little-endian. */
    YUV440P, /*!< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples) */
    YUVJ440P, /*!< planar YUV 4:4:0 full scale (JPEG) */
    YUVA420P, /*!< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples) */
    VDPAU_H264, /*!< H.264 HW decoding with VDPAU */
    VDPAU_MPEG1, /*!< MPEG-1 HW decoding with VDPAU */
    VDPAU_MPEG2, /*!< MPEG-2 HW decoding with VDPAU */
    VDPAU_WMV3, /*!< WMV3 HW decoding with VDPAU */
    VDPAU_VC1, /*!< VC-1 HW decoding with VDPAU */
    RGB48BE, /*!< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian */
    RGB48LE, /*!< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian */
    RGB565BE, /*!< packed RGB 5:6:5, 16bpp, (msb) 5R 6G 5B(lsb), big-endian */
    RGB565LE, /*!< packed RGB 5:6:5, 16bpp, (msb) 5R 6G 5B(lsb), little-endian */
    RGB555BE, /*!< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), big-endian, most significant bit to 0 */
    RGB555LE, /*!< packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), little-endian, most significant bit to 0 */
    BGR565BE, /*!< packed BGR 5:6:5, 16bpp, (msb) 5B 6G 5R(lsb), big-endian */
    BGR565LE, /*!< packed BGR 5:6:5, 16bpp, (msb) 5B 6G 5R(lsb), little-endian */
    BGR555BE, /*!< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), big-endian, most significant bit to 1 */
    BGR555LE, /*!< packed BGR 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), little-endian, most significant bit to 1 */
    VAAPI_MOCO, /*!< HW acceleration through VA API at motion compensation entry-point */
    VAAPI_IDCT, /*!< HW acceleration through VA API at IDCT entry-point */
    VAAPI_VLD, /*!< HW decoding through VA API */
    YUV420P16LE, /*!< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian */
    YUV420P16BE, /*!< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian */
    YUV422P16LE, /*!< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian */
    YUV422P16BE, /*!< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian */
    YUV444P16LE, /*!< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian */
    YUV444P16BE, /*!< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian */
    VDPAU_MPEG4, /*!< MPEG4 HW decoding with VDPAU */
    DXVA2_VLD, /*!< HW decoding through DXVA2 */
    RGB444BE, /*!< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), big-endian, most significant bits to 0 */
    RGB444LE, /*!< packed RGB 4:4:4, 16bpp, (msb)4A 4R 4G 4B(lsb), little-endian, most significant bits to 0 */
    BGR444BE, /*!< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), big-endian, most significant bits to 1 */
    BGR444LE, /*!< packed BGR 4:4:4, 16bpp, (msb)4A 4B 4G 4R(lsb), little-endian, most significant bits to 1 */
    Y400A /*!< 8bit gray, 8bit alpha */
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

Q_DECLARE_OPERATORS_FOR_FLAGS(EncoderGlobal::Partitions)
Q_DECLARE_OPERATORS_FOR_FLAGS(EncoderGlobal::Flags)
Q_DECLARE_OPERATORS_FOR_FLAGS(EncoderGlobal::Flags2)

#endif // ENCODERGLOBAL_H
