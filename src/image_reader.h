#include "imageio/jpegdec.h"
#include "imageio/metadata.h"
#include "imageio/pngdec.h"
#include "imageio/tiffdec.h"
#include "imageio/webpdec.h"
#include "webp/types.h"

extern "C" {

typedef enum {
  WEBP_PNG_FORMAT = 0,
  WEBP_JPEG_FORMAT,
  WEBP_TIFF_FORMAT,
  WEBP_WEBP_FORMAT,
  WEBP_UNSUPPORTED_FORMAT
} WebPInputFileFormat;

// Try to infer the image format. 'data_size' should be larger than 12.
// Returns WEBP_UNSUPPORTED_FORMAT if format can't be guess safely.
WebPInputFileFormat WebPGuessImageType(const uint8_t* const data,
                                       size_t data_size);

// Signature for common image-reading functions (ReadPNG, ReadJPEG, ...)
typedef int (*WebPImageReader)(const uint8_t* const data, size_t data_size,
                               struct WebPPicture* const pic, int keep_alpha,
                               struct Metadata* const metadata);

// Return the reader associated to a given file format.
WebPImageReader WebPGetImageReader(WebPInputFileFormat format);

// This function is similar to WebPGuessImageType(), but returns a
// suitable reader function. The returned reader is never NULL, but
// unknown formats will return an always-failing valid reader.
WebPImageReader WebPGuessImageReader(const uint8_t* const data,
                                     size_t data_size);
}
