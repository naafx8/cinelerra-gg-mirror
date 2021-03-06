#include <stdlib.h>
#include <string.h>
#include "libzmpeg3.h"

#define BUFSIZE 65536

void test_32bit_overflow(char *outfile, int (*out_counter), FILE *(*out))
{
  if( ftell(*out) > 0x7f000000 ) {
    char string[1024];
    fclose(*out);
    sprintf(string, "%s%03d", outfile, ++(*out_counter));
    (*out) = fopen(string, "wb");
  }
}

int main(int argc, char *argv[])
{
  zmpeg3_t *file;
  int i, j, len, result = 0;
  int out_counter = 0;
  float *audio_output_f;
  unsigned char *audio_output_i;
  FILE *out;
  char outfile[1024];
  int decompress_audio = 0;
  int audio_track = 0;
/* Print cell offsets */
  int print_offsets = 0;
  int print_pids = 1;

  outfile[0] = 0;
  if( argc < 2 ) {
    printf( "Dump information or extract audio to a 24 bit pcm file.\n"
            "Example: dump -a0 outputfile.pcm take1.vob\n");
    exit(1);
  }

  for( i=1; i < argc; ++i ) {
    if( !strncmp(argv[i], "-a", 2) ) {
// Check for track number
      if( strlen(argv[i]) > 2 ) {
        audio_track = atol(argv[i] + 2);
      }

// Check for filename
      if( i + 1 < argc ) {
        strcpy(outfile, argv[++i]);
        decompress_audio = 1;
      }
      else {
        fprintf(stderr, "-a must be paired with a filename.\n");
        exit(1);
      }

// Check if file exists
      if( (out = fopen(outfile, "r")) ) {
        fprintf(stderr, "%s exists.\n", outfile);
        exit(1);
      }
    }
  }

  int error = 0;
  file = mpeg3_open(argv[argc - 1], &error);
  if( outfile[0] ) {
    out = fopen(outfile, "wb");
  }


  if( file ) {
// Audio streams
    fprintf(stderr, "total_astreams=%d\n", mpeg3_total_astreams(file));

    for( i=0; i < mpeg3_total_astreams(file); ++i ) {
      fprintf(stderr, "  Stream 0x%04x: channels=%d rate=%d samples=%ld format=%s\n", 
        file->atrack[i]->demuxer->astream, 
        mpeg3_audio_channels(file, i), 
        mpeg3_sample_rate(file, i),
        mpeg3_audio_samples(file, i),
        mpeg3_audio_format(file, i));
      
      if( print_offsets ) {
        fprintf(stderr, "total_sample_offsets=%d\n", file->atrack[i]->total_sample_offsets);
        for( j=0; j < file->atrack[i]->total_sample_offsets; ++j ) {
          fprintf(stderr, "%jx ", file->atrack[i]->sample_offsets[j]);
          if( j > 0 && !(j % 8)) fprintf(stderr, "\n" );
        }
        fprintf(stderr, "\n");
      }
    }

// Video streams
    fprintf(stderr, "total_vstreams=%d\n", mpeg3_total_vstreams(file));

    for( i=0; i < mpeg3_total_vstreams(file); ++i ) {
      fprintf(stderr, "  Stream 0x%04x: w=%d h=%d framerate=%0.3f frames=%ld coding=%s\n", 
        file->vtrack[i]->demuxer->vstream, 
        mpeg3_video_width(file, i), 
        mpeg3_video_height(file, i), 
        mpeg3_frame_rate(file, i),
        mpeg3_video_frames(file, i),
        mpeg3_colormodel(file, i) == zmpeg3_t::cmdl_YUV420P ? "420" : "422");
      
      if( print_offsets ) {
        fprintf(stderr, "total_frame_offsets=%d\n", file->vtrack[i]->total_frame_offsets);
        for( j=0; j < file->vtrack[i]->total_frame_offsets; ++j ) {
          fprintf(stderr, "%d=%jx ", j, file->vtrack[i]->frame_offsets[j]);
          if( j > 0 && !(j % 8)) fprintf(stderr, "\n" );
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "total_keyframe_numbers=%d\n", file->vtrack[i]->total_keyframe_numbers);
        for( j=0; j < file->vtrack[i]->total_keyframe_numbers; ++j ) {
          fprintf(stderr, "%d ", file->vtrack[i]->keyframe_numbers[j]);
          if( j > 0 && !(j % 8)) fprintf(stderr, "\n" );
        }
        fprintf(stderr, "\n");
      }
    }

// Subtitle tracks
    printf("total subtitle tracks: %d\n", mpeg3_subtitle_tracks(file));
    for( i=0; i < mpeg3_subtitle_tracks(file); ++i ) {
      zstrack_t *strack = file->strack[i];
      printf("  stream: 0x%02x total_offsets: %d\n", 
        strack->id, strack->total_offsets);
      if( print_offsets ) {
        for( j=0; j < strack->total_offsets; ++j ) {
          printf("%jx ", strack->offsets[j]);
        }
        printf("\n");
      }
    }

// Titles
    fprintf(stderr, "total_titles=%d\n", file->demuxer->total_titles);
    for( i=0; i < file->demuxer->total_titles; ++i ) {
      fprintf(stderr, "  Title path=%s total_bytes=%jx cell_table_size=%d\n", 
        file->demuxer->titles[i]->fs->path,
        file->demuxer->titles[i]->total_bytes, 
        file->demuxer->titles[i]->cell_table_size);
      
      if( print_offsets ) {
        for( j=0; j < file->demuxer->titles[i]->cell_table_size; ++j )
          fprintf(stderr, "    Cell: %jx-%jx %jx-%jx\n", 
            file->demuxer->titles[i]->cell_table[j].program_start, 
            file->demuxer->titles[i]->cell_table[j].program_end,
            file->demuxer->titles[i]->cell_table[j].title_start, 
            file->demuxer->titles[i]->cell_table[j].title_end);
      }
    }

// Pids
    if( print_pids ) {
      zdemuxer_t *demuxer = file->demuxer;
      printf("Total PIDs=%d\n", demuxer->total_pids);
      for( i=0; i < demuxer->total_pids; ++i ) {
        printf("0x%04x ", demuxer->pid_table[i]);
      }
      printf("\n");
    }

// Write audio
    if( decompress_audio ) {
      mpeg3_set_cpus(file, 2);
      audio_output_f = new float[BUFSIZE];
      len = BUFSIZE * 3 * mpeg3_audio_channels(file, audio_track);
      audio_output_i = new unsigned char[len];

//printf("%d\n", mpeg3_end_of_audio(file, audio_track));
      while( !mpeg3_end_of_audio(file, audio_track) && !result ) {
        test_32bit_overflow(outfile, &out_counter, &out);
        
        for( i=0; i < mpeg3_audio_channels(file, audio_track); ++i ) {
          if( i == 0 )
              result = mpeg3_read_audio(file, audio_output_f, 
                          0, i, BUFSIZE, audio_track);
          else /* Pointer to pre-allocated buffer of floats */
            result = mpeg3_reread_audio(file, audio_output_f,
                          0, i, BUFSIZE, audio_track);
          for( j=0; j < BUFSIZE; ++j ) {
            int sample = audio_output_f[j] * 0x7fffff;
            unsigned char * output_i = audio_output_i + j * 3 *
              mpeg3_audio_channels(file, audio_track) + i * 3;
            if( sample > 0x7fffff ) 
              sample = 0x7fffff;
            else if( sample < -0x7fffff )
              sample = -0x7fffff;
            *output_i++ = (sample & 0xff0000) >> 16;
            *output_i++ = (sample & 0xff00) >> 8;
            *output_i = sample & 0xff;
          }
        }

        len = BUFSIZE * 3 * mpeg3_audio_channels(file, audio_track);
        result = !fwrite(audio_output_i, len, 1, out);
      }
    }

/*
 *     len = BUFSIZE * 2 * mpeg3_audio_channels(file, 0);
 *     audio_output_i = unsigned char[len];
 *     mpeg3_seek_percentage(file, 0.1);
 *     result = mpeg3_read_audio(file, 0, audio_output_i, 1, BUFSIZE, 0);
 */

/*
 *       len = mpeg3_video_width(file, 0) * mpeg3_video_height(file, 0) * 3 + 4;
 *       output = unsigned char[len];
 *       len = sizeof(unsigned char*) * mpeg3_video_height(file, 0);
 *       output_rows = unsigned char *[len];
 *       for( i=0; i < mpeg3_video_height(file, 0); ++i )
 *         output_rows[i] = &output[i * mpeg3_video_width(file, 0) * 3];
 * printf("dump 1\n");
 *      mpeg3_seek_percentage(file, 0.375);
 *      result = mpeg3_read_frame(file, 
 *            output_rows, 
 *            0, 
 *            0, 
 *            mpeg3_video_width(file, 0), 
 *           mpeg3_video_height(file, 0), 
 *            mpeg3_video_width(file, 0), 
 *            mpeg3_video_height(file, 0), 
 *           MPEG3_RGB888, 
 *            0);
 * printf("dump 2\n");
 */

    mpeg3_close(file);
  }
  return 0;
}
