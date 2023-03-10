import { readFile } from 'fs/promises'
import path from 'path'
import { fileURLToPath } from 'url'

import test from 'ava'

import { convertToWebpSync, convertToWebp } from 'node-webp-native'

const __dirname = path.dirname(fileURLToPath(import.meta.url))

test('ESM sync: convert a WebP image', async t => {
  const buf = await readFile(path.join(__dirname, 'fixtures/1.webp'))
  const data = convertToWebpSync(buf)

  t.true(data.length < buf.length)
})
