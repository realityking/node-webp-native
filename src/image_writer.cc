#include "./image_writer.h"

static const int kChunkHeaderSize = 8;

// Sets 'flag' in 'vp8x_flags' and updates 'metadata_size' with the size of the
// chunk if there is metadata and 'keep' is true.
static int UpdateFlagsAndSize(const MetadataPayload* const payload, int keep,
                              int flag, uint32_t* vp8x_flags,
                              uint64_t* metadata_size) {
  if (keep && payload->bytes != NULL && payload->size > 0) {
    *vp8x_flags |= flag;
    *metadata_size += kChunkHeaderSize + payload->size + (payload->size & 1);
    return 1;
  }
  return 0;
}

WebPMuxError create_output_buffer(WebPData* output_data,
                                  const WebPData* input_data,
                                  const Metadata* const metadata,
                                  const int keep_metadata) {
  if (!keep_metadata) {
    int ok = WebPDataCopy(input_data, output_data);
    if (!ok) {
      return WEBP_MUX_MEMORY_ERROR;
    }
  }

  const int kAlphaFlag = 0x10;
  const int kEXIFFlag = 0x08;
  const int kICCPFlag = 0x20;
  const int kXMPFlag = 0x04;
  WebPMuxError muxError = WEBP_MUX_OK;
  int copy_data = 1;
  uint32_t flags = 0;
  uint64_t metadata_size = 0;

  const int write_exif =
      UpdateFlagsAndSize(&metadata->exif, !!(keep_metadata & METADATA_EXIF),
                         kEXIFFlag, &flags, &metadata_size);
  const int write_iccp =
      UpdateFlagsAndSize(&metadata->iccp, !!(keep_metadata & METADATA_ICC),
                         kICCPFlag, &flags, &metadata_size);
  const int write_xmp =
      UpdateFlagsAndSize(&metadata->xmp, !!(keep_metadata & METADATA_XMP),
                         kXMPFlag, &flags, &metadata_size);

  WebPMux* mux = WebPMuxCreate(input_data, copy_data);
  if (write_exif) {
    muxError =
        WebPMuxSetChunk(mux, "EXIF", (WebPData*)&metadata->exif, copy_data);
    if (muxError != WEBP_MUX_OK) {
      return muxError;
    }
  }
  if (write_iccp) {
    muxError =
        WebPMuxSetChunk(mux, "ICCP", (WebPData*)&metadata->iccp, copy_data);
    if (muxError != WEBP_MUX_OK) {
      return muxError;
    }
  }
  if (write_xmp) {
    muxError =
        WebPMuxSetChunk(mux, "XMP ", (WebPData*)&metadata->xmp, copy_data);
    if (muxError != WEBP_MUX_OK) {
      return muxError;
    }
  }
  // @todo investigate if any handling of alpha channels is necessary

  muxError = WebPMuxAssemble(mux, output_data);
  if (muxError != WEBP_MUX_OK) {
    return muxError;
  }
  WebPMuxDelete(mux);

  return WEBP_MUX_OK;
}