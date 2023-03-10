type MetadataOptions = 'exif'|'icc'|'xmp';
export type Options = {
  preset?: 'default'|'photo'|'picture'|'drawing'|'icon'|'text'
  hint?: 'photo'|'picture'|'graph'
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
  metadata?: 'none'|'all'|MetadataOptions|MetadataOptions[]
}

export function convertToWebpSync(input: Buffer, options?: Options) : Buffer;

export function convertToWebp(input: Buffer, options?: Options) : Promise<Buffer>;
