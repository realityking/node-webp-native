'use strict'

const fs = require('fs/promises')
const sharp = require('sharp')
const path = require('path')

const test = require('ava')

const { convertToWebp, convertToWebpSync } = require('..')

test('sync: convert a WebP image', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf)

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "sharpness" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { sharpness: 7 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "sns" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { sns: 20 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "autoFilter" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { autoFilter: true })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "method" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { method: 2 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "alphaMethod" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { alphaMethod: 0 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "filter" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { filter: 30 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "preset" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { preset: 'drawing' })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "hint" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { hint: 'photo' })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "size" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { size: 5000 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "pass" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { pass: 4 })

  t.true(data.length < buf.length)
})

test('sync: convert a WebP image "lossless" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { lossless: true })

  t.false(data.length < buf.length)
})

test('sync: convert a WebP image "losslessPreset" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { losslessPreset: 2 })

  t.false(data.length < buf.length)
})

test('sync: convert a WebP image "nearLossless" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf, { nearLossless: 1 })

  t.false(data.length < buf.length)
})

test('sync: strip exif metadata by default', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/exif.webp'))
  const originalImage = sharp(buf)
  const originalMetadata = await originalImage.metadata()

  // Make sure the source image actually has exif data
  t.true(Buffer.isBuffer(originalMetadata.exif))

  const data = convertToWebpSync(buf)
  const convertedImage = sharp(data)
  const convertedMetadata = await convertedImage.metadata()

  t.is(convertedMetadata.exif, undefined)
})

test('sync: keep exif metadata when preserving all metadata', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/exif.webp'))
  const originalImage = sharp(buf)
  const originalMetadata = await originalImage.metadata()

  // Make sure the source image actually has exif data
  t.true(Buffer.isBuffer(originalMetadata.exif))

  const data = convertToWebpSync(buf, { metadata: 'all' })
  const convertedImage = sharp(data)
  const convertedMetadata = await convertedImage.metadata()

  // @todo uncomment when functionality is implemented
  // t.true(Buffer.isBuffer(convertedMetadata.exif))
})

test('sync: keep exif metadata when preserving only exif', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/exif.webp'))
  const originalImage = sharp(buf)
  const originalMetadata = await originalImage.metadata()

  // Make sure the source image actually has exif data
  t.true(Buffer.isBuffer(originalMetadata.exif))

  const data = convertToWebpSync(buf, { metadata: ['exif'] })
  const convertedImage = sharp(data)
  const convertedMetadata = await convertedImage.metadata()

  // @todo uncomment when functionality is implemented
  // t.true(Buffer.isBuffer(convertedMetadata.exif))
})

test('sync: convert a WebP image "noAlpha" option', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/3_webp_a.webp'))
  const originalImage = sharp(buf)
  const originalMetadata = await originalImage.metadata()

  // Make sure we test with an image that has alpha transparency
  t.true(originalMetadata.hasAlpha)

  const data = convertToWebpSync(buf, { noAlpha: true })
  const convertedImage = sharp(data)
  const convertedMetadata = await convertedImage.metadata()

  t.false(convertedMetadata.hasAlpha)
})

test('sync: throws error if input is not a buffer', t => {
  t.throws(() => {
    convertToWebpSync('nope')
  }, {
    instanceOf: TypeError,
    message: 'input must be a buffer'
  })
})

// @todo fix me
/* test('sync: throws error if options is not an object', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  convertToWebpSync(buf, 'nope')

  t.throws(() => {
    convertToWebpSync(buf, 'nope')
  }, {
    instanceOf: TypeError,
    message: 'options must be an object'
  })
}) */

test('sync: throws error if option value has wrong type', async t => {
  const buf = await fs.readFile(path.join(__dirname, 'fixtures/1.webp'))
  t.throws(() => {
    convertToWebpSync(buf, { sharpness: 'a lot' })
  }, {
    instanceOf: TypeError,
    message: 'Wrong type for option \'sharpness\''
  })
})
