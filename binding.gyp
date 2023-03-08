{
  'targets': [
    {
      'target_name': 'webp',
      'sources': [
        'src/webp.cc',
      ],
      'include_dirs': [
        'src',
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
        '-ansi',
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
          '-ansi',
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
