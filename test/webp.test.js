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

test('sync: throws error if input is not a buffer', t => {
  t.throws(() => {
    convertToWebpSync('nope')
  }, {
    instanceOf: TypeError,
    message: 'input must be a buffer'
  })
})
