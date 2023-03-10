#ifndef WEBP_READER_WEBP_H_
#define WEBP_READER_WEBP_H_

#include "webp/decode.h"

struct Metadata;
struct WebPPicture;

//------------------------------------------------------------------------------
// WebP decoding

// Prints an informative error message regarding decode failure of 'in_file'.
// 'status' is treated as a VP8StatusCode and if valid will be printed as a
// text string.
void PrintWebPError(const char* const in_file, int status);

// Decodes the WebP contained in 'data'.
// 'config' is a structure previously initialized by WebPInitDecoderConfig().
// 'config->output' should have the desired colorspace selected.
// Returns the decoder status. On success 'config->output' will contain the
// decoded picture.
VP8StatusCode DecodeWebP(const uint8_t* const data, size_t data_size,
                         WebPDecoderConfig* const config);

// Same as DecodeWebP(), but using the incremental decoder.
VP8StatusCode DecodeWebPIncremental(
    const uint8_t* const data, size_t data_size,
    WebPDecoderConfig* const config);

//------------------------------------------------------------------------------

// Decodes a WebP contained in 'data', returning the decoded output in 'pic'.
// Output is RGBA or YUVA, depending on pic->use_argb value.
// If 'keep_alpha' is true and the WebP has an alpha channel, the output is RGBA
// or YUVA. Otherwise, alpha channel is dropped and output is RGB or YUV.
// Returns true on success.
int ReadWebP(const uint8_t* const data, size_t data_size,
             struct WebPPicture* const pic,
             int keep_alpha, struct Metadata* const metadata);

#endif  // WEBP_READER_WEBP_H_
