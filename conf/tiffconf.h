#ifndef _TIFFCONF_
#define _TIFFCONF_

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

/* Signed 16-bit type */
/* #undef TIFF_INT16_T */

/* Signed 32-bit type */
/* #undef TIFF_INT32_T */

/* Signed 64-bit type */
/* #undef TIFF_INT64_T */

/* Signed 8-bit type */
/* #undef TIFF_INT8_T */

/* Unsigned 16-bit type */
/* #undef TIFF_UINT16_T */

/* Unsigned 32-bit type */
/* #undef TIFF_UINT32_T */

/* Unsigned 64-bit type */
/* #undef TIFF_UINT64_T */

/* Unsigned 8-bit type */
/* #undef TIFF_UINT8_T */

/* Signed size type */
#undef TIFF_SSIZE_T
#if __SIZEOF_SIZE_T__ == 8
#  define TIFF_SSIZE_T int64_t
#elif __SIZEOF_SIZE_T__ == 4
#  define TIFF_SSIZE_T int32_t
#else
#  error "Unsupported size_t size; please submit a bug report"
#endif

/* Compatibility stuff. */

/* Define as 0 or 1 according to the floating point format supported by the
   machine */
#define HAVE_IEEEFP 1

/* Set the native cpu bit order (FILLORDER_LSB2MSB or FILLORDER_MSB2LSB) */
#define HOST_FILLORDER FILLORDER_MSB2LSB

/* Native cpu byte order: 1 if big-endian (Motorola) or 0 if little-endian
   (Intel) */
// some endian fix (e.g.: mips-gcc doesn't define __BIG_ENDIAN__)
#if !defined(WORDS_BIGENDIAN) &&                   \
    (defined(__BIG_ENDIAN__) || defined(_M_PPC) || \
     (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)))
  #define HOST_BIGENDIAN 1
#else 
  #define HOST_BIGENDIAN 0
#endif

/* Support CCITT Group 3 & 4 algorithms */
#undef CCITT_SUPPORT

/* Support JPEG compression (requires IJG JPEG library) */
#undef JPEG_SUPPORT

/* Support JBIG compression (requires JBIG-KIT library) */
#undef JBIG_SUPPORT

/* Support LERC compression */
#undef LERC_SUPPORT

/* Support LogLuv high dynamic range encoding */
#undef LOGLUV_SUPPORT

/* Support LZW algorithm */
#undef LZW_SUPPORT

/* Support NeXT 2-bit RLE algorithm */
#undef NEXT_SUPPORT

/* Support Old JPEG compresson (read contrib/ojpeg/README first! Compilation
   fails with unpatched IJG JPEG library) */
#undef OJPEG_SUPPORT

/* Support Macintosh PackBits algorithm */
#undef PACKBITS_SUPPORT

/* Support Pixar log-format algorithm (requires Zlib) */
#undef PIXARLOG_SUPPORT

/* Support ThunderScan 4-bit RLE algorithm */
#undef THUNDER_SUPPORT

/* Support Deflate compression */
#undef ZIP_SUPPORT

/* Support libdeflate enhanced compression */
#undef LIBDEFLATE_SUPPORT

/* Support strip chopping (whether or not to convert single-strip uncompressed
   images to multiple strips of ~8Kb to reduce memory usage) */
#undef STRIPCHOP_DEFAULT

/* Enable SubIFD tag (330) support */
#undef SUBIFD_SUPPORT

/* Treat extra sample as alpha (default enabled). The RGBA interface will
   treat a fourth sample with no EXTRASAMPLE_ value as being ASSOCALPHA. Many
   packages produce RGBA files but don't mark the alpha properly. */
#undef DEFAULT_EXTRASAMPLE_AS_ALPHA

/* Pick up YCbCr subsampling info from the JPEG data stream to support files
   lacking the tag (default enabled). */
#undef CHECK_JPEG_YCBCR_SUBSAMPLING

/* Support MS MDI magic number files as TIFF */
#undef MDI_SUPPORT

/*
 * Feature support definitions.
 * XXX: These macros are obsoleted. Don't use them in your apps!
 * Macros stays here for backward compatibility and should be always defined.
 */
#define COLORIMETRY_SUPPORT
#define YCBCR_SUPPORT
#define CMYK_SUPPORT
#define ICC_SUPPORT
#define PHOTOSHOP_SUPPORT
#define IPTC_SUPPORT

#endif /* _TIFFCONF_ */