#include "rle.h"

//decoder function
void* decoder(void *thread_args)
{
  int ret = 0;
  struct thread_data *t_args = (struct thread_data*)thread_args;
  unsigned char buffer[256] = {};
  char out_file_name[THREAD_OUTPUT_FILE_NAME_SIZE] = {};

  //sanity check
  if(t_args->in == NULL || t_args->out_fp == NULL || t_args->size < 2)
  {
    ret = -1;
  }

  //create file for thread to write encoded data in
  sprintf(out_file_name, "%d", t_args->thread_id);
  if((t_args->out_fp= fopen(out_file_name, "w")) == NULL)
  {
    ret = -2;
  }

  //iterate over input data
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

    fwrite(buffer, sizeof(char), i, t_args->out_fp);
    t_args->size = t_args->size-2;
  }

  //clean up
  fclose(t_args->out_fp);
  pthread_exit(&ret);
}
