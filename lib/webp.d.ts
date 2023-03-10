export type Options = {
  preset?: 'default'|'photo'|'picture'|'drawing'|'icon'|'text'
  hint?: 'photo'|'picture'|'graph' // @todo unimplemented
  quality?: number
  alphaQuality?: number
  method?: 0|1|2|3|4|5|6
  alphaMethod?: 0|1
  size?: number
  sns?: number
  filter?: number
  noAlpha?: boolean
  autoFilter?: boolean
  sharpness?: 0|1|2|3|4|5|6|7
  losslessPreset?: any
  lossless?: any
  nearLossless?: number
  pass: any
  crop?: [number, number, number, number] // @todo unimplemented
  resize?: [number, number] // @todo unimplemented
  metadata?: any // @todo unimplemented. Need to define how this is passed
}

export function createWebpSync(input: Buffer, options?: Options) : Buffer;

export function createWebp(input: Buffer, options?: Options) : Promise<Buffer>;
