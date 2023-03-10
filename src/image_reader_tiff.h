#ifndef WEBP_READER_TIFF_H_
#define WEBP_READER_TIFF_H_

#include "webp/types.h"

struct Metadata;
struct WebPPicture;

// Reads a TIFF from 'data', returning the decoded output in 'pic'.
// Output is RGBA or YUVA, depending on pic->use_argb value.
// If 'keep_alpha' is true and the TIFF has an alpha channel, the output is RGBA
// or YUVA. Otherwise, alpha channel is dropped and output is RGB or YUV.
// Returns true on success.
int ReadTIFF(const uint8_t* const data, size_t data_size,
             struct WebPPicture* const pic, int keep_alpha,
             struct Metadata* const metadata);

#endif  // WEBP_READER_TIFF_H_
