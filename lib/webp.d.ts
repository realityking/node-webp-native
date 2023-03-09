export type Options = {
  preset?: 'default'|'photo'|'picture'|'drawing'|'icon'|'text' // @todo unimplemented
  quality?: number // @todo unimplemented
  alphaQuality?: number  // @todo unimplemented
  method?: 0|1|2|3|4|5|6 // @todo unimplemented
  size?: number // @todo unimplemented
  sns?: number // @todo unimplemented
  filter?: number // @todo unimplemented
  noAlpha?: boolean // @todo unimplemented
  autoFilter?: boolean // @todo unimplemented
  sharpness?: 0|1|2|3|4|5|6|7
  lossless?: any // @todo unimplemented. Might split the definition
  nearLossless?: number // @todo unimplemented
  crop?: [number, number, number, number] // @todo unimplemented
  resize?: [number, number] // @todo unimplemented
  metadata?: any // @todo unimplemented. Need to define how this is passed
}

export function createWebpSync(input: Buffer, options?: Options) : Buffer;

export function createWebp(input: Buffer, options?: Options) : Promise<Buffer>;
