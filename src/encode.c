#include "rle.h"

void* encoder(void *thread_args)
{
  int ret = 0;
  struct thread_data *t_args = (struct thread_data*)thread_args;
  char partial_encoded_file_name[5] = {};
  char write_cache[4000000] = {};

  if(t_args->in == NULL || t_args->out_fp == NULL || t_args->size == 0)
  {
    ret = -1;
  }

  sprintf(partial_encoded_file_name, "%d", t_args->thread_id);
  if((t_args->out_fp= fopen(partial_encoded_file_name, "w")) == NULL)
  {
    ret = -2;
  }

  unsigned int i = 0;
  while(t_args->size > 0)
  {
    char var = *(t_args->in);
    unsigned char count = 1;

    while( --(t_args->size)>0 && *(++(t_args->in)) == var)
    {
      count++;
      // printf("%d ", count);
      if(count == 255)
      {
        (t_args->in)++;
        --(t_args->size);
        break;
      }
    }

    write_cache[i++] = var;
    write_cache[i++] = count;
    if(i == 4000000)
    {
      fwrite(write_cache, sizeof(char), 4000000,t_args->out_fp);
      i = 0;
    }
  }
  fclose(t_args->out_fp);
  pthread_exit(&ret);
}
