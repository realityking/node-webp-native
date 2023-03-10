#include "imageio/metadata.h"
#include "webp/mux.h"

enum {
  METADATA_EXIF = (1 << 0),
  METADATA_ICC = (1 << 1),
  METADATA_XMP = (1 << 2),
  METADATA_ALL = METADATA_EXIF | METADATA_ICC | METADATA_XMP
};

WebPMuxError create_output_buffer(WebPData* output_data,
                                  const WebPData* input_data,
                                  const Metadata* const metadata,
                                  const int keep_metadata);
