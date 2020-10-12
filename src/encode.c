#include "rle.h"
#define WRITE_CACHE_SIZE  4000000

//encoder function
void* encoder(void *thread_args)
{
  int ret = 0;
  struct thread_data *t_args = (struct thread_data*)thread_args;
  char partial_encoded_file_name[5] = {};
  char write_cache[WRITE_CACHE_SIZE] = {};

  //sanity check
  if(t_args->in == NULL || t_args->out_fp == NULL || t_args->size == 0)
  {
    ret = -1;
  }

  //create file for thread to write encoded data in
  sprintf(partial_encoded_file_name, "%d", t_args->thread_id);
  if((t_args->out_fp= fopen(partial_encoded_file_name, "w")) == NULL)
  {
    ret = -2;
  }

  //iterate over input data
  unsigned int i = 0;
  while(t_args->size > 0)
  {
    char var = *(t_args->in);
    unsigned char count = 1;

    while( --(t_args->size)>0 && *(++(t_args->in)) == var)
    {
      count++;
      if(count == 0xFF)
      {
        (t_args->in)++;
        --(t_args->size);
        break;
      }
    }

    //for performance purposes, write in write_cache before dumping into file
    write_cache[i++] = var;
    write_cache[i++] = count;
    if(i == WRITE_CACHE_SIZE)
    {
      //write into file
      fwrite(write_cache, sizeof(char), WRITE_CACHE_SIZE,t_args->out_fp);
      i = 0;
    }
  }
  fwrite(write_cache, sizeof(char), i, t_args->out_fp);

  //clean up
  fclose(t_args->out_fp);
  pthread_exit(&ret);
}
