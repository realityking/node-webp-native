'use strict'

const fs = require('fs/promises')
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

//test('sync: convert a WebP image "noAlpha" option', async t => {
  //@todo implement test
//})

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
