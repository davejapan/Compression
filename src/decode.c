#include "rle.h"

void* decoder(void *thread_args)
{
  int ret = 0;
  struct thread_data *t_args = (struct thread_data*)thread_args;
  unsigned char buffer[256] = {};
  char out_file_name[5] = {};

  if(t_args->in == NULL || t_args->out_fp == NULL || t_args->size < 2)
  {
    ret = -1;
  }

  sprintf(out_file_name, "%d", t_args->thread_id);
  if((t_args->out_fp= fopen(out_file_name, "w")) == NULL)
  {
    ret = -2;
  }

  while(t_args->size > 0)
  {
    unsigned char var = *(t_args->in);
    (t_args->in)++;
    unsigned char count = *(t_args->in);
    (t_args->in)++;

    // printf("%d %d \n", var, count);
    unsigned char i = 0;
    while( count >0)
    {
      buffer[i++] = var;
      count--;
    }

    // printf("%d \n\n", i);
    fwrite(buffer, sizeof(char), i, t_args->out_fp);
    t_args->size = t_args->size-2;
  }
  fclose(t_args->out_fp);
  pthread_exit(&ret);
}
