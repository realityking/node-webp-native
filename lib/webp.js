'use strict'

const { dirname } = require('path')
const binding = require('node-gyp-build')(dirname(__dirname))

const presetMap = {
  default: 1,
  photo: 2,
  picture: 3,
  drawing: 4,
  icon: 5,
  text: 6
};
const availablePresets = Object.keys(presetMap);

function presetStringToNumber(preset = 'default') {
  if (!preset) {
    // Undefined values become the default preset
    return 1;
  }
  if (!presetMap[preset]) {
    throw new Error('Unknown preset "' + preset + '"')
  }

  return presetMap[preset];
}

function convertToWebpSync (input, options = {}) {
  if (!Buffer.isBuffer(input)) {
    throw new TypeError('input must be a buffer')
  }

  return binding.convertToWebpSync(input, {
    ...options,
    preset: presetStringToNumber(options.preset)
  })
}

function convertToWebp (input, options = {}) {
  if (!Buffer.isBuffer(input)) {
    return Promise.reject(new TypeError('input must be a buffer'))
  }
}

module.exports = {
  convertToWebpSync,
  convertToWebp
}
