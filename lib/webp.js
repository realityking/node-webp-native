'use strict'

const { dirname } = require('path')
const binding = require('node-gyp-build')(dirname(__dirname))

function convertToWebpSync (input, options = {}) {
  if (!Buffer.isBuffer(input)) {
    throw new TypeError('input must be a buffer')
  }

  return binding.convertToWebpSync(input, options)
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
