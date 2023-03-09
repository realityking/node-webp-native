{
  'targets': [
    {
      'target_name': 'webp',
      'sources': [
        'src/libwebp/imageio/imageio_util.c',
        'src/libwebp/imageio/jpegdec.c',
        'src/libwebp/imageio/metadata.c',
        'src/libwebp/imageio/pngdec.c',
        'src/libwebp/imageio/tiffdec.c',
        'src/libwebp/imageio/webpdec.c',

        'src/libwebp/sharpyuv/sharpyuv.c',
        'src/libwebp/sharpyuv/sharpyuv_cpu.c',
        'src/libwebp/sharpyuv/sharpyuv_csp.c',
        'src/libwebp/sharpyuv/sharpyuv_dsp.c',
        'src/libwebp/sharpyuv/sharpyuv_gamma.c',
        'src/libwebp/sharpyuv/sharpyuv_neon.c',
        'src/libwebp/sharpyuv/sharpyuv_sse2.c',

        'src/libwebp/src/dec/alpha_dec.c',
        'src/libwebp/src/dec/buffer_dec.c',
        'src/libwebp/src/dec/frame_dec.c',
        'src/libwebp/src/dec/idec_dec.c',
        'src/libwebp/src/dec/io_dec.c',
        'src/libwebp/src/dec/quant_dec.c',
        'src/libwebp/src/dec/tree_dec.c',
        'src/libwebp/src/dec/vp8_dec.c',
        'src/libwebp/src/dec/vp8l_dec.c',
        'src/libwebp/src/dec/webp_dec.c',

        'src/libwebp/src/demux/anim_decode.c',
        'src/libwebp/src/demux/demux.c',

        'src/libwebp/src/dsp/alpha_processing.c',
        'src/libwebp/src/dsp/alpha_processing_neon.c',
        'src/libwebp/src/dsp/cost.c',
        'src/libwebp/src/dsp/cost_neon.c',
        'src/libwebp/src/dsp/cpu.c',
        'src/libwebp/src/dsp/dec_clip_tables.c',
        'src/libwebp/src/dsp/dec.c',
        'src/libwebp/src/dsp/dec_neon.c',
        'src/libwebp/src/dsp/enc.c',
        'src/libwebp/src/dsp/enc_neon.c',
        'src/libwebp/src/dsp/filters.c',
        'src/libwebp/src/dsp/filters_neon.c',
        'src/libwebp/src/dsp/lossless.c',
        'src/libwebp/src/dsp/lossless_neon.c',
        'src/libwebp/src/dsp/lossless_enc.c',
        'src/libwebp/src/dsp/lossless_enc_neon.c',
        'src/libwebp/src/dsp/rescaler.c',
        'src/libwebp/src/dsp/rescaler_neon.c',
        'src/libwebp/src/dsp/ssim.c',
        'src/libwebp/src/dsp/upsampling.c',
        'src/libwebp/src/dsp/upsampling_neon.c',
        'src/libwebp/src/dsp/yuv.c',
        'src/libwebp/src/dsp/yuv_neon.c',

        'src/libwebp/src/enc/alpha_enc.c',
        'src/libwebp/src/enc/analysis_enc.c',
        'src/libwebp/src/enc/backward_references_cost_enc.c',
        'src/libwebp/src/enc/backward_references_enc.c',
        'src/libwebp/src/enc/config_enc.c',
        'src/libwebp/src/enc/cost_enc.c',
        'src/libwebp/src/enc/filter_enc.c',
        'src/libwebp/src/enc/frame_enc.c',
        'src/libwebp/src/enc/histogram_enc.c',
        'src/libwebp/src/enc/iterator_enc.c',
        'src/libwebp/src/enc/near_lossless_enc.c',
        'src/libwebp/src/enc/picture_csp_enc.c',
        'src/libwebp/src/enc/picture_enc.c',
        'src/libwebp/src/enc/picture_psnr_enc.c',
        'src/libwebp/src/enc/picture_rescale_enc.c',
        'src/libwebp/src/enc/picture_tools_enc.c',
        'src/libwebp/src/enc/predictor_enc.c',
        'src/libwebp/src/enc/quant_enc.c',
        'src/libwebp/src/enc/syntax_enc.c',
        'src/libwebp/src/enc/token_enc.c',
        'src/libwebp/src/enc/tree_enc.c',
        'src/libwebp/src/enc/vp8l_enc.c',
        'src/libwebp/src/enc/webp_enc.c',

        'src/libwebp/src/mux/anim_encode.c',
        'src/libwebp/src/mux/muxedit.c',
        'src/libwebp/src/mux/muxinternal.c',
        'src/libwebp/src/mux/muxread.c',

        'src/libwebp/src/utils/bit_reader_utils.c',
        'src/libwebp/src/utils/bit_writer_utils.c',
        'src/libwebp/src/utils/color_cache_utils.c',
        'src/libwebp/src/utils/filters_utils.c',
        'src/libwebp/src/utils/huffman_encode_utils.c',
        'src/libwebp/src/utils/huffman_utils.c',
        'src/libwebp/src/utils/quant_levels_dec_utils.c',
        'src/libwebp/src/utils/quant_levels_utils.c',
        'src/libwebp/src/utils/random_utils.c',
        'src/libwebp/src/utils/rescaler_utils.c',
        'src/libwebp/src/utils/thread_utils.c',
        'src/libwebp/src/utils/utils.c',

        'src/image_reader.cc',
        'src/webp.cc',
      ],
      'include_dirs': [
        'src',
        'src/libwebp',
        'src/libwebp/src',
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'dependencies': [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      'cflags': [
        '-fno-exceptions',
        '-Wextra',
        '-Wall',
        '-Wno-unused-function',
        '-Wno-unused-parameter',
        '-std=c99',
        '-pedantic',
        '-O3',
        '-flto',
      ],
      'cflags_cc': [
        '-fno-exceptions',
        '-Wextra',
        '-Wall',
        '-std=c++11',
        '-pedantic',
        '-O3',
        '-flto',
      ],
      'ldflags': [
        '-flto'
      ],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'NO',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
        'OTHER_CFLAGS': [
          '-fno-exceptions',
          '-Wextra',
          '-Wall',
          '-Wno-unused-function',
          '-Wno-unused-parameter',
          '-std=c99',
          '-pedantic',
          '-O3',
          '-flto'
        ],
        'OTHER_CPLUSPLUSFLAGS': [
          '-fno-exceptions',
          '-Wextra',
          '-Wall',
          '-std=c++11',
          '-pedantic',
          '-O3',
          '-flto'
        ],
        'OTHER_LDFLAGS': [
          '-flto'
        ]
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS",
        "NAPI_VERSION=3",
        "NDEBUG"
      ],
    }
  ]
}
