#include "../libzmpeg3.h"

int pcm_check(uint8_t *header)
{
  if( header[0] == ((zmpeg3_t::PCM_START_CODE >> 24) & 0xff) &&
      header[1] == ((zmpeg3_t::PCM_START_CODE >> 16) & 0xff) &&
      header[2] == ((zmpeg3_t::PCM_START_CODE >>  8) & 0xff) &&
      header[3] == ((zmpeg3_t::PCM_START_CODE >>  0) & 0xff) )
    return 0;
  return 1;
}

int zaudio_decoder_pcm_t::
pcm_header(uint8_t *data)
{
  if( pcm_check(data) ) return 0;

/* Custom header generated by the demuxer */
  samplerate = *(int32_t*)(data + 4);
  bits = *(int32_t*)(data + 8);
  channels = *(int32_t*)(data + 12);
  framesize = *(int32_t*)(data + 16);

  return framesize;
}

int zaudio_decoder_pcm_t::
do_pcm(uint8_t *zframe, int zframe_size, float **zoutput, int render)
{
  int i, j;
  int bytes_per_sample = bits / 8 * channels;
  int output_size =
    (zframe_size - zaudio_t::PCM_HEADERSIZE) / bytes_per_sample;
//zmsgs("2 %d\n", zframe_size);

  if( render ) {
    for( i=0; i < channels; ++i ) {
//zmsg("3\n");
      float *output_channel = zoutput[i];
//zmsg("4\n");
      switch( bits ) {
      case 16:
//zmsg("5\n");
        uint8_t *input =
          zframe + zaudio_t::PCM_HEADERSIZE + bits / 8 * i;
//zmsg("6\n");
        for( j=0; j < output_size; ++j ) {
          int16_t sample = ((int16_t)(input[0])) << 8;
          sample |= input[1];
          *output_channel = (float)sample / 32767.0;
          input += bytes_per_sample;
          ++output_channel;
        }
        break;
//zmsg("7\n");
      }
    }
  }

//zmsgs("2 %02x%02x%02x%02x\n", 
//  *(uint8_t*)(zframe+zaudio_t::PCM_HEADERSIZE + 0),
//  *(uint8_t*)(zframe+zaudio_t::PCM_HEADERSIZE + 1),
//  *(uint8_t*)(zframe+zaudio_t::PCM_HEADERSIZE + 2),
//  *(uint8_t*)(zframe+zaudio_t::PCM_HEADERSIZE + 3));
  return output_size;
}

